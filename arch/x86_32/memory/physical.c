/*
 * Copyright (C) 2020  Jordan DALCQ & contributors 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "arch/arch.h"
#include "arch/x86_32/memory/physical.h"
#include "kernel/log.h"

#include <Navy/range.h>
#include <Navy/assert.h>

size_t USED_MEMORY = 0;
size_t TOTAL_MEMORY = 0;

uint8_t page_bitmap[1024 * 1024 / 8] = { };

size_t best_bet = 0;

void
init_bitmap(void)
{
    size_t i;

    for (i = 0; i < 1024 * 1024 / 8; i++)
    {
        page_bitmap[i] = USED;
    }
}

bool
physical_page_is_used(uintptr_t addr)
{
    uintptr_t page = addr / PAGE_SIZE;

    return page_bitmap[page / 8] & (1 << (page % 8));
}

void
physical_page_set_used(uintptr_t addr)
{
    uintptr_t page = addr / PAGE_SIZE;

    if (page == best_bet)
    {
        best_bet++;
    }

    page_bitmap[page / 8] |= 1 << (page % 8);
}

void
physical_set_free(Range range)
{
    size_t i;
    uintptr_t addr;

    if (!is_range_page_aligned(range))
    {
        panic("Range not page aligned !\n");
    }

    for (i = 0; i < range.size / PAGE_SIZE; i++)
    {
        addr = range.begin + i * PAGE_SIZE;

        if (physical_page_is_used(addr))
        {
            USED_MEMORY -= PAGE_SIZE;
            physical_page_set_free(addr);
        }
    }
}

bool
physical_is_used(Range range)
{
    size_t i;
    uintptr_t addr;

    if (!is_range_page_aligned(range))
    {
        panic("RANGE NOT ALIGNED!\n");
    }

    for (i = 0; i < range.size / PAGE_SIZE; i++)
    {
        addr = range.begin + i * PAGE_SIZE;

        if (physical_page_is_used(addr))
        {
            return true;
        }
    }

    return false;
}

void
physical_set_used(Range range)
{
    size_t i;
    uintptr_t address;

    if (!is_range_page_aligned(range))
    {
        panic("This memory range is not page aligned ! (BEGIN: %x, SIZE: %x)\n",
              range.begin, range.size);
    }

    for (i = 0; i < range.size / PAGE_SIZE; i++)
    {
        address = range.begin + i * PAGE_SIZE;

        if (!physical_page_is_used(address))
        {
            USED_MEMORY += PAGE_SIZE;
            physical_page_set_used(address);
        }
    }
}

Range
physical_alloc(size_t size)
{
    size_t i;
    Range range;

    assert(size % PAGE_SIZE == 0);

    for (i = best_bet; i < ((TOTAL_MEMORY - size) / PAGE_SIZE); i++)
    {
        range.begin = i * PAGE_SIZE;
        range.size = size;

        if (!physical_is_used(range))
        {
            physical_set_used(range);
            return range;
        }
    }

    panic("Out of physical memory !\tTrying to allocate %dkio but free memory is %dkio",
          size / 1024, TOTAL_MEMORY / 1024);
    return range;
}

void
physical_page_set_free(uintptr_t addr)
{
    uintptr_t page = addr / PAGE_SIZE;

    if (page < best_bet)
    {
        best_bet = page;
    }

    page_bitmap[page / 8] &= ~(1 << (page % 8));
}

void
set_total_memory(size_t mem)
{
    TOTAL_MEMORY = mem;
}
