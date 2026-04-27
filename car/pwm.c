#include "include/pwm.h"

uint16_t pwm_pct(uint32_t pct) {
    if(pct < PWM_PCT_MIN) pct = PWM_PCT_MIN;
    else if(pct > PWM_PCT_MAX) pct = PWM_PCT_MAX;
    return (pct * PWM_DUTY_MAX) / 100;
}
