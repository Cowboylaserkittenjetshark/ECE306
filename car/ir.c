#include "include/ir.h"
#include "include/global.h"
#include "include/ports.h"

#include <stdio.h>

void init_ir() {
    ir_on = true;

    display_emitter_status();
    display_detector_status();
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
