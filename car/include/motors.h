#ifndef MOTORS_H
#define MOTORS_H
#include <stdio.h>

typedef enum motor_state { REV, OFF, FWD } Motor_State;

void motors_forward(void);
void motors_reverse(void);
void motors_off(void);
void left_motor_forward(void);
void right_motor_forward(void);
void left_motor_reverse(void);
void right_motor_reverse(void);
void left_motor_off(void);
void right_motor_off(void);

#endif
