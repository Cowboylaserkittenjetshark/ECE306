#include "msp430.h"
#include "include/timers.h"
#include "include/global.h"

#include "include/pwm.h"
#include "include/dac.h"
#include "include/led.h"
#include <stdint.h>

void init_timers(void) {
    init_timer_b0();
    init_timer_b3();
}

void init_timer_b0(void) {
    time_sequence = 0;
    one_time = 0;
    on_time = 0;
    on_time_changed = false;
    TB0CTL = TBSSEL__SMCLK;
    TB0CTL |= TBCLR;
    TB0CTL |= MC__CONTINUOUS;
    TB0CTL |= ID__8;
    TB0EX0 |= TBIDEX__8;

    TB0CCR0 = TB0CCR0_INTERVAL;
    TB0CCTL0 |= CCIE;

    TB0CCR1 = TB0CCR1_INTERVAL;
    TB0CCTL1 |= CCIE;

    TB0CTL &= ~TBIE;
    TB0CTL &= ~TBIFG;
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void timer_b0_interrupt(void) {
    TB0CCR0 += TB0CCR0_INTERVAL;
    time_sequence += 1;
    if(time_sequence > 250) time_sequence = 0;
    update_display = 1;
    update_display_count += 1;
    one_time = 1;
    ADCCTL0 |= ADCSC;

    // switch (__even_in_range(TB0IV, 14)) {
    //     case 14:
    //         dac_adjust();
    //         break;
    //     default: break;
    // }
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void timer_b1_interrupt(void) {
    switch (__even_in_range(TB0IV, TB0IV_TBIFG)) {
        case TB0IV_NONE: break;
        case TB0IV_TB0CCR1:
            TB0CCR1 += TB0CCR1_INTERVAL;
            on_time += 1;
            if(on_time >= 2000) on_time = 0;
            on_time_changed = true;
            break; 
        default: break;
    }
}

void init_timer_b3(void) {
    TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR;

    PWM_PERIOD = INITIAL_PWM_PERIOD;

    TB3CCTL1 = OUTMOD_7;
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
