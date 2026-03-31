#ifndef PWM_H
#define PWM_H

#include "msp430.h"

void init_serial_comms(char speed);
static inline void init_serial_uca0(char speed);
static inline void init_serial_uca1(char speed);

// void uca0_transmit(void);
// void uca1_transmit(void);

char echo_ring[25];
unsigned int idt;
unsigned int idr;
unsigned int idw;

#endif
