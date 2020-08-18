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


#include "kernel/warning.h"
#include "kernel/log.h"

#if defined(__i386__)
#include "arch/x86/serial.h"
#include "arch/x86/vga.h"
#endif

#include <stdlib.h>
#include <math.h>

void
kmain(void)
{
    serial_init(COM1);
    klog(COM1, LOG, "Navy started");

    term_init();
    term_puts("Hello Navy !\n", LIGHT_GREY, BLACK);
}
