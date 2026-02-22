#include "include/display.h"
#include "include/global.h"
#include "include/clocks.h"
#include "include/init.h"
#include "include/led.h"
#include "include/motors.h"
#include "include/ports.h"
#include "include/shapes.h"
#include "include/switches.h"
#include "include/switches_addons.h"
#include "include/timers.h"
#include "msp430.h"
#include <stdbool.h>
#include <string.h>

void main(void) {
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  init_ports();      // Initialize Ports
  init_clocks();     // Initialize Clock System
  init_conditions(); // Initialize Variables and Initial Conditions
  Init_Timers();     // Initialize Timers
  Init_LCD();        // Initialize LCD
  strcpy(display_line[0], "   NCSU   ");
  strcpy(display_line[1], " WOLFPACK ");
  strcpy(display_line[2], "  ECE306  ");
  strcpy(display_line[3], "  GP I/O  ");
  display_changed = true;

  // Begining of the "While" Operating System
  while (true) {
    if (Time_Sequence != last_time_sequence) {
      last_time_sequence = Time_Sequence;
      cycle_time += 1;
      time_change = true;
    }
    shape_state_machine();
    switches_process();
    display_process();   // Update Display
    P3OUT ^= TEST_PROBE; // Change State of TEST_PROBE OFF
  }
}
