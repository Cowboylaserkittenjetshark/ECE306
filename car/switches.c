#include "include/switches.h"
#include "include/global.h"
#include "include/ports.h"
#include "msp430.h"
#include <driverlib.h>
#include <stdbool.h>

// Switch actions
#include "include/display.h"
#include "include/comms.h"
#include "include/menu.h"

volatile bool sw1_pressed = false;
volatile bool sw2_pressed = false;
volatile bool sw1_idle = true;
volatile bool sw2_idle = true;
volatile unsigned int sw1_debounce_elapsed = 0;
volatile unsigned int sw2_debounce_elapsed = 0;

void switches_process(void) {
  sw1_process();
  sw2_process();
}

void sw1_process(void) {
  if (sw1_pressed) {
      sw1_pressed = false;
      menu_enter();
  }
}

void sw2_process(void) {
  if (sw2_pressed) {
    sw2_pressed = false;
    menu_back();
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
