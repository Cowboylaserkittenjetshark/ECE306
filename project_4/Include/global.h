#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdbool.h>

// Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
volatile unsigned int last_time_sequence;
volatile unsigned int cycle_time;
volatile bool time_change;
extern volatile char one_time;
unsigned int test_value;
char chosen_direction;
char change;

#endif
