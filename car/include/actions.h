#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdbool.h>

#define MAX_TIMED_TASKS (16)
#define MAX_SEQ_TASKS (16)

typedef enum action { MOTORS_OFF, IOT_INIT, INIT_LINE_ALIGN, LINE_ALIGN, INIT_LINE_FOLLOW, LINE_FOLLOW } Action;
typedef struct timed_future {
    Action act;
    unsigned int ticks; 
} TimedFuture;

static TimedFuture timed_tasks[MAX_TIMED_TASKS];
static unsigned int timed_tasks_len;
bool timed_tasks_next_tick;

static Action sequential_tasks[MAX_SEQ_TASKS];
static unsigned int sequential_tasks_len;

void init_scheduler(void);
void scheduler_process(void);
void schedule_timed_task(Action act, unsigned int duration);
void queue_sequential_task(Action act);

static inline bool execute_action(Action act);
static inline bool motors_off_action(void);
static inline bool iot_init_action(void);

#endif
