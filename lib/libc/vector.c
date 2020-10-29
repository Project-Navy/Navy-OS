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
    vector->array[vector->length-1] = NULL;
    vector->length--;

    return return_value;
}

void *
vector_get(Vector vector, size_t index)
{
    return vector.array[index];
}

size_t 
vector_length(Vector vector)
{
    return vector.length;
}
