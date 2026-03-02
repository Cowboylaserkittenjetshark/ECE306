#include "include/switches.h"
#include "include/global.h"
#include "include/ports.h"
#include "include/shapes.h"
#include "msp430.h"
#include <driverlib.h>
#include <stdbool.h>

volatile bool sw1_pressed = false;
volatile bool sw2_pressed = false;
volatile bool sw1_idle = true;
volatile bool sw2_idle = true;
volatile unsigned int sw1_debounce_elapsed = 0;
volatile unsigned int sw2_debounce_elapsed = 0;
volatile unsigned int sw1_press_count = 0;
volatile unsigned int sw2_press_count = 0;

void switches_process(void) {
  sw1_process();
  sw2_process();
}

void sw1_process(void) {
  if (sw1_pressed) {
      sw1_pressed = false;
      sw1_press_count += 1;
      if (sw1_press_count > 9) sw1_press_count = 0;
      display_line[0][0] = (char)(sw1_press_count + 48);
      display_changed = true;
      GPIO_toggleOutputOnPin(PORT6, LCD_BACKLITE);
  }
}

void sw2_process(void) {
  if (sw2_pressed) {
    sw2_pressed = false;
    sw2_press_count += 1;
    if (sw2_press_count > 9) sw2_press_count = 0;
    display_line[0][9] = (char)(sw2_press_count + 48);
    display_changed = true;
    GPIO_toggleOutputOnPin(PORT6, LCD_BACKLITE);
 }
}

// TODO Debounce
#pragma vector=PORT4_VECTOR
__interrupt void p4_interrupt(void) {
  if(P4IFG & SW1) {
    GPIO_clearInterrupt(PORT4, SW1);
    sw1_pressed = true;
  }
}

#pragma vector=PORT2_VECTOR
__interrupt void p2_interrupt(void) {
  if(P2IFG & SW2) {
    GPIO_clearInterrupt(PORT2, SW2);
    sw2_pressed = true;
  }
}
