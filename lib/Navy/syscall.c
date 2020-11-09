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


#include "kernel/log.h"

#include "arch/x86_32/memory/task.h"
#include "kernel/filesystem/ramdisk.h"

#include <stdarg.h>
#include <stdio.h>

#include <Navy/syscall.h>
#include <Navy/macro.h>

void
sys_texit(int return_value)
{
    __unused(return_value);
#if defined (__i386__)
    exit_task(return_value);
#else
    for (;;);
#endif
}

pid_t
sys_gettid(void)
{
    pid_t pid;

#if defined(__i386__)
    pid = task_get_pid();
#else
    pid = 0;
#endif

    return pid;
}

uintptr_t
syscall(uintptr_t eax, uintptr_t ebx, uintptr_t ecx, uintptr_t edx)
{
    uintptr_t return_value = 0;

    __unused(edx);

    switch (eax)
    {
        case SYS_syslog:
            klog(ebx, (char *) ecx);
            /*
             * Implement syslog
             * (https://www.kernel.org/doc/html/latest/core-api/printk-basics.html) 
             */
            break;
        case SYS_texit:
            sys_texit(ebx);
            break;

        case SYS_gettid:
#if defined(__i386__)
            return_value = task_get_pid();
#endif
            break;

        case SYS_usleep:
#if defined (__i386__)
            task_sleep(ebx / 1000);
#endif
            break;

        default:
            klog(ERROR, "Unkown syscall !\n");
            return_value = 1;
    }

    return return_value;
}
