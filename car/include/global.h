#ifndef GLOBAL_H
#define GLOBAL_H

// Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
unsigned int test_value;
char chosen_direction;
char change;

#endif
