#ifndef COMMS_H
#define COMMS_H

#include "msp430.h"
#include <stdbool.h>

#define TX_BUFF_LEN (64)
#define MAX_ARGS (2)
#define CMD_LEN (3)
#define IOT_INIT_FRAME_LEN (4)

typedef enum command { NONE, UNKNOWN, TEST, DRIVE, TURN, SET_BAUD_FAST, SET_BAUD_SLOW } Command;
typedef enum comms_state { NOR, CMD, ARGS, END_CR, END_LF } CommsState;

extern const char iot_init_frame[IOT_INIT_FRAME_LEN][TX_BUFF_LEN];
volatile unsigned int iot_init_frame_id;
void init_serial_comms(char speed);
static inline void init_serial_uca0(char speed);
static inline void init_serial_uca1(char speed);
void comms_process(void);
static inline void pc_log(const char * msg);
inline void iot_msg(const char * msg);

bool volatile pc_tx_blocked;
static volatile char pc_tx_buff[TX_BUFF_LEN + 1];
static volatile unsigned int pc_tx_id;
static volatile char iot_tx_buff[TX_BUFF_LEN + 1];
static volatile unsigned int iot_tx_id;

static volatile CommsState uca0_state;
static volatile bool cmd_ready;
static volatile char cmd_buff[CMD_LEN + 1];
static volatile unsigned int cmd_buff_id;

#endif
