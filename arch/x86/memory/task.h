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

#ifndef _NAVY_x86_MEMORY_TASKS_H_
#define _NAVY_x86_MEMORY_TASKS_H_
#pragma GCC diagnostic ignored "-Wdeclaration-after-statement"

#include "arch/x86/interrupt/interrupt.h"

#include <stdint.h>
#include <stdbool.h>

#include <Navy/range.h>

typedef enum STATE
{
    RUNNING,
    GONNADIE,
    DEAD,
    SLEEPING
} state;

struct TASK
{
    uint32_t pid;
    uint32_t stack;
    uint8_t state;
    char name[32];
    void *address_space;
    Range stack_range;
    uint32_t wakeup_tick;

    void (*thread)();
};

typedef struct TASK task_t;

int create_task(char *name, void (*thread)());
int kill_task(int);
unsigned int sched(unsigned int context);
void init_tasking(void);
void task_slayer(void);
uint32_t task_get_pid(void);
void task_sleep(uint32_t);

#endif
