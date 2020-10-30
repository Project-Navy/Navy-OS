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

#include <vector.h>
#include <stdlib.h>
#include <string.h>

#include <Navy/assert.h>

#include "kernel/log.h"

void 
init_vector(Vector *vector)
{
    vector->array = (void **) malloc(sizeof(void *) * 32);
    vector->capacity = 32;
    vector->length = 0;
}

void 
vector_push_back(Vector *vector, void *to_push)
{
    if (vector->length == vector->capacity)
    {
        vector->array = (void **) realloc(vector->array, sizeof(void *) * (vector->capacity+32));
        vector->capacity += 32;
    }

    vector->array[vector->length++] = to_push;
}

void *
vector_pop_back(Vector *vector)
{
    void *return_value;

    return_value = vector->array[vector->length-1];
    free(vector->array[vector->length-1]);

    vector->length--;
    return return_value;
}

void *
vector_get(Vector vector, size_t index)
{
    return vector.array[index];
}


void 
vector_dump_str(Vector vector)
{
    size_t i;

    klog(OK, "[ ");

    for (i = 0; i < vector.length; i++)
    {
        klog(NONE, "%s, ", (char *) vector_get(vector, i));
    }

    klog(NONE, " ]\n");
}

Vector 
vector_split(char *str, char del)
{
    Vector vector;
    char tmp[256];

    size_t i = 0;

    init_vector(&vector);

    
    while(*str)
    {
        if (*str == del && i > 0)
        {
            tmp[i] = '\0';
            klog(OK, "Pushing: %s\n", tmp);
            vector_push_back(&vector, tmp);
            memset(tmp, 0, i);
            i = 0;
        }
        else if(*str == del)
        {
            i = 0;
        }
        else 
        {
            tmp[i++] = *str;
        }

        str++;
    }

    klog(OK, "Pushing: %s\n", tmp);
    vector_push_back(&vector, tmp);

    return vector;
}