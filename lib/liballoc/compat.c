/*
 * Copyright (C) 2020  Jordan DALCQ & contributors
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

#include <liballoc/compat.h>
#include <stdbool.h>

#include <Navy/macro.h>

#include "arch/arch.h"
#include "kernel/log.h"
#include "arch/x86_32/memory/virtual.h"

bool lock = false;

int
liballoc_lock(void)
{
    disable_interrupts();
    return 0;
}

int
liballoc_unlock(void)
{
    enable_interrupts();
    return 0;
}

void *
liballoc_alloc(int pages)
{
    uintptr_t addr;

    __unused(pages);

#if defined(__i386__)
    memory_alloc(kernel_address_space(), pages * PAGE_SIZE, MEMORY_CLEAR, &addr);
#else
    addr = 0;
#endif
    return (void *) addr;
}

int
liballoc_free_(void *addr, int size)
{
    Range range;

    __unused(addr);
    __unused(size);
    __unused(range);

#if defined(__i386__)

    range.begin = (uintptr_t) addr;
    range.size = size * PAGE_SIZE;

    memory_free(kernel_address_space(), range);
#endif

    return 0;
}
