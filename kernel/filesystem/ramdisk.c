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
#include "kernel/filesystem/ramdisk.h"
#include "kernel/log.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <Navy/macro.h>

void
mount_ramdisk(BootInfo * info)
{
    size_t i;
    size_t header_length;

    Module ramdisk;

    for (i = 0; i < info->modules_size; i++)
    {
        if (strcmp(info->modules[i].cmd, "ramdisk") == 0)
        {
            ramdisk = info->modules[i];
        }
    }

    header_length = parse_tar(ramdisk.range);

    __unused(header_length);
}

size_t
getsize(const char *in)
{
    size_t j;
    size_t size = 0;
    size_t count = 1;

    for (j = 11; j > 0; j--, count *= 8)
    {
        size += ((in[j - 1] - '0') * count);
    }

    return size;
}

size_t
parse_tar(Range ramdisk_range)
{
    size_t i;
    size_t size;
    struct TAR_HEADER *header;

    uintptr_t addr = ramdisk_range.begin;

    for (i = 0;; i++)
    {
        header = (struct TAR_HEADER *) addr;

        if (header->name[0] == '\0')
        {
            break;
        }

        klog(OK, "%s\n", header->name);

        size = getsize(header->size);
        addr += ((size / 512) + 1) * 512;

        if (size % 512)
        {
            addr += 512;
        }
    }

    return i;
}
