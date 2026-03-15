#include "include/led.h"
#include "include/global.h"
#include "include/ports.h"

#include "include/display.h"
#include "include/init.h"
#include "include/timers.h"

#include "msp430.h"

void carlson_state_machine(void) {
  switch (Time_Sequence) {
  case 250:
    if (one_time) {
      init_leds();
      lcd_BIG_mid();
      display_changed = 1;
      one_time = 0;
    }
    Time_Sequence = 0;
    break;
  case 200:
    if (one_time) {
      P6OUT |= GRN_LED; // Change State of LED 5
      one_time = 0;
    }
    break;
  case 150:
    if (one_time) {
      P1OUT |= RED_LED;  // Change State of LED 4
      P6OUT &= ~GRN_LED; // Change State of LED 5
      one_time = 0;
    }
    break;
  case 100:
    if (one_time) {
      lcd_BIG_bot();
      P6OUT |= GRN_LED; // Change State of LED 5
      display_changed = 1;
      one_time = 0;
    }
    break;
  case 50:
    if (one_time) {
      one_time = 0;
    }
    break;
  default:
    break;
  }
}
