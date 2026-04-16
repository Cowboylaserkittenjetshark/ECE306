#include "include/actions.h"
#include "include/motors.h"
#include "include/comms.h"

#include <stdbool.h>
#include <string.h>

void init_scheduler() {
    tasks_len = 0;
    next_tick = false;
}

void scheduler_process() {
    if(next_tick) {
        unsigned int tid;
        for (tid = 0; tid < tasks_len; tid += 1) {
            tasks[tid].ticks -= 1;
            if(tasks[tid].ticks == 0) {
                execute_action(tasks[tid].act);
                memmove(&tasks[tid], &tasks[tid + 1], (tasks_len - 1 - tid)*sizeof(Task));
                tasks_len -= 1;
            }
        }
        next_tick = false;
    }   
}

void execute_action(Action act) {
    switch (act) {
        case MOTORS_OFF:
            motors_off();
            break;
        case IOT_INIT:
            iot_msg(iot_init_frame[iot_init_frame_id]);
            iot_init_frame_id += 1;
            break;
        default: break;
    }
}

void schedule_task(Action act, unsigned int duration) {
    if(tasks_len < MAX_TASKS) {
        tasks[tasks_len] =  (Task) {act, duration};
        tasks_len += 1;
    }
}
