#include "msp430.h"
#include "Include/ports.h"
#include "Include/motors.h"

void motors_forward(void) {
  P6OUT &= ~L_REVERSE;
  P6OUT &= ~R_REVERSE;

  P6OUT |= L_FORWARD;
  P6OUT |= R_FORWARD;
  
}

void motors_off(void) {
  P6OUT &= ~L_REVERSE;
  P6OUT &= ~R_REVERSE;

  P6OUT &= ~L_FORWARD;
  P6OUT &= ~R_FORWARD;  
}

void left_motor_off(void) {
  P6OUT &= ~L_REVERSE;
  P6OUT &= ~L_FORWARD;
}

void right_motor_off(void) {
  P6OUT &= ~R_REVERSE;
  P6OUT &= ~R_FORWARD;
}

void left_motor_forward(void) {
  P6OUT &= ~L_REVERSE;
  P6OUT |= L_FORWARD;
}

void right_motor_forward(void) {
  P6OUT &= ~R_REVERSE;
  P6OUT |= R_FORWARD;
}

