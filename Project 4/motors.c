#include "include/motors.h"
#include "include/ports.h"
#include "msp430.h"

volatile Motor_State left_motor_state = OFF;
volatile Motor_State right_motor_state = OFF;

void motors_forward(void) {
  left_motor_forward();
  right_motor_forward();
}

void motors_off(void) {
  left_motor_off();
  right_motor_off();
}

void left_motor_off(void) {
  switch(left_motor_state) {
    case FWD:
      P6OUT &= ~L_FORWARD;
      left_motor_state = OFF;
      break;
    case REV:
      P6OUT &= ~L_REVERSE;
      left_motor_state = OFF;
      break;
    default: break;     
  }
}

void right_motor_off(void) {
  switch(right_motor_state) {
    case FWD:
      P6OUT &= ~R_FORWARD;
      right_motor_state = OFF;
      break;
    case REV:
      P6OUT &= ~R_REVERSE;
      right_motor_state = OFF;
      break;
    default: break;     
  }
}

void left_motor_forward(void) {
  switch(left_motor_state) {
    case OFF:
      P6OUT |= L_FORWARD;
      left_motor_state = FWD;
      break;
    case REV:
      P6OUT &= ~L_REVERSE;
      P6OUT |= L_FORWARD;
      left_motor_state = FWD;
      break;
    default: break;    
  }
}

void right_motor_forward(void) {
  switch(right_motor_state) {
    case OFF:
      P6OUT |= R_FORWARD;
      right_motor_state = FWD;
      break;
    case REV:
      P6OUT &= ~R_REVERSE;
      P6OUT |= R_FORWARD;
      right_motor_state = FWD;
      break;
    default: break;    
  }
}
