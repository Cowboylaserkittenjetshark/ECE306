#include "msp430.h"
#include "include/timers.h"
#include "include/global.h"

#include "include/pwm.h"
#include <stdint.h>
volatile uint16_t lcd_dbg = 0;

void init_timers(void) {
    init_timer_b0();
    init_timer_b3();
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

void init_timer_b3(void) {
    TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR;

    PWM_PERIOD = INITIAL_PWM_PERIOD;

    TB3CCTL1 = OUTMOD_7;
    lcd_dbg = pwm_pct(80);
    LCD_BACKLIGHT_DUTY = pwm_pct(80);

    TB3CCTL2 = OUTMOD_7;
    RIGHT_FORWARD_DUTY = 0;

    TB3CCTL3 = OUTMOD_7;
    LEFT_FORWARD_DUTY = 0;

    TB3CCTL4 = OUTMOD_7;
    RIGHT_REVERSE_DUTY = 0;

    TB3CCTL5 = OUTMOD_7;
    LEFT_REVERSE_DUTY = 0;
}
