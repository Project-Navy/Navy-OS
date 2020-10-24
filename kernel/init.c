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
#include "kernel/log.h"
#include "kernel/ascii.h"

#include <pthread.h>
#include <unistd.h>

void
a(void)
{
    size_t i;

    for (i = 0; i < 10; i++)
    {
        klog(OK, "[ %d ] A\n", i);
        usleep(500000);
    }

    pthread_exit("I'm dead");
}

void
b(void)
{
    size_t i;

    for (i = 0; i < 20; i++)
    {
        klog(OK, "[ %d ] B\n", i);
        usleep(500000);
    }

    pthread_exit("B");
}

void
init(void)
{
    klog(NONE, ascii_art);

    vga_print(ascii_art);
    for (;;);
}
