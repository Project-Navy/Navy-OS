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

#pragma GCC diagnostic ignored "-Wpedantic" /* Just because of the bit field (I'm so sad) 
                                             */

#ifndef _NAVY_X86_GDT_H_
#define _NAVY_X86_GDT_H_

#define GDT_SIZE 6

#include <stdint.h>
#include <stddef.h>

struct gdtdesc
{                               /* https://wiki.osdev.org/File:Gdtr.png */
    uint16_t size;
    uint32_t offset;
} __attribute__((packed));

struct gdtentry
{                               /* https://wiki.osdev.org/File:GDT_Entry.png */
    uint16_t limit0_15;
    uint16_t base0_15;
    uint8_t base16_23;
    uint8_t access;
    uint8_t limit16_19:4;
    uint8_t flags:4;
    uint8_t base24_31;
} __attribute__((packed));


enum gdtbit
{                               /* https://wiki.osdev.org/File:Gdt_bits_fixed.png */
    /*
     * Access Byte 
     */
    PRESENT = 0x80,             /* 0b10000000 */
    SYSTEM = 0x10,              /* 0b00010000 */
    USER_PRIV = 0x60,           /* 0b01100000 */
    EXECUTABLE = 0x08,          /* 0b00001000 */
    GROWS_DOWN = 0x04,          /* 0b00000100 */
    READ_WRITE = 0x02,          /* 0b00000010 */
    ACCESSED = 0x01,            /* 0b00000001 */

    /*
     * Flag 
     */
    BYTE_GR = 0x00,             /* 0b0000 */
    PAGE_GR = 0x08,             /* 0b1000 */
    BITS16 = 0x00,              /* 0b0000 */
    BITS32 = 0x04,              /* 0b0100 */
    BITS64 = 0x02               /* 0b0010 */
};

struct tssentry
{                               /* https://wiki.osdev.org/Getting_to_Ring_3#The_TSS */
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iopb_offset;
} __attribute__((packed));


void init_gdt_desc(uint32_t, uint32_t, uint8_t, uint8_t, struct gdtentry *);
void init_gdt(void);

void set_kernel_stack(uint32_t);

extern void gdt_flush(uint32_t);
extern void tss_flush(void);

#endif
