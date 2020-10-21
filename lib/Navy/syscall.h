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


#ifndef _NAVY_ABI_SYSCALL_H
#define _NAVY_ABI_SYSCALL_H

#include <stdint.h>

typedef enum SYSCALL
{
    SYS_syslog,
    SYS_tkill,
    SYS_gettid,
    SYS_usleep
} Syscall;

typedef int pid_t;

int sys_tkill(pid_t, int);
pid_t sys_getpid(void);
uint32_t syscall(uint32_t, uint32_t, uint32_t);

#endif
