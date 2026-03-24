#include "include/pwm.h"

uint16_t pwm_pct(uint32_t pct) {
    return (pct * PWM_DUTY_MAX) / 100;
}
