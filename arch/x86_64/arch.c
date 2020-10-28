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

#include "arch/arch.h"
#include "kernel/log.h"

#include <Navy/libmultiboot.h>
#include <Navy/assert.h>

#include <stivale2.h>
#include <string.h>
#include <stddef.h>

typedef uint8_t stack[4096]; 
static stack stacks[10] = {0};

void boot_stivale2(struct stivale2_struct *);

__attribute__((section(".stivale2hdr"), used))
struct stivale2_header header2 = {
    (uint64_t) boot_stivale2,
    (uintptr_t) stacks[0] + sizeof(stack),
    0,
    0
};

void 
boot_stivale2(struct stivale2_struct *info)
{
    BootInfo boot_info;

    assert(strcmp(info->bootloader_brand, "Limine") == 0);
    stivale2_parse_header(&boot_info, info);

    klog(OK, "Hello 64 bits !\n");

    hlt();
    disable_interrupts();
}