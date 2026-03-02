#include "include/display.h"
#include "include/global.h"
#include "include/clocks.h"
#include "include/init.h"
#include "include/led.h"
#include "include/motors.h"
#include "include/ports.h"
#include "include/shapes.h"
#include "include/switches.h"
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
  init_timers();     // Initialize Timers
  Init_LCD();        // Initialize LCD

  // Begining of the "While" Operating System
  while (true) {
    if (Time_Sequence != last_time_sequence) {
      last_time_sequence = Time_Sequence;
      cycle_time += 1;
      time_change = true;
    }
    if (time_change) {
      time_change = false;
      switch (cycle_time) {
        case 0:
          motors_off();
          strcpy(display_line[0], " WAIT     ");
          break;
        case 5:
          motors_forward();
          strcpy(display_line[0], " FORWARD  ");
          break;
        case 10:
          motors_off();
          strcpy(display_line[0], " PAUSE    ");
          break;
        case 15:
          motors_reverse();
          strcpy(display_line[0], " REVERSE  ");
          break; 
        case 25:
          motors_off();
          strcpy(display_line[0], " PAUSE    ");
          break;
        case 30:
          motors_forward();
          strcpy(display_line[0], " FORWARD  ");
          break;
        case 35:
          motors_off();
          strcpy(display_line[0], " PAUSE    ");
          break;
        case 40:
          left_motor_reverse();
          right_motor_forward();
          strcpy(display_line[0], " CW       ");
          break;
        case 55:
          motors_off();
          strcpy(display_line[0], " PAUSE    ");
          break;
        case 65:
          left_motor_forward();
          right_motor_reverse();
          strcpy(display_line[0], " CCW      ");
          break;
        case 80:
          motors_off();
          strcpy(display_line[0], " PAUSE    ");
          break;
        case 90:
          motors_off();
          cycle_time = 0;
          break;
        default: break;
      }
      display_changed = 1;
    }
    switches_process();
    display_process();   // Update Display
    P3OUT ^= TEST_PROBE; // Change State of TEST_PROBE OFF
  }
}
