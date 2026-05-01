#include "include/global.h"
#include "include/clocks.h"   // Provides init_clocks
#include "include/init.h"     // Provides init_conditions and init_leds
#include "include/display.h"  // Provices Init_Display and display_process
#include "include/ports.h"    // Provides init_ports and TEST_PROBE
#include "include/switches.h" // Provides switches_process
#include "include/timers.h" 
// Provides
//  - init_timers
//  - time_sequence
//  - last_time_sequence
//  - cycle_time
//  - time_change
//  - one_time
#include "include/adc.h"     // Provides init_adc
#include "include/ir.h"      // Provides init_ir
#include "include/thumb.h"   // Provides init_thumb
#include "include/line.h"    // Provides init_follow and follow_process
#include "include/comms.h"   // Provides init_comms and comms_process
#include "include/actions.h" // Provides init_scheduler and scheduler_process
#include "msp430.h"          // Provides register definitions

#include "include/motors.h"

void main(void) {
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  
   PM5CTL0 &= ~LOCKLPM5;

  init_ports();      // Initialize Ports
  init_clocks();     // Initialize Clock System
  init_conditions(); // Initialize Variables and Initial Conditions
  init_timers();     // Initialize Timers
  Init_LCD();        // Initialize LCD
  init_ir();
  init_thumb();
  init_adc();        // Initialize ADC
  init_scheduler();  // Initialize scheduler
  init_serial_comms('s');

  lcd_clear();
  // Begining of the "While" Operating System
  while (true) {
    if (time_sequence != last_time_sequence) {
      last_time_sequence = time_sequence;
      cycle_time += 1;
      time_change = true;
      timed_tasks_next_tick = true;
      line_follow_ticks_elapsed += 1;
      display_all();
    }

    comms_process();
    scheduler_process();
    switches_process();
    display_process();   // Update Display
    P3OUT ^= TEST_PROBE; // Change State of TEST_PROBE OFF
  }
}
