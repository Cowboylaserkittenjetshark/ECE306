#ifndef MOTORS_H
#define MOTORS_H
#include <stdio.h>
#include <stdint.h>

typedef enum motor_dir { REV, OFF, FWD } MotorDir;
typedef enum motor_side { MOTOR_LEFT, MOTOR_RIGHT } MotorSide;

void motor_set(MotorSide side, uint32_t pct, MotorDir dir);
void motors_set(uint32_t pct, MotorDir dir);
void motor_forward(MotorSide side, uint32_t pct);
void motors_forward(uint32_t pct);
void motor_reverse(MotorSide side, uint32_t pct);
void motors_reverse(uint32_t pct);
void motor_off(MotorSide side);
void motors_off(void);

#endif
