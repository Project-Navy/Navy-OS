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


#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "kernel/log.h"


size_t
strlen(const char *s)
{
    size_t i;

    for (i = 0; s[i] != '\0'; i++);
    return i;
}


char *
strcat(char *dest, const char *src)
{
    size_t dest_len = strlen(dest);
    size_t i;

    for (i = 0 ; src[i] != '\0' ; i++)
    {
        dest[dest_len + i] = src[i];
    }
    
    dest[dest_len + i] = '\0';

    return dest; 
}

char *
memcpy(void *dst, const void *src, size_t count)
{
    char *s = (char *) src;
    char *d = (char *) dst;

    while (count-- > 0)
    {
        *s++ = *d++;
    }

    return dst;
}

char *
strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    for (i = 0; i < n && src[i]; i++)
    {
        dest[i] = src[i];
    }

    for (; i < n; i++)
    {
        dest[i] = 0;
    }

    return dest;
}


int
memcmp(const void *aptr, const void *bptr, size_t size)
{
    const unsigned char *a = (const unsigned char *) aptr;
    const unsigned char *b = (const unsigned char *) bptr;
    size_t i;


    for (i = 0; i < size; i++)
    {
        if (a[i] < b[i])
        {
            return -1;
        }

        else if (b[i] < a[i])
        {
            return 1;
        }
    }

    return 0;
}

int
strncmp(const char *s1, const char *s2, size_t n)
{
    while (n && *s1 && (*s1 == *s2))
    {
        ++s1;
        ++s2;
        --n;
    }

    if (n == 0)
    {
        return 0;
    }

    else
    {
        return (*(unsigned char *) s1 - *(unsigned char *) s2);
    }

}

void *
memset(void *s, int c, size_t n)
{
    size_t i;
    unsigned char *buf = (unsigned char *) s;

    for (i = 0; i < n; i++)
    {
        buf[i] = (unsigned char) c;
    }

    return s;
}

char *
strrev(char *s)
{
    /*
     * Thanks to d0p1s4m4 <3 
     */

    int start;
    int end;
    char tmp;

    start = 0;
    end = strlen(s) - 1;

    while (start < end)
    {
        tmp = s[start];
        s[start] = s[end];
        s[end] = tmp;

        start++;
        end--;
    }

    return s;
}

char *
strcpy(char *dest, const char *src)
{
    size_t index = 0;

    while (*src)
    {
        dest[index++] = *src++;
    }

    dest[index] = '\0';

    return dest;
}

int
strcmp(const char *s1, const char *s2)
{
    size_t max_length;
    size_t i;
    int if_eq;

    if (strlen(s1) > strlen(s2))
    {
        max_length = strlen(s2);
        if_eq = 100;
    }
    else if (strlen(s1) < strlen(s2))
    {
        max_length = strlen(s1);
        if_eq = -100;
    }
    else
    {
        max_length = strlen(s1);
        if_eq = 0;
    }

    for (i = 0; i < max_length; i++)
    {
        if (s1[i] < s2[i] || s1[i] > s2[i])
        {
            return s1[i] - s2[i];
        }
    }

    return if_eq;
}

char *
strdup(char *s)
{
    char *return_value = (char *) malloc(strlen(s)+1);
    strcpy(return_value, s);

    return return_value;
}