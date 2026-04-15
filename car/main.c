#include "include/display.h"
#include "include/global.h"
#include "include/clocks.h"
#include "include/init.h"
#include "include/led.h"
#include "include/motors.h"
#include "include/pwm.h"
#include "include/ports.h"
#include "include/switches.h"
#include "include/timers.h"
#include "include/adc.h"
#include "include/ir.h"
#include "include/comms.h"
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
  init_serial_comms('s');

  // Begining of the "While" Operating System
  while (true) {
    if (Time_Sequence != last_time_sequence) {
      last_time_sequence = Time_Sequence;
      cycle_time += 1;
      time_change = true;
    }
    comms_process();
    switches_process();
    display_process();   // Update Display
    P3OUT ^= TEST_PROBE; // Change State of TEST_PROBE OFF
  }
}
