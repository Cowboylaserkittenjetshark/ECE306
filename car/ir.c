#include "include/ir.h"
#include "include/global.h"
#include "include/ports.h"

#include "include/timers.h"
#include "include/motors.h"
#include "include/pwm.h"

#include <stdio.h>
#include <string.h>

void line_process() {
    switch (line_state) {
        case START:
            motors_off();
            break;
        case WAIT:
            motors_off();
            if((Time_Sequence - pause_time_start) > PAUSE_TIME) set_line_state(SEARCH);
            break;
        case SEARCH:
            motors_forward(PWM_DUTY_MAX);
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
            motor_reverse(MOTOR_LEFT, PWM_DUTY_MAX);
            motor_forward(MOTOR_RIGHT, PWM_DUTY_MAX);
            if(left_ir > BLACK_LEVEL) {
                motors_off();
                if(right_ir < left_ir) set_line_state(END);
            }
            break;
        case END:
            motors_off();
            break;
    }
}

void init_ir() {
    ir_on = false;
    left_ir = 0;
    right_ir = 0;
    thumb = 0;
    pause_time_start = 0;
    line_state = START;
    
    display_emitter_status();
    display_detector_status();
    display_thumb_status();
    display_line_state();
}

void toggle_emitter() {
    ir_on = !ir_on;
    GPIO_toggleOutputOnPin(PORT2, IR_LED);
    display_emitter_status();
}

void display_emitter_status() {
    snprintf(display_line[1], 5, "IR:%d ", ir_on);
    display_line[1][4] = ' ';
    display_changed = true;
}

void display_detector_status() {
    snprintf(display_line[0], 10, "L:%2lu R:%2lu ", (left_ir * 100) / 1025, (right_ir * 100) / 1025);
    display_line[0][9] = ' ';
    display_changed = true;
}

void display_thumb_status() {
    snprintf(display_line[1] + 5, 5, "T:%d ", (thumb * 10) / 4095);
    display_line[1][9] = ' ';
    display_changed = true;
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
