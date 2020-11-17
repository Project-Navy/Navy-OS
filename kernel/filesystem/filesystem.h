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

#ifndef _NAVY_FILESYSTEM_H_
#define _NAVY_FILESYSTEM_H_

#include <stdint.h>
#include <vector.h>

#include "kernel/filesystem/ramdisk.h"

enum NODE_TYPE
{
    TARNODE
};

struct PATH_NODE 
{ 
    char filename[100];
    enum NODE_TYPE type;
    struct PATH_NODE *parent;
    Vector children;

    union 
    {
        struct TAR_HEADER *header;
    };
};

struct VFSNODE 
{
    struct PATH_NODE *node;

    size_t (*read)(struct VFSNODE, void *, size_t);
    size_t (*write)(struct VFSNODE, const void *, size_t);
};

uintptr_t find_node(const char *, Vector);
void find_parent(struct PATH_NODE *, Vector, Vector);
void init_vfsnode(struct VFSNODE *);

#endif