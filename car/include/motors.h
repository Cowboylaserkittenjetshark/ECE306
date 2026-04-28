#ifndef MOTORS_H
#define MOTORS_H
#include <stdio.h>
#include <stdint.h>
#include <stdint.h>

#define DEADZONE (10)

typedef enum motor_dir { REV = 'R', OFF = 'O', FWD = 'F' } MotorDir;
typedef enum motor_side { MOTOR_LEFT, MOTOR_RIGHT } MotorSide;

volatile uint16_t left_motor_pwm;
volatile uint16_t right_motor_pwm;

void motor_set(MotorSide side, uint32_t pct, MotorDir dir);
void motor_set_bidir(MotorSide side, float pct_bi);
void motors_set(uint32_t pct, MotorDir dir);
void motor_forward(MotorSide side, uint32_t pct);
void motors_forward(uint32_t pct);
void motor_reverse(MotorSide side, uint32_t pct);
void motors_reverse(uint32_t pct);
void motor_off(MotorSide side);
void motors_off(void);

#endif
