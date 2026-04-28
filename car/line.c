#include "include/line.h"
#include "include/global.h"

#include "include/motors.h"
#include "include/pwm.h"
#include "include/timers.h"
#include "include/ir.h"
#include "include/thumb.h"

void init_follow() {
    error = 0;
    last_error = 0;
    derivative = 0;
    correction = 0;
}

void follow_process() {
    error = left_ir - right_ir;
    derivative = error - last_error;
    if(left_ir < WHITE_THRESH && right_ir < WHITE_THRESH) {
        switch (last_dir) {
            case 1: // Left
                motor_reverse(MOTOR_LEFT, 20);
                motor_forward(MOTOR_RIGHT, 20);
                break;
            case 2: // Right
                motor_forward(MOTOR_LEFT, 20);
                motor_reverse(MOTOR_RIGHT, 20);
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
        
        if(error < 0) last_dir = 1;
        else last_dir = 2;
    }

    
    last_error = error;
}

bool line_process() {
    switch (line_state) {
        case START:
            motors_off();
            break;
        case WAIT:
            motors_off();
            if((Time_Sequence - pause_time_start) > PAUSE_TIME) set_line_state(SEARCH);
            break;
        case SEARCH:
            motors_forward(30);
            if (left_ir > BLACK_LEVEL && right_ir > BLACK_LEVEL) {
                motors_off();
                set_line_state(PAUSE);
            }
            break;
        case PAUSE:
            motors_off();
            if((Time_Sequence - pause_time_start) > PAUSE_TIME) set_line_state(ALIGN);
            break;
        case ALIGN:
            motor_reverse(MOTOR_LEFT, 20);
            motor_forward(MOTOR_RIGHT, 20);
            if(right_ir > BLACK_LEVEL) {
                motors_off();
                set_line_state(END);
            }
            break;
        case END:
            motors_off();
            return true;
            break;
    }
    return false;
}

void init_line() {
    pause_time_start = 0;
    line_state = WAIT;
}

void set_line_state(LineProcessState new_state) {
    switch(new_state) {
        case WAIT:
        case PAUSE:
            pause_time_start = Time_Sequence;
            break;
    }
    line_state = new_state;
    display_line_state();
}

void display_line_state() {
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
