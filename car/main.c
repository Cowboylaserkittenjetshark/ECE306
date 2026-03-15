#include "include/display.h"
#include "include/global.h"
#include "include/clocks.h"
#include "include/init.h"
#include "include/led.h"
#include "include/motors.h"
#include "include/ports.h"
#include "include/switches.h"
#include "include/timers.h"
#include "include/adc.h"
#include "include/ir.h"
#include "msp430.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

void main(void) {
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  init_ports();      // Initialize Ports
  init_clocks();     // Initialize Clock System
  init_conditions(); // Initialize Variables and Initial Conditions
  init_timers();     // Initialize Timers
  Init_LCD();        // Initialize LCD
  init_adc();        // Initialize ADC
  init_ir();         // Initialize IR module
  
  // Begining of the "While" Operating System
  while (true) {
    if (Time_Sequence != last_time_sequence) {
      last_time_sequence = Time_Sequence;
      cycle_time += 1;
      display_thumb_status();
      display_detector_status();
      time_change = true;
    }
    line_process();
    switches_process();
    display_process();   // Update Display
    P3OUT ^= TEST_PROBE; // Change State of TEST_PROBE OFF
  }
}
