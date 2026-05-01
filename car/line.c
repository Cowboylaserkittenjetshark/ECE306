#include "include/line.h"
#include "include/global.h"

#include "include/motors.h"
#include "include/pwm.h"
#include "include/timers.h"
#include "include/ir.h"
#include "include/thumb.h"

#include <string.h>

bool init_line_follow_action() {
    last_dir = NODIR;
    line_follow_ticks_elapsed = 0;
    error = 0;
    last_error = 0;
    derivative = 0;
    correction = 0;
    return true;
}

bool line_follow_action() {
    if(line_follow_ticks_elapsed > 25) return true;
    error = left_ir - right_ir;
    derivative = error - last_error;
    if(left_ir < WHITE_THRESH && right_ir < WHITE_THRESH) {
        switch (last_dir) {
            case LEFT:
                motor_reverse(MOTOR_LEFT, LINE_LOST_SPEED);
                motor_forward(MOTOR_RIGHT, LINE_LOST_SPEED);
                break;
            case RIGHT:
                motor_forward(MOTOR_LEFT, LINE_LOST_SPEED);
                motor_reverse(MOTOR_RIGHT, LINE_LOST_SPEED);
                break;
            default:
                break;
        }
    } else {
        correction = (((error * KP) + (derivative * KD)) * 100) / 1024;
        left_wheel_pct = FOLLOW_SPEED - correction;
        if(left_wheel_pct < 0) motor_off(MOTOR_LEFT);
        else motor_set_bidir(MOTOR_LEFT, left_wheel_pct);
        right_wheel_pct = FOLLOW_SPEED + correction;
        if(right_wheel_pct < 0) motor_off(MOTOR_RIGHT);
        else motor_set_bidir(MOTOR_RIGHT, right_wheel_pct);
        
        if(error < 0) last_dir = LEFT;
        else last_dir = RIGHT;
    }
    
    last_error = error;
    return false;
}

bool init_line_align_action() {
    pause_time_start = 0;
    line_state = WAIT;
    return true;
}

bool line_align_action() {
    switch (line_state) {
        case START:
            motors_off();
            break;
        case WAIT:
            motors_off();
            if((time_sequence - pause_time_start) > PAUSE_TIME) set_line_align_state(SEARCH);
            break;
        case SEARCH:
            motors_forward(30);
            if (left_ir > BLACK_LEVEL && right_ir > BLACK_LEVEL) {
                motors_off();
                set_line_align_state(PAUSE);
            }
            break;
        case PAUSE:
            motors_off();
            if((time_sequence - pause_time_start) > PAUSE_TIME) set_line_align_state(ALIGN);
            break;
        case ALIGN:
            motor_reverse(MOTOR_LEFT, 20);
            motor_forward(MOTOR_RIGHT, 20);
            if(right_ir > BLACK_LEVEL) {
                motors_off();
                set_line_align_state(END);
            }
            break;
        case END:
            motors_off();
            return true;
    }
    return false;
}

void set_line_align_state(LineProcessState new_state) {
    switch(new_state) {
        case WAIT:
        case PAUSE:
            pause_time_start = time_sequence;
            break;
    }
    line_state = new_state;
    display_line_align_state();
}

void display_line_align_state() {
    switch(line_state) {
        case START:
            strcpy(display_line[2], " START    ");
            break;
        case WAIT:
            strcpy(display_line[2], " WAIT     ");
            break;
        case SEARCH:
            strcpy(display_line[2], " SEARCH   ");
            break;
        case PAUSE:
            strcpy(display_line[2], " PAUSE    ");
            break;
        case ALIGN:
            strcpy(display_line[2], " ALIGN    ");
            break;
        case END:
            strcpy(display_line[2], " END      ");
            break;
    }
    strcpy(display_line[3], "SWITCH    ");
    display_changed = true;
}
