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

#include "arch/x86_32/memory/task.h"
#include "arch/x86_32/memory/virtual.h"
#include "arch/arch.h"
#include "kernel/log.h"

#include <stdlib.h>
#include <string.h>

#include <Navy/assert.h>

#define STACK_SIZE 0x1000

task_t tasks[5] = { 0 };

int pid_limit = 5;
int pid = -1;
int new_pid = 0;

int
create_task(char *name, void (*thread)())
{
    size_t i;
    int last_pid;
    uintptr_t addr;
    task_t *task;
    struct InterruptStackFrame *stackframe;

    disable_interrupts();
    assert(strlen(name) < 32);

    task = (task_t *) malloc(sizeof(task_t));
    memory_alloc(kernel_address_space(), STACK_SIZE, MEMORY_CLEAR, &addr);

    task->stack_range.begin = addr;
    task->stack_range.size = STACK_SIZE;

    task->joinee_pid = 0;
    task->pid = new_pid;
    task->stack = addr + STACK_SIZE;
    task->address_space = kernel_address_space();   /* It's temporary */


    for (i = 0; i < 64; i++)
    {
        init_vfsnode(&task->vfsnodes[i]);
    }

    stackframe =
        (struct InterruptStackFrame *) (addr - sizeof(struct InterruptStackFrame));

    stackframe->eflags = 0x202;
    stackframe->cs = 0x08;
    stackframe->eip = (uint32_t) thread;

    /*
     * Pusha 
     */
    stackframe->edi = 0;
    stackframe->esi = 0;
    stackframe->ebp = 0;
    stackframe->ebx = 0;
    stackframe->edx = 0;
    stackframe->ecx = 0;
    stackframe->ebx = 0;
    stackframe->eax = 0;

    /*
     * Data segments 
     */
    stackframe->ds = 0x10;
    stackframe->es = 0x10;
    stackframe->fs = 0x10;
    stackframe->gs = 0x10;

    task->state = RUNNING;
    task->stack = (unsigned int) stackframe;

    task->thread = thread;
    strncpy(task->name, (const char *) name, strlen((const char *) name));
    klog(OK, "Task %d (%s) created !\n", task->pid, task->name);

    tasks[new_pid] = *task;
    last_pid = new_pid++;

    enable_interrupts();
    return last_pid;
}

void
idle(void)
{
    while (1)
    {
        hlt();
    }
}

void
init_tasking(void)
{
    pid = create_task("System", NULL);
    create_task("Task Slayer", task_slayer);
    create_task("Idle", idle);
}

void
exit_task(int return_value)
{
    tasks[pid].return_value = return_value;
    tasks[pid].state = DEPRESSED;

    task_yield();
    __builtin_unreachable();
}

uint32_t
task_get_pid(void)
{
    return pid;
}

void
slay_task(task_t * task)
{
    disable_interrupts();

    memory_free(kernel_address_space(), task->stack_range);
    task->state = DEAD;

    klog(LOG, "%s  has been slayed\n", task->name);

    enable_interrupts();
}

void
task_slayer(void)
{
    int i;

    while (1)
    {
        task_sleep(100);

        for (i = 0; i < new_pid; i++)
        {
            if (tasks[i].state == GONNADIE)
            {
                slay_task(&tasks[i]);
            }
        }

    }
}

void
task_yield(void)
{
    __asm__("int $70");
}

void
task_sleep(uint32_t milli)
{
    disable_interrupts();
    tasks[pid].state = SLEEPING;
    tasks[pid].wakeup_tick = milli + fetch_tick();
    enable_interrupts();

    task_yield();
}

int
task_wait(uint32_t wait_pid)
{
    disable_interrupts();

    if (tasks[wait_pid].state == DEAD)
    {
        return -1;
    }

    tasks[pid].state = JOINING;
    tasks[pid].joinee_pid = wait_pid;
    enable_interrupts();

    task_yield();

    return tasks[pid].return_value;
}

void
update_task_state(void)
{
    int i;
    task_t *joinee;

    for (i = 0; i < pid_limit; i++)
    {
        if (tasks[i].state == JOINING)
        {
            joinee = &tasks[tasks[i].joinee_pid];

            if (joinee->state == DEPRESSED)
            {
                tasks[i].return_value = joinee->return_value;
                joinee->state = GONNADIE;
                tasks[i].state = RUNNING;
            }
        }

        if (tasks[i].state == SLEEPING)
        {
            if (tasks[i].wakeup_tick <= fetch_tick())
            {
                tasks[i].state = RUNNING;
            }
        }

    }
}

unsigned int
sched(unsigned int context)
{
    if (pid == -1)
    {
        return context;
    }

    update_task_state();
    tasks[pid].stack = context;

    do
    {
        pid++;

        if (pid >= new_pid)
        {
            pid = 0;
        }
    }
    while (tasks[pid].state != RUNNING);

    return tasks[pid].stack;
}
