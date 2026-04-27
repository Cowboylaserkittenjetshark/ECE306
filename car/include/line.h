#ifndef LINE_H
#define LINE_H

#include <stdint.h>
#include <stdbool.h>

#define BLACK_LEVEL (730)
#define WHITE_THRESH (650)
#define PAUSE_TIME (5)
#define FOLLOW_SPEED (20)
#define KP (3)
#define KD (0)

typedef enum line_process_state { START, WAIT, SEARCH, PAUSE, ALIGN, END } LineProcessState;

volatile LineProcessState line_state;
volatile unsigned int pause_time_start; 
void init_line(void);
bool line_process(void);
void set_line_state(LineProcessState new_state);
void display_line_state(void);

volatile int error;
volatile int last_error;
volatile int derivative;
volatile int correction;
volatile int left_wheel_pct;
volatile int right_wheel_pct;
volatile int last_dir;

void init_follow(void);
void follow_process(void);

#endif
