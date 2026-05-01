#ifndef LINE_H
#define LINE_H

#include <stdint.h>
#include <stdbool.h>

#define BLACK_LEVEL (730)
#define WHITE_THRESH (650)
#define PAUSE_TIME (5)
#define FOLLOW_SPEED (20)
#define LINE_LOST_SPEED (20)
#define FOLLOW_DURATION (25)
#define KP (1.9)
#define KD (0.1)

typedef enum last_direction { NODIR, LEFT, RIGHT } LastDirection;
typedef enum line_align_state { START, WAIT, SEARCH, PAUSE, ALIGN, END } LineProcessState;

volatile LineProcessState line_state;
volatile unsigned int pause_time_start; 
bool init_line_align_action(void);
bool line_align_action(void);
void set_line_align_state(LineProcessState new_state);
void display_line_align_state(void);

volatile int error;
volatile int last_error;
volatile int derivative;
volatile int correction;
volatile int left_wheel_pct;
volatile int right_wheel_pct;
volatile LastDirection last_dir;
volatile unsigned int line_follow_ticks_elapsed;

bool init_line_follow_action(void);
bool line_follow_action(void);

#endif
