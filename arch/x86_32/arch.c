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

#include "arch/x86_32/interrupt/idt.h"
#include "arch/x86_32/memory/gdt.h"
#include "arch/x86_32/memory/virtual.h"
#include "arch/x86_32/memory/task.h"

#include "kernel/log.h"

#include <Navy/libmultiboot.h>
#include <Navy/assert.h>

#include <multiboot2.h>
#include <stivale2.h>
#include <string.h>
#include <stddef.h>


void
init_arch(BootInfo * info)
{
    init_x86(info);
    init_paging(info);
    init_term();

    init_gdt();
    klog(OK, "GDT loaded\n");

    init_idt();
    klog(OK, "IDT loaded\n");

    init_tasking();
}

void
boot_multiboot2(uintptr_t addr, uint32_t magic)
{
    BootInfo boot_info;

    assert(magic == MULTIBOOT2_BOOTLOADER_MAGIC);
    multiboot2_parse_header(&boot_info, addr);

    init_arch(&boot_info);
}

void
boot_stivale2(struct stivale2_struct *info)
{
    BootInfo boot_info;

    assert(strcmp(info->bootloader_brand, "Limine") == 0);
    stivale2_parse_header(&boot_info, info);

    init_arch(&boot_info);
}
