/*
 * Copyright (C) 2020 Jordan DALCQ & contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stivale2.h>
#include <string.h>
#include <Navy/libmultiboot.h>
#include <multiboot2.h>

void 
stivale2_parse_mmap(BootInfo *info, struct stivale2_struct_tag_memmap *m)
{
    Range range;
    MemoryMapEntry *entry;
    struct stivale2_mmap_entry mmap;

    size_t i;

    info->memory_map_size = 0;
    info->memory_usable = 0;

    for (i = 0; i < m->entries; i++)
    {
        mmap = m->memmap[i];

        if (mmap.base > UINTPTR_MAX || (mmap.base + mmap.length > UINTPTR_MAX))
        {
            continue;
        }

        if (mmap.type == STIVALE2_MMAP_USABLE || mmap.type == STIVALE2_MMAP_KERNEL_AND_MODULES)
        {
            info->memory_usable += mmap.length;
        }

        entry = &info->mmap[info->memory_map_size];

        range.begin = mmap.base;
        range.size = mmap.length;

        align_range(&range);

        entry->range = range;
        
        switch (mmap.type)
        {
            case STIVALE2_MMAP_USABLE:
                /* FALLTROUGH */
            case STIVALE2_MMAP_KERNEL_AND_MODULES:
                entry->type = MULTIBOOT_MEMORY_AVAILABLE;
                break;
            case STIVALE2_MMAP_ACPI_RECLAIMABLE:
                entry->type = MULTIBOOT_MEMORY_ACPI_RECLAIMABLE;
                break;
            case STIVALE2_MMAP_ACPI_NVS:
                entry->type = MULTIBOOT_MEMORY_NVS;
                break;
            case STIVALE2_MMAP_BAD_MEMORY:
                entry->type = MULTIBOOT_MEMORY_BADRAM;
                break;
            default:
               entry->type = MULTIBOOT_MEMORY_RESERVED;
               break;
        }

        info->memory_map_size++;
    }
}

void 
stivale2_parse_module(BootInfo *info, struct stivale2_struct_tag_modules *m)
{
    Module *module;
    Range range;
    size_t i;

    for (i = 0; i < m->module_count; i++)
    {
        struct stivale2_module me = m->modules[i];
        module = &info->modules[info->modules_size];

        range.begin = me.begin;
        range.size = me.end - me.begin;
        align_range(&range);
        
        strncmp(module->cmd, (const char *) me.string, STIVALE2_MODULE_STRING_SIZE);

        info->modules_size++;
    }
}

void 
stivale2_parse_header(BootInfo *info, struct stivale2_struct *stivale)
{
    struct stivale2_tag *tag = (struct stivale2_tag *) ((uintptr_t) stivale->tags);

    while (tag != NULL)
    {
        switch(tag->identifier)
        {
            case STIVALE2_STRUCT_TAG_RSDP_ID:
                info->rsdp = (uintptr_t) ((struct stivale2_struct_tag_rsdp *) tag)->rsdp;                
                break;
            case STIVALE2_STRUCT_TAG_MEMMAP_ID:
                stivale2_parse_mmap(info, (struct stivale2_struct_tag_memmap *)tag);
                break;
            case STIVALE2_STRUCT_TAG_MODULES_ID:
                stivale2_parse_module(info, (struct stivale2_struct_tag_modules *)tag);
                break;
        }

        tag = (struct stivale2_tag *)((uintptr_t) tag->next);
    }
}