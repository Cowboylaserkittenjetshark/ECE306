#ifndef COMMS_H
#define COMMS_H

#define CMD_LENGTH (10)

#include "msp430.h"
#include <stdbool.h>

typedef enum comms_state { RX, IDLE, TX } CommsState;

void init_serial_comms(char speed);
static inline void init_serial_uca0(char speed);
static inline void init_serial_uca1(char speed);
void comms_process(void);

CommsState uca0_state;
bool uca0_next_state;
char cmdbuf[CMD_LENGTH + 1];
unsigned int cmdid;

#endif
