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

#include <stdarg.h>

#include <Navy/syscall.h>
#include <Navy/macro.h>

void
sys_texit(int return_value)
{
    exit_task(return_value);
}

pid_t
sys_gettid(void)
{
    pid_t pid;

    pid = task_get_pid();

    return pid;
}

uint32_t
syscall(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx)
{
    uint32_t return_value = 0;

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
            return_value = task_get_pid();
            break;

        case SYS_usleep:
            task_sleep(ebx / 1000);
            break;

        default:
            klog(ERROR, "Unkown syscall !\n");
            return_value = 1;
    }

    return return_value;
}
