#include <driverlib.h>
#include "include/adc.h"
#include "include/ports.h"

// Peripherals
#include "include/ir.h"
#include "include/thumb.h"

void init_adc(void) {
  current_channel = 2;
  vbat = 0;
  
  ADCCTL0 = 0;
  ADCCTL0 |= ADCSHT;
  ADCCTL0 |= ADCMSC;
  ADCCTL0 |= ADCON;

  ADCCTL1 = 0;
  ADCCTL1 |= ADCSHS_0;
  ADCCTL1 |= ADCSHP;
  ADCCTL1 &= ~ADCISSH;
  ADCCTL1 |= ADCDIV_0;
  ADCCTL1 |= ADCSSEL_0;
  ADCCTL1 |= ADCCONSEQ_0;

  ADCCTL2 = 0;
  ADCCTL2 |= ADCPDIV0;
  ADCCTL2 |= ADCRES_2; // 10-bit mode
  ADCCTL2 &= ~ADCDF;
  ADCCTL2 &= ~ADCSR;

  ADCMCTL0 |= ADCSREF_0;
  ADCMCTL0 |= ADCINCH_2;

  ADCIE |= ADCIE0;
  ADCCTL0 |= ADCENC;
  ADCCTL0 |= ADCSC;
}

#pragma vector=ADC_VECTOR
__interrupt void adc_interrupt(void) {
  switch(__even_in_range(ADCIV, ADCIV_ADCIFG)) {
    case ADCIV_ADCIFG:
      ADCCTL0 &= ~ADCENC;
      switch(current_channel) {
        case 2:
          left_ir = ADCMEM0;
          left_ir = left_ir >> 2;
          ADCMCTL0 &= ~ADCINCH_2;
          ADCMCTL0 |= ADCINCH_3;
          current_channel = 3;
          ADCCTL0 |= ADCSC;
          break;
        case 3:
          right_ir = ADCMEM0;
          right_ir = right_ir >> 2;
          ADCMCTL0 &= ~ADCINCH_3;
          ADCMCTL0 |= ADCINCH_5;
          current_channel = 5;
          ADCCTL0 |= ADCSC;
          break;
        case 5:
          thumb = ADCMEM0;
          ADCMCTL0 &= ~ADCINCH_5;
          ADCMCTL0 |= ADCINCH_2;
          current_channel = 2;
          break;
      }
      ADCCTL0 |= ADCENC;
      break;
    default: break;
  }
}
