#include "msp430.h"
#include <string.h>
#include "Include/ports.h"
#include "Include/global.h"
#include "Include/clocks.h"
#include "Include/init.h"
#include "Include/timers.h"
#include "Include/display.h"
#include "Include/led.h"
#include "Include/switches.h"

#define ALWAYS (1)
#define TRUE (0x01)

void main(void){
  // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  Init_Ports();      // Initialize Ports
  Init_Clocks();     // Initialize Clock System
  Init_Conditions(); // Initialize Variables and Initial Conditions
  Init_Timers();     // Initialize Timers
  Init_LCD();        // Initialize LCD
  strcpy(display_line[0], "   NCSU   ");
  strcpy(display_line[1], " WOLFPACK ");
  strcpy(display_line[2], "  ECE306  ");
  strcpy(display_line[3], "  GP I/O  ");
  display_changed = TRUE;

  wheel_move = 0;
  forward = TRUE;
  P6OUT |= L_FORWARD;
  P6OUT |= R_FORWARD;
  // Begining of the "While" Operating System
  while(ALWAYS) {                      // Can the Operating system run
    Carlson_StateMachine();            // Run a Time Based State Machine
    Switches_Process();                // Check for switch state change
    Display_Process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
  }
}
