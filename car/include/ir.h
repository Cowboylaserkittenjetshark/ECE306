#ifndef IR_H
#define IR_H

#include <stdbool.h>

#define BLACK_LEVEL (770)
#define PAUSE_TIME (5)
#define THUMB_MAX (4096)

typedef enum line_process_state { START, WAIT, SEARCH, PAUSE, ALIGN, END } LineProcessState;

volatile bool ir_on;
volatile unsigned int thumb;
volatile unsigned long left_ir;
volatile unsigned long right_ir;
volatile LineProcessState line_state;
volatile unsigned int pause_time_start; 
void line_process(void);

void init_ir(void);
void toggle_emitter(void);
void set_line_state(LineProcessState new_state);
void display_emitter_status(void);
void display_detector_status(void);
void display_thumb_status(void);
void display_line_state(void);

#endif
