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

#ifndef _NAVY_ARCH_H_
#define _NAVY_ARCH_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <Navy/libmultiboot.h>

#if defined(__i386__) || defined(__x86_64__)

#include "arch/x86/x86.h"
#define PAGE_SIZE (4096)

#else
void debug_print(const char *);
void debug_putc(const char);
void debug_clear(void);
void init_serial(void);

void vga_print(const char *);
void vga_printerr(const char *);
void vga_putc(char c);
void disable_vga_cursor(void);

void breakpoint(void);
void hlt(void);
void disable_interrupts(void);
void enable_interrupts(void);
void reboot(void);
void panic(char *, ...);

unsigned char kbd_getc(void);
char kbd_lastKeyCode(void);
#endif

void init_arch(BootInfo *);
#endif
