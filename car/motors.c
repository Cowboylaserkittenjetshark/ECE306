#include "include/motors.h"
#include "include/ports.h"
#include "include/global.h"
#include "msp430.h"
#include <driverlib.h>

#include "include/pwm.h"

volatile MotorDir left_motor_state = OFF;
volatile MotorDir right_motor_state = OFF;

void motor_off(MotorSide side) {
  switch (side) {
    case MOTOR_LEFT:
      LEFT_FORWARD_DUTY = PWM_DUTY_MIN;
      LEFT_REVERSE_DUTY = PWM_DUTY_MIN;
      left_motor_state = OFF;
      break;
    case MOTOR_RIGHT:
      RIGHT_FORWARD_DUTY = PWM_DUTY_MIN;
      RIGHT_REVERSE_DUTY = PWM_DUTY_MIN;
      right_motor_state = OFF;
      break;
    default: break;
  }
}

void motors_off() {
  motor_off(MOTOR_LEFT);
  motor_off(MOTOR_RIGHT);
}

void motor_forward(MotorSide side, uint32_t pct) {
  switch (side) {
    case MOTOR_LEFT:
      left_motor_pwm = pwm_pct(pct);
      switch (left_motor_state) {
        case FWD:
        case OFF:
          LEFT_FORWARD_DUTY = pwm_pct(pct);
          left_motor_state = FWD;
          break;
        case REV:
          LEFT_REVERSE_DUTY = PWM_DUTY_MIN;
          left_motor_state = OFF;
          break;
        default: break;
      }
      break;
    case MOTOR_RIGHT:
      right_motor_pwm = pwm_pct(pct);
      switch (right_motor_state) {
        case FWD:
        case OFF:
          RIGHT_FORWARD_DUTY = pwm_pct(pct);
          right_motor_state = FWD;
          break;
        case REV:
          RIGHT_REVERSE_DUTY = PWM_DUTY_MIN;
          right_motor_state = OFF;
          break;
        default: break;
      }
      break;
    default: break;
  }
}

void motors_forward(uint32_t pct) {
  motor_forward(MOTOR_LEFT, pct);
  motor_forward(MOTOR_RIGHT, pct);
}

void motor_reverse(MotorSide side, uint32_t pct) {
  switch (side) {
    case MOTOR_LEFT:
      switch (left_motor_state) {
        case REV:
        case OFF:
          LEFT_REVERSE_DUTY = pwm_pct(pct);
          left_motor_state = REV;
          break;
        case FWD:
          LEFT_FORWARD_DUTY = PWM_DUTY_MIN;
          left_motor_state = OFF;
          break;
        default: break;
      }
      break;
    case MOTOR_RIGHT:
      switch (right_motor_state) {
        case REV:
        case OFF:
          RIGHT_REVERSE_DUTY = pwm_pct(pct);
          right_motor_state = REV;
          break;
        case FWD:
          RIGHT_FORWARD_DUTY = PWM_DUTY_MIN;
          right_motor_state = OFF;
          break;
        default: break;
      }
      break;
    default: break;
  }
}

void motors_reverse(uint32_t pct) {
  motor_reverse(MOTOR_LEFT, pct);
  motor_reverse(MOTOR_RIGHT, pct);
}
 
void motor_set(MotorSide side, uint32_t pct, MotorDir dir) {
  switch (dir) {
    case FWD:
      motor_forward(side, pct);
      break;
    case REV:
      motor_reverse(side, pct);
      break;
    case OFF:
      motor_off(side);
      break;
    default: break;
  }
}

void motor_set_bidir(MotorSide side, uint32_t pct_bi) {
  MotorDir dir;
  uint32_t pct = (pct_bi * 200) / 255;
  if(pct > (100 + DEADZONE)) {
    dir = FWD;
    pct -= 100;
  }
  else if(pct < (100 - DEADZONE)) {
    dir = REV;
    pct = 100 - pct;
  } else {
    dir = OFF;
    pct = 0;
  }

  motor_set(side, pct, dir);
}

void motors_set(uint32_t pct, MotorDir dir) {
  motor_set(MOTOR_LEFT, pct, dir);
  motor_set(MOTOR_RIGHT, pct, dir);
}
