#ifndef TIMERS_H
#define TIMERS_H

#include <stdbool.h>

#define TB0CCR0_INTERVAL (25000)

volatile unsigned int Time_Sequence;
volatile unsigned int last_time_sequence;
volatile unsigned int cycle_time;
volatile bool time_change;
volatile char one_time;

void init_timers(void);

void init_timer_b0(void);
__interrupt void timer_b0_interrupt(void);

void init_timer_b3(void);

#endif
