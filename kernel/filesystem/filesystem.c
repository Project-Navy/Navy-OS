
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

#include "kernel/filesystem/filesystem.h"
#include "kernel/log.h"

#include <vector.h>
#include <string.h>

void
find_parent(struct PATH_NODE *child, Vector path, Vector nodes)
{
    size_t i;
    struct PATH_NODE *parent_node;

    char *parent_path;

    if (path.length == 0)
    {
        child->parent = NULL;
    }

    parent_path = vector_join(path, '/');
    strcat(parent_path, "/");

    for (i = 0; i < nodes.length; i++)
    {
        parent_node = (struct PATH_NODE *) vector_get(nodes, i);

        if (strcmp(parent_path, parent_node->header->name) == 0)
        {
            vector_push_back(&parent_node->children, child);
            child->parent = parent_node;
        }
    }
}

uintptr_t 
find_node(const char *filename, Vector nodes)
{
    size_t i;
    struct PATH_NODE *node;
    struct TAR_HEADER *header;

    filename++;
    for (i = 0; i < nodes.length; i++)
    {
        node = (struct PATH_NODE *) vector_get(nodes, i);
        header = node->header;

        if (strcmp(header->name, filename) == 0)
        {
            klog(OK, "%x\n", header);
        }
    }

    return 0;
}