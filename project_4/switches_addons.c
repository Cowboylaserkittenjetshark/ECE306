#include "Include/switches_addons.h"

#include "msp430.h"
#include <stdbool.h>
#include "Include/ports.h"
#include "Include/global.h"
#include "Include/shapes.h"

volatile bool sw1_pressed = false;
volatile bool sw2_pressed = false;
volatile bool sw1_idle = true;
volatile bool sw2_idle = true;
volatile unsigned int sw1_debounce_elapsed = 0;
volatile unsigned int sw2_debounce_elapsed = 0;
volatile unsigned int button_press_count = 0;

void switches_process(void) {
  sw1_process();
  sw2_process();
}

void sw1_process(void) {
  if(sw1_idle && !sw1_pressed) {
    if (!(P4IN & SW1)) {
      sw1_pressed = true;
      sw1_idle = false;
      sw1_debounce_elapsed = 0;
      prev_shape();
    }
  }

  if(sw1_debounce_elapsed <= DEBOUNCE_TIME) {
    sw1_debounce_elapsed += 1;
  } else {
    sw1_idle = true;
    if(P4IN & SW1) sw1_pressed = false;
  }
}

void sw2_process(void) {
  if(sw2_idle && !sw2_pressed) {
    if (!(P2IN & SW2)) {
      sw2_pressed = true;
      sw2_idle = false;
      sw2_debounce_elapsed = 0;
      next_shape();
    }
  }

  if(sw2_debounce_elapsed <= DEBOUNCE_TIME) {
    sw2_debounce_elapsed += 1;
  } else {
    sw2_idle = true;
    if(P2IN & SW2) sw2_pressed = false;
  }
}
