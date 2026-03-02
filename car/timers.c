#include "msp430.h"
#include "include/timers.h"
#include "include/global.h"

void init_timers(void) {
    init_timer_b0();
}

void init_timer_b0(void) {
    Time_Sequence = 0;
    one_time = 0;
    TB0CTL = TBSSEL__SMCLK;
    TB0CTL |= TBCLR;
    TB0CTL |= MC__CONTINUOUS;
    TB0CTL |= ID__8;
    TB0EX0 |= TBIDEX__8;
    TB0CCR0 = TB0CCR0_INTERVAL;
    TB0CCTL0 |= CCIE;
    TB0CTL &= ~TBIE;
    TB0CTL &= ~TBIFG;
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void timer_b0_interrupt(void) {
    TB0CCR0 +=  TB0CCR0_INTERVAL;
    Time_Sequence += 1;
    if(Time_Sequence > 250) Time_Sequence = 0;
    update_display = 1;
    update_display_count += 1;
    one_time = 1;
}
