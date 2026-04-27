#ifndef IR_H
#define IR_H

#include <stdbool.h>

#define THUMB_MAX (4096)

volatile bool ir_on;
volatile unsigned long left_ir;
volatile unsigned long right_ir;

void init_ir(void);
void toggle_emitter(void);
void display_emitter_status(void);
void display_detector_status(void);
void display_line_state(void);

#endif
