#ifndef COMMS_H
#define COMMS_H

#include "msp430.h"
#include <stdbool.h>

#define TX_BUFF_LEN (64)
#define MAX_ARGS (2)
#define CMD_LEN (64)
#define IOT_INIT_FRAME_LEN (5)

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
static inline bool is_float_start(char c);
static inline float parse_float(const char ** buffp);

static inline void handle_command(const char * currc);
static inline void drive_command(const char * currc);
static inline void turn_command(const char * currc);
static inline bool curvature_command(const char * currc);
static inline void follow_command(void);
static inline void pad_command(const char * currc);

void display_all(void);
static inline void display_command(const char * currc);
static inline void display_pad(void);
static inline void display_splash(void);
static inline void display_ip(unsigned int line);
static inline void display_timer(void);

bool volatile pc_tx_blocked;
static volatile char pc_tx_buff[TX_BUFF_LEN + 1];
static volatile unsigned int pc_tx_id;
static volatile char iot_tx_buff[TX_BUFF_LEN + 1];
static volatile unsigned int iot_tx_id;

static volatile CommsState uca0_state;
static volatile bool cmd_ready;
static volatile char cmd_buff[CMD_LEN + 1];
static volatile unsigned int cmd_buff_id;

static volatile char pad_num;
static volatile bool pad_changed;
static volatile char ip_addr[16];
static volatile bool ip_changed;

#endif
