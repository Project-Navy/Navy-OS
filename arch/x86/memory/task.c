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

#include "arch/x86/memory/task.h"
#include "arch/x86/memory/virtual.h"
#include "arch/arch.h"
#include "kernel/log.h"

#include <stdlib.h>
#include <string.h>
#include <Navy/assert.h>

#define STACK_SIZE 0x1000

task_t tasks[4];
int pid = -1;
int new_pid = 0;

int
create_task(char *name, void (*thread)())
{
    int last_pid;
    uintptr_t addr;
    task_t *task;
    struct InterruptStackFrame *stackframe;

    assert(strlen(name) < 32);

    disable_interrupts();
    task = (task_t *) malloc(sizeof(task_t));
    memory_alloc(kernel_address_space(), STACK_SIZE, MEMORY_CLEAR, &addr);

    task->stack = addr + STACK_SIZE;

    stackframe = (struct InterruptStackFrame *) (addr - sizeof(struct InterruptStackFrame));

    stackframe->eflags = 0x202;
    stackframe->cs = 0x08;
    stackframe->eip = (uint32_t) thread;

    /* Pusha */
    stackframe->edi = 0;
    stackframe->esi = 0;
    stackframe->ebp = 0;
    stackframe->ebx = 0;
    stackframe->edx = 0;
    stackframe->ecx = 0;
    stackframe->ebx = 0;
    stackframe->eax = 0;

    /* Data segments */
    stackframe->ds = 0x10;
    stackframe->es = 0x10;
    stackframe->fs = 0x10;
    stackframe->gs = 0x10;

    task->state = 1;
    task->stack = (unsigned int) stackframe;

    task->thread = thread;
    strncpy(task->name,(const char *) name, strlen((const char *) name));

    tasks[new_pid] = *task;
    last_pid = new_pid++;

    enable_interrupts();

    return last_pid;
}

void 
init_tasking(void)
{
    pid = create_task("System", NULL);
}

unsigned int
sched(unsigned int context)
{
    if (pid == -1)
    {
        return context;
    }


    tasks[pid].stack = context;
    pid++;

    if (pid >= new_pid)
    {
        pid = 0;
    }

    klog(OK, "Switching to %s ESP: %x\n", tasks[pid].name, context);
    return tasks[pid].stack;
}

