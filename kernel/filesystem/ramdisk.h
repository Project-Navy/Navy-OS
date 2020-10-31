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

#ifndef _NAVY_FILESYSTEM_RAMDISK_H_
#define _NAVY_FILESYSTEM_RAMDISK_H_

#include <Navy/libmultiboot.h>
#include <Navy/range.h>

#include <vector.h>

struct TAR_HEADER
{
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
} __attribute__((packed));

struct TAR_NODE
{
    char filename[100];
    struct TAR_HEADER *header;
    struct TAR_NODE *parent;
    Vector children;
};

void mount_ramdisk(BootInfo *);
size_t parse_tar(Range);

#endif
