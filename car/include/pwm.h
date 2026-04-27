#ifndef PWM_H
#define PWM_H

#include "msp430.h"
#include <stdint.h>

#define INITIAL_PWM_PERIOD (50005)
#define PWM_DUTY_MAX       (INITIAL_PWM_PERIOD - 5)
#define PWM_DUTY_MIN       (0)
#define PWM_PCT_MAX        (100)
#define PWM_PCT_MIN        (10)

#define PWM_PERIOD         (TB3CCR0)
#define LCD_BACKLIGHT_DUTY (TB3CCR1)
#define RIGHT_FORWARD_DUTY (TB3CCR2)
#define LEFT_FORWARD_DUTY  (TB3CCR3)
#define RIGHT_REVERSE_DUTY (TB3CCR4)
#define LEFT_REVERSE_DUTY  (TB3CCR5)

uint16_t pwm_pct(uint32_t pct);

#endif
