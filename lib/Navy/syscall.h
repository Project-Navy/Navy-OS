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


#ifndef _NAVY_ABI_SYSCALL_H_
#define _NAVY_ABI_SYSCALL_H_

#include <stdint.h>
#include <Navy/macro.h>

typedef enum SYSCALL
{
    SYS_syslog,
    SYS_texit,
    SYS_gettid,
    SYS_usleep
} Syscall;

typedef int pid_t;

__no_return void sys_texit(int);
pid_t sys_getpid(void);
uintptr_t syscall(uintptr_t, uintptr_t, uintptr_t, uintptr_t);

#endif
