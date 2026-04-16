#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdbool.h>

#define MAX_TASKS (16)

typedef enum action { MOTORS_OFF, IOT_INIT } Action;
typedef struct task {
    Action act;
    unsigned int ticks; 
} Task;

static Task tasks[MAX_TASKS];
static unsigned int tasks_len;
bool next_tick;

void init_scheduler(void);
void scheduler_process(void);
void schedule_task(Action act, unsigned int duration);
void execute_action(Action act);

#endif
