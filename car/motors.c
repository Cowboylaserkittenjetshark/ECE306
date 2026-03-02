#include "include/motors.h"
#include "include/ports.h"
#include "msp430.h"
#include <driverlib.h>

volatile Motor_State left_motor_state = OFF;
volatile Motor_State right_motor_state = OFF;

void motors_forward(void) {
  left_motor_forward();
  right_motor_forward();
}

void motors_reverse(void) {
  left_motor_reverse();
  right_motor_reverse();
}

void motors_off(void) {
  left_motor_off();
  right_motor_off();
}

void left_motor_off(void) {
  switch(left_motor_state) {
    case FWD:
      GPIO_setOutputLowOnPin(PORT6, L_FORWARD);
      left_motor_state = OFF;
      break;
    case REV:
      GPIO_setOutputLowOnPin(PORT6, L_REVERSE);
      left_motor_state = OFF;
      break;
    default: break;     
  }
}

void right_motor_off(void) {
  switch(right_motor_state) {
    case FWD:
      GPIO_setOutputLowOnPin(PORT6, R_FORWARD);
      right_motor_state = OFF;
      break;
    case REV:
      GPIO_setOutputLowOnPin(PORT6, R_REVERSE);
      right_motor_state = OFF;
      break;
    default: break;     
  }
}

void left_motor_forward(void) {
  switch(left_motor_state) {
    case OFF:
      GPIO_setOutputHighOnPin(PORT6, L_FORWARD);
      left_motor_state = FWD;
      break;
    case REV:
      GPIO_setOutputLowOnPin(PORT6, L_REVERSE);
      GPIO_setOutputHighOnPin(PORT6, L_FORWARD);
      left_motor_state = FWD;
      break;
    default: break;    
  }
}

void right_motor_forward(void) {
  switch(right_motor_state) {
    case OFF:
      GPIO_setOutputHighOnPin(PORT6, R_FORWARD);
      right_motor_state = FWD;
      break;
    case REV:
      GPIO_setOutputLowOnPin(PORT6, R_REVERSE);
      GPIO_setOutputHighOnPin(PORT6, R_FORWARD);
      right_motor_state = FWD;
      break;
    default: break;    
  }
}

void left_motor_reverse(void) {
  switch(left_motor_state) {
    case OFF:
      GPIO_setOutputHighOnPin(PORT6, L_REVERSE);
      left_motor_state = REV;
      break;
    case FWD:
      GPIO_setOutputLowOnPin(PORT6, L_FORWARD);
      GPIO_setOutputHighOnPin(PORT6, L_REVERSE);
      left_motor_state = REV;
      break;
    default: break;    
  }
}

void right_motor_reverse(void) {
  switch(right_motor_state) {
    case OFF:
      GPIO_setOutputHighOnPin(PORT6, R_REVERSE);
      right_motor_state = REV;
      break;
    case FWD:
      GPIO_setOutputLowOnPin(PORT6, R_FORWARD);
      GPIO_setOutputHighOnPin(PORT6, R_REVERSE);
      right_motor_state = REV;
      break;
    default: break;    
  }
}
