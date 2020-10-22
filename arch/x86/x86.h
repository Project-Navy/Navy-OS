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

#ifndef _NAVY_X86_H_
#define _NAVY_X86_H_

#include <Navy/libmultiboot.h>

void init_x86(BootInfo *info);

void debug_print(const char *);
void debug_putc(const char);
void debug_clear(void);
void init_serial(void);
void init_term(void);

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

bool is_page_aligned(size_t);

#endif