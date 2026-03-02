#ifndef SWITCHES_H
#define SWITCHES_H

#include <stdbool.h>
#include "msp430.h"

#define DEBOUNCE_TIME (5)

void switches_process(void);
void sw1_process(void);
void sw2_process(void);

__interrupt void p4_interrupt(void);
__interrupt void p2_interrupt(void);

#endif
