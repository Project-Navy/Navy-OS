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
vector_free(Vector *vector)
{
    free(vector->array);
    vector->capacity = 0;
    vector->length = 0;
}

char *
vector_join(Vector vector, char c)
{
    size_t i;

    char return_value[100];
    memset(return_value, 0, 100);

    for (i = 0; i < vector.length; i++)
    {
        strcat(return_value+strlen(return_value), vector_get(vector, i));
        return_value[strlen(return_value)] = c;
    }

    return_value[strlen(return_value)-1] = '\0';
    return strdup(return_value);
}

void 
vector_push_back(Vector *vector, void *to_push)
{
    if (vector->length == vector->capacity)
    {
        vector->capacity += 32;
        vector->array = (void **) realloc(vector->array, sizeof(void *) * vector->capacity);
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

    if (vector.length == 0)
    {
        klog(OK, "[]");
        return;
    }

    klog(OK, "[ ");

    for (i = 0; i < vector.length-1; i++)
    {
        klog(NONE, "%s, ", (char *) vector_get(vector, i));
    }

    klog(NONE, "%s ]\n", (char *) vector_get(vector, i));
}

Vector 
vector_split(char *str, char del)
{
    Vector vector;
    char tmp[512];

    size_t i = 0;

    init_vector(&vector);

    while(*str)
    {
        if (*str == del && i > 0)
        {
            tmp[i] = '\0';
            vector_push_back(&vector, strdup(tmp));
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

    tmp[i] = '\0';
    vector_push_back(&vector, strdup(tmp));

    return vector;
}