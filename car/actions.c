#include "include/actions.h"
#include "include/motors.h"
#include "include/comms.h"
#include "include/line.h"

#include <stdbool.h>
#include <string.h>

void init_scheduler() {
    timed_tasks_len = 0;
    timed_tasks_next_tick = false;
}

void scheduler_process() {
    // Handle timed tasks
    if(timed_tasks_next_tick) {
        unsigned int tid;
        for (tid = 0; tid < timed_tasks_len; tid += 1) {
            timed_tasks[tid].ticks -= 1;
            if(timed_tasks[tid].ticks == 0) {
                execute_action(timed_tasks[tid].act);
                memmove(&timed_tasks[tid], &timed_tasks[tid + 1], (timed_tasks_len - 1 - tid)*sizeof(TimedFuture));
                timed_tasks_len -= 1;
            }
        }
        timed_tasks_next_tick = false;
    }

    // Handle queued tasks
    if(sequential_tasks_len > 0 && execute_action(sequential_tasks[0])) {
        // Executed task completed
        memmove(&sequential_tasks[0], &sequential_tasks[1], (sequential_tasks_len - 1)*sizeof(Action));
        sequential_tasks_len -= 1;
    }
}

static inline bool execute_action(Action act) {
    switch (act) {
        case MOTORS_OFF: return motors_off_action();
        case IOT_INIT: return iot_init_action();
        case INIT_LINE_ALIGN: return init_line_align_action();
        case LINE_ALIGN: return line_align_action();
        case INIT_LINE_FOLLOW: return init_line_follow_action();
        case LINE_FOLLOW: return line_follow_action();
        default: return false;
    }
}

static inline bool motors_off_action() {
    motors_off();
    return true;
}

static inline bool iot_init_action() {
    iot_msg(iot_init_frame[iot_init_frame_id]);
    iot_init_frame_id += 1;
    return true;
}

void schedule_timed_task(Action act, unsigned int duration) {
    if(timed_tasks_len < MAX_TIMED_TASKS) {
        timed_tasks[timed_tasks_len] = (TimedFuture) {act, duration};
        timed_tasks_len += 1;
    }
}

void queue_sequential_task(Action act) {
    if(sequential_tasks_len < MAX_SEQ_TASKS) {
        sequential_tasks[sequential_tasks_len] = act;
        sequential_tasks_len += 1;
    }
}