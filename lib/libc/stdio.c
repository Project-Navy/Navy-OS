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

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>

#include "arch/arch.h"
#include "kernel/log.h"

int
getchar(void)
{
    char c = kbd_getc();

    return (int) c;
}

char *
gets(char *s)
{
    size_t i = 0;

    while (getchar() != '\n')
    {
        s[i] = getchar();
        vga_putc(s[i]);
        i++;
    }

    s[i++] = '\0';

    return s;
}

void
vs_printf(char *str, const char *format, va_list ap)
{
    char pad[2];
    char nbr[64];

    uint32_t padding = 0;
    size_t index = 0;
    const char *ptr = format;
    bool is_parsing = false;

    while (*ptr)
    {
        if (*ptr == '%')
        {
            if (is_parsing)
            {
                str[index++] = '%';
                ptr++;
                is_parsing = false;
            }

            else
            {
                is_parsing = true;
                ptr++;
                continue;
            }
        }

        if (*ptr == '0' && is_parsing)
        {
            pad[0] = *++ptr;
            pad[1] = '\0';
            padding = atoi(pad);
            ptr++;
        }

        if (*ptr == 's' && is_parsing)
        {
            const char *s = va_arg(ap, const char *);

            strcpy(str + index, s);

            is_parsing = false;
            index += strlen(s);
        }

        else if (*ptr == 'd' && is_parsing)
        {
            itoa(va_arg(ap, int), nbr, 10);

            while (padding && padding - strlen(nbr) > 0)
            {
                str[index++] = '0';
                padding--;
            }

            padding = 0;

            strcpy(str + index, nbr);
            index += strlen(nbr);
            is_parsing = false;
        }

        else if (*ptr == 'c' && is_parsing)
        {
            str[index++] = (char) va_arg(ap, int);

            is_parsing = false;
        }

        else if (*ptr == 'x' && is_parsing)
        {
            itoa(va_arg(ap, int), nbr, 16);

            while (padding && padding - strlen(nbr) > 0)
            {
                str[index++] = '0';
                padding--;
            }

            padding = 0;

            strcpy(str + index, nbr);
            index += strlen(nbr);

            is_parsing = false;
        }

        else
        {
            str[index++] = *ptr;
        }

        if (*ptr)
        {
            ptr++;
        }
    }

    str[index] = '\0';
}
