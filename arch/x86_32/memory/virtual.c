/*
 * Copyright (C) 2020  Jordan DALCQ & contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 :* (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "arch/x86_32/memory/virtual.h"
#include "arch/x86_32/memory/physical.h"
#include "kernel/log.h"

#include <stdint.h>
#include <stddef.h>
#include <multiboot2.h>
#include <string.h>

#include <Navy/macro.h>
#include <Navy/assert.h>

extern int __start;
extern int __end;

static struct PAGE_DIR kernel_page_dir __attribute__((aligned(PAGE_SIZE))) = { };
static struct PAGE_TABLE kernel_page_table[256] __attribute__((aligned(PAGE_SIZE))) = { };

bool is_paging_enabled = false;

void
virtual_free(void *address_space, Range range)
{
    size_t i;
    size_t offset;
    size_t dir_index;
    size_t table_index;

    PageDirEntry *dir_entry;
    PageTableEntry *table_entry;
    struct PAGE_TABLE *table;
    struct PAGE_DIR *page_dir = (struct PAGE_DIR *) (address_space);

    for (i = 0; i < range.begin / PAGE_SIZE; i++)
    {
        offset = i * PAGE_SIZE;

        dir_index = __pd_index(range.begin + offset);
        dir_entry = &page_dir->entries[dir_index];

        if (!dir_entry->present)
        {
            continue;
        }

        table = (struct PAGE_TABLE *) (dir_entry->page_framenbr * PAGE_SIZE);

        table_index = __pt_index(range.begin + offset);
        table_entry = &table->entries[table_index];

        if (table_entry->present)
        {
            table_entry->as_uint = 0;
        }
    }
}

void *
kernel_address_space()
{
    return &kernel_page_dir;
}

void
memory_free(void *address_space, Range range)
{
    size_t i;
    uintptr_t virt_addr;

    Range page_phys_range;
    Range page_virt_range;

    assert(is_range_page_aligned(range));

    for (i = 0; i < range.size / PAGE_SIZE; i++)
    {
        virt_addr = range.begin + i * PAGE_SIZE;

        if (is_virtual_present(address_space, virt_addr))
        {
            page_phys_range.begin = virtual_to_physical(address_space, virt_addr);
            page_phys_range.size = PAGE_SIZE;

            page_virt_range.begin = virt_addr;
            page_virt_range.size = PAGE_SIZE;

            physical_set_free(page_phys_range);
            virtual_free(address_space, page_virt_range);
        }
    }
}

void
init_paging(BootInfo * info)
{
    Range kernel_range;
    Range page_zero;

    size_t i;
    PageDirEntry *dir_entry;

    init_bitmap();

    kernel_range.begin = (uintptr_t) (&__start);
    kernel_range.size = (size_t) (&__end) - (size_t) (&__start);

    page_zero.begin = 0;
    page_zero.size = PAGE_SIZE;

    for (i = 0; i < info->memory_map_size; i++)
    {
        MemoryMapEntry *entry = &info->mmap[i];

        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            physical_set_free(entry->range);
        }
    }

    for (i = 0; i < 256; i++)
    {
        dir_entry = &kernel_page_dir.entries[i];
        dir_entry->user = 0;
        dir_entry->write = 1;
        dir_entry->present = 1;
        dir_entry->page_framenbr = (size_t) &kernel_page_table[i] / PAGE_SIZE;
    }

    set_total_memory(info->memory_usable);
    align_range(&kernel_range);

    memory_map_identity(kernel_address_space(), kernel_range, MEMORY_NONE);

    for (i = 0; i < info->modules_size; i++)
    {
        memory_map_identity(kernel_address_space(), info->modules[i].range, MEMORY_NONE);
    }

    klog(OK, "Modules mapped\n");

    virtual_free(kernel_address_space(), page_zero);
    physical_set_used(page_zero);

    address_space_switch(kernel_address_space());

    _asm_init_paging();
    klog(OK, "Paging enabled !\n");
    is_paging_enabled = true;
}

bool
paging_enabled(void)
{
    return is_paging_enabled;
}

void
address_space_switch(void *address_space)
{
    uintptr_t addr =
        virtual_to_physical(kernel_address_space(), (uintptr_t) address_space);
    _asm_load_pagedir(addr);
}

bool
is_virtual_present(void *addr_space, uintptr_t virtual_addr)
{
    struct PAGE_TABLE page_table;
    int page_table_index;
    PageTableEntry page_table_entry;

    struct PAGE_DIR *page_directory = (struct PAGE_DIR *) (addr_space);

    int page_directory_index = __pd_index(virtual_addr);
    PageDirEntry page_dir_entry = page_directory->entries[page_directory_index];

    if (!page_dir_entry.present)
    {
        return false;
    }

    page_table = *(struct PAGE_TABLE *) (page_dir_entry.page_framenbr * PAGE_SIZE);

    page_table_index = __pt_index(virtual_addr);
    page_table_entry = page_table.entries[page_table_index];

    return page_table_entry.present;
}

Range
virtual_alloc(void *address_space, Range range, uint8_t mode)
{
    size_t i;
    Range final_range;
    uintptr_t current_addr;

    uintptr_t virt_addr = 0;
    size_t current_size = 0;
    bool is_user_memory = mode & MEMORY_USER;

    for (i = (is_user_memory ? 256 : 1) * 1024; i < (is_user_memory ? 1024 : 256) * 1024;
         i++)
    {
        current_addr = i * PAGE_SIZE;

        if (!is_virtual_present(address_space, current_addr))
        {
            if (current_size == 0)
            {
                virt_addr = current_addr;
            }

            current_size += PAGE_SIZE;

            if (current_size == range.begin)
            {
                virtual_map(address_space, range, virt_addr, mode);

                final_range.begin = virt_addr;
                final_range.size = current_size;

                return final_range;
            }
        }
        else
        {
            current_size = 0;
        }

    }

    panic("Out of virutal memory !");
    return range;
}

int
memory_alloc(void *address_space, size_t size, uint8_t mode, uintptr_t * out_addr)
{
    Range range;
    uintptr_t virt_addr;

    assert(size % 4096 == 0);
    assert(size != 0);

    *out_addr = 0;

    range = physical_alloc(size);

    virt_addr = virtual_alloc(address_space, range, mode).begin;

    if (mode & MEMORY_CLEAR)
    {
        memset((void *) virt_addr, 0, size);
    }

    *out_addr = virt_addr;
    return 0;

}

void
memory_alloc_identity(void *address_space, uint8_t mode, uintptr_t * out_addr)
{
    size_t i;

    for (i = 1; i < 256 * 1024; i++)
    {
        Range mem_range;

        mem_range.begin = i * PAGE_SIZE;
        mem_range.size = PAGE_SIZE;

        if (!is_virtual_present(address_space, mem_range.begin)
            && !physical_is_used(mem_range))
        {
            physical_set_used(mem_range);
            virtual_map(address_space, mem_range, mem_range.begin, mode);

            if (mode & MEMORY_CLEAR)
            {
                memset((void *) mem_range.begin, 0, PAGE_SIZE);
            }

            *out_addr = mem_range.begin;
            return;
        }

    }

    panic("Failed to allocate identity mapped page !");
    *out_addr = 0;

    disable_interrupts();
    hlt();
}

int
virtual_map(void *address_space, Range range, uintptr_t addr, uint8_t mode)
{
    size_t offset;
    size_t i;
    int page_directory_index;
    int page_table_index;

    PageDirEntry *page_dir_entry;
    PageTableEntry *page_table_entry;
    struct PAGE_TABLE *page_table;
    struct PAGE_DIR *page_dir = (struct PAGE_DIR *) address_space;

    for (i = 0; i < range.size / PAGE_SIZE; i++)
    {
        offset = i * PAGE_SIZE;

        page_directory_index = __pd_index(addr + offset);
        page_dir_entry = &page_dir->entries[page_directory_index];
        page_table = (struct PAGE_TABLE *) (page_dir_entry->page_framenbr * PAGE_SIZE);

        if (!page_dir_entry->present)
        {
            memory_alloc_identity(page_dir, MEMORY_CLEAR, (uintptr_t *) & page_table);

            page_dir_entry->present = 1;
            page_dir_entry->write = 1;
            page_dir_entry->user = 1;
            page_dir_entry->page_framenbr = (uint32_t) (page_table) >> 12;
        }

        page_table_index = __pt_index(addr + offset);
        page_table_entry = &page_table->entries[page_table_index];

        page_table_entry->present = 1;
        page_table_entry->write = 1;
        page_table_entry->user = mode & MEMORY_USER;
        page_table_entry->page_framenbr = (range.begin + offset) >> 12;
    }

    __unused(page_table_entry);
    _asm_reload_pagedir();
    return 0;
}

void
memory_map_identity(void *address_space, Range range, uint8_t mode)
{
    assert(is_range_page_aligned(range));

    physical_set_used(range);
    virtual_map(address_space, range, range.begin, mode);

    if (mode & MEMORY_CLEAR)
    {
        memset((void *) range.begin, 0, range.size);
    }
}

uintptr_t
virtual_to_physical(void *address_space, uintptr_t addr)
{
    int page_dir_index;
    int page_table_index;

    PageDirEntry page_dir_entry;
    PageTableEntry page_table_entry;

    struct PAGE_TABLE page_table;
    struct PAGE_DIR *page_dir = (struct PAGE_DIR *) (address_space);

    page_dir_index = __pd_index(addr);
    page_dir_entry = (PageDirEntry) page_dir->entries[page_dir_index];

    if (!page_dir_entry.present)
    {
        klog(ERROR, "PAGE DIR ENTRY NOT PRESENT\n");
        return 0;
    }

    page_table = *(struct PAGE_TABLE *) (page_dir_entry.page_framenbr * PAGE_SIZE);

    page_table_index = __pt_index(addr);
    page_table_entry = (PageTableEntry) page_table.entries[page_table_index];

    if (!page_table_entry.present)
    {
        klog(ERROR, "PAGE TABLE ENTRY NOT PRESENT\n");
        return 0;
    }

    return (page_table_entry.page_framenbr * PAGE_SIZE) + (addr & 0xfff);
}
