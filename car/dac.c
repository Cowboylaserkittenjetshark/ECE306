#include "include/dac.h"
#include "msp430.h"

#include "include/led.h"

void init_dac() {
    SAC3DAC = DACSREF_0;
    SAC3DAC |= DACLSEL_0;

    SAC3OA = NMUXEN;
    SAC3OA |= PMUXEN;
    SAC3OA |= PSEL_1;
    SAC3OA |= NSEL_1;
    SAC3OA |= OAPM;

    SAC3PGA = MSEL_1;
    SAC3OA |= SACEN;
    SAC3OA |= OAEN;
    dac_data = DAC_INIT;
    SAC3DAT = dac_data;

    TB0CTL |= TBIE;
    red_led_on();
    SAC3DAC |= DACEN;
}

inline void dac_adjust() {
    dac_data -= 100;
    SAC3DAT = dac_data;
    if(dac_data < DAC_LIMIT) {
        dac_data = DAC_ADJUST;
        SAC3DAT = dac_data;
        TB0CTL &= ~TBIE;
        red_led_off();
    }
}