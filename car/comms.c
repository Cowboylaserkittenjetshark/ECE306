#include "include/comms.h"
#include <stdio.h>
#include "include/global.h"
#include "include/motors.h"
#include "include/actions.h"
#include "include/timers.h"
#include "include/display.h"
#include <string.h>
#include <stdbool.h>

const char iot_init_frame[IOT_INIT_FRAME_LEN][TX_BUFF_LEN] = { "AT+CIPSTAMAC=\"E0:51:D8:21:6A:E4\"", "AT+SYSSTORE=0", "AT+CIPMUX=1", "AT+CIPSERVER=1,3107", "AT+CIFSR" };

void comms_process(void) {
    if(cmd_ready) {
        char * currc = strchr(cmd_buff, '^');
        if(currc != NULL) handle_command(currc); // Got a ^{CMD} style command
        else { // Got IOT info
            char * ip = strstr(cmd_buff, "STAIP");
            if(ip != NULL) {
                ip += 7;
                int i;
                for(i = 0; i < 16; i += 1) {
                    if(*ip == '"') break;
                    else ip_addr[i] = *(ip++);
                }
                ip_changed = true;
            }
        }
        cmd_ready = false;
    }
}

static inline void handle_command(const char * currc) {
    currc += 1; // Throw away ^
    display_command(currc);
    switch (*(currc++)) {
        case '^':
            pc_log("TEST");
            break;
        case 'F':
            pc_log("FAST BAUD");
            break;
        case 'S':
            pc_log("SLOW BAUD");
            break;
        case 'C':
            if(!curvature_command(currc)) pc_log("Malformed curvature");
            break;
        case 'P':
            follow_command();
            break;
        case 'N':
            pad_command(currc);
            break;
        default:
            pc_log("Unknown");
            break;
    }
}

static inline bool curvature_command(const char * currc) {
    float fwd_pct = 0.0;
    float turn_pct = 0.0;

    if(is_float_start(*currc)) {
        fwd_pct = parse_float(&currc);
        if(*currc != ',') return false;
        else currc += 1;
        if(is_float_start(*currc)) {
            turn_pct = parse_float(&currc);
            motor_set_bidir(MOTOR_LEFT, fwd_pct - turn_pct);
            motor_set_bidir(MOTOR_RIGHT, fwd_pct + turn_pct);
        } else return false;
    } else return false;
    return true;
}

static inline void follow_command() {
    queue_sequential_task(INIT_LINE_ALIGN);
    queue_sequential_task(LINE_ALIGN);
    queue_sequential_task(INIT_LINE_FOLLOW);
    queue_sequential_task(LINE_FOLLOW);
}

static inline void pad_command(const char * currc) {
    pad_num = *currc;
    pad_changed = true;
}

void init_serial_comms(char speed) {
    init_serial_uca0(speed);
    init_serial_uca1(speed);
    
    pad_num = '\0';
    pad_changed = false;
    strcpy(ip_addr, "0.0.0.0");
    ip_changed = false;

    iot_init_frame_id = 0;
    unsigned int i;
    for(i = 1; i <= IOT_INIT_FRAME_LEN; i += 1) {
        schedule_timed_task(IOT_INIT, 10 * i);
    }
}

void init_serial_uca0(char speed) {
    UCA0CTLW0 = 0;
    UCA0CTLW0 |= UCSWRST;
    UCA0CTLW0 |= UCSSEL__SMCLK;
    UCA0CTLW0 &= ~UCMSB;
    UCA0CTLW0 &= ~UCSPB;
    UCA0CTLW0 &= ~UCPEN;
    UCA0CTLW0 &= ~UCSYNC;
    UCA0CTLW0 &= ~UC7BIT;
    UCA0CTLW0 |= UCMODE_0;

    switch (speed) {
        case 'f':
            UCA0BRW = 17;
            UCA0MCTLW = 0x4A00;
            snprintf(display_line[1], 11, "BR: 460800");
            break;
        case 's':
        default:
            UCA0BRW = 4;
            UCA0MCTLW = 0x5551;
            snprintf(display_line[1], 11, "BR: 115200");
            break;
    }
    display_changed = 1;

    UCA0CTLW0 &= ~UCSWRST;
    UCA0TXBUF = 0x00;
    UCA0IE |= UCRXIE;

    iot_tx_buff[0] = '\0';
    iot_tx_id = 0;
    
    uca0_state = NOR;
    cmd_buff[0] = '\0';
    cmd_buff_id = 0;
    cmd_ready = false; 
}

void init_serial_uca1(char speed) {
    UCA1CTLW0 = 0;
    UCA1CTLW0 |= UCSWRST;
    UCA1CTLW0 |= UCSSEL__SMCLK;
    UCA1CTLW0 &= ~UCMSB;
    UCA1CTLW0 &= ~UCSPB;
    UCA1CTLW0 &= ~UCPEN;
    UCA1CTLW0 &= ~UCSYNC;
    UCA1CTLW0 &= ~UC7BIT;
    UCA1CTLW0 |= UCMODE_0;

    switch (speed) {
        case 'f':
            UCA1BRW = 17;
            UCA1MCTLW = 0x4A00;
            snprintf(display_line[1], 11, "BR: 460800");
            break;
        case 's':
        default:
            UCA1BRW = 4;
            UCA1MCTLW = 0x5551;
            snprintf(display_line[1], 11, "BR: 115200");
            break;
    }
    display_changed = 1;
   
    UCA1CTLW0 &= ~UCSWRST;
    UCA1TXBUF = 0x00;
    UCA1IE |= UCRXIE;

    pc_tx_buff[0] = '\0';
    pc_tx_id = 0;
    pc_tx_blocked = false;
}

#pragma vector=EUSCI_A0_VECTOR
__interrupt void uca0_interrupt() {
    char iot_rx_char;
    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG)) {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            iot_rx_char = UCA0RXBUF;
            switch (uca0_state) {
                case NOR:
                    if(iot_rx_char == '+') {
                        uca0_state = CMD;
                        cmd_ready = false;
                        cmd_buff_id = 0;
                    }
                    else UCA1TXBUF = iot_rx_char;
                    break;
                case CMD:
                    switch (iot_rx_char) {
                        case '\r':
                            uca0_state = END_LF;
                            break;
                        case '\n':
                            uca0_state = NOR;
                            if(cmd_buff_id > 0) cmd_ready = true;
                            break;
                        default:
                            if(cmd_buff_id <= CMD_LEN) {
                                cmd_buff[cmd_buff_id] = iot_rx_char;
                                cmd_buff_id += 1;
                            }
                            break;
                    }
                    break;
                case END_CR:
                    if(UCA0RXBUF == '\r') uca0_state = END_LF;
                    break;
                case END_LF:
                    if(UCA0RXBUF == '\n') uca0_state = NOR;
                    if(cmd_buff_id > 0) cmd_ready = true;
                    break;
                default:
                    uca0_state = NOR;
                    cmd_buff_id = 0;
                    cmd_ready = false;
                    break;
            }
            break;
        case USCI_UART_UCTXIFG:
            if(iot_tx_id < TX_BUFF_LEN && iot_tx_buff[iot_tx_id] != '\0') {
                UCA0TXBUF = iot_tx_buff[iot_tx_id];
                iot_tx_id += 1;
            } else {
                UCA0IE &= ~UCTXIE; // Disable transmit
                iot_tx_buff[0] = '\0';
                iot_tx_id = 0;
            }
            break;
        default: break;
    }
}

#pragma vector=EUSCI_A1_VECTOR
__interrupt void uca1_interrupt() {
    switch(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG)) {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            if (!pc_tx_blocked) UCA0TXBUF = UCA1RXBUF;
        case USCI_UART_UCTXIFG:
            if(pc_tx_id < TX_BUFF_LEN && pc_tx_buff[pc_tx_id] != '\0') {
                UCA1TXBUF = pc_tx_buff[pc_tx_id];
                pc_tx_id += 1;
            } else {
                UCA1IE &= ~UCTXIE; // Disable transmit
                pc_tx_buff[0] = '\0';
                pc_tx_id = 0;
                pc_tx_blocked = false;
            }
            break;
        default: break;
    }
}

static inline void pc_log(const char * msg) {
    snprintf(pc_tx_buff, TX_BUFF_LEN - 1, "%s", msg);
    pc_tx_id = 0;
    pc_tx_blocked = true;
    UCA1IE |= UCTXIE; // Enable transmit
}

inline void iot_msg(const char * msg) {
    snprintf(iot_tx_buff, TX_BUFF_LEN - 1, "%s\r\n", msg);
    iot_tx_id = 0;
    UCA0TXBUF = 0x00;
    UCA0IE |= UCTXIE; // Enable transmit
}

static inline float parse_float(const char ** buffp) {
    const char * buff = *buffp;
    float sign        = 1.0f;
    float int_part    = 0.0f;
    float frac_part   = 0.0f;
    float frac_scale  = 1.0f;

    if(*buff == '-') {
        sign = -1.0f;
        buff++;
    }
    else if (*buff == '+') buff++;

    while ((*buff >= '0') && (*buff <= '9')) {
        int_part = int_part * 10.0f + (float)(*buff - '0');
        buff++;
    }

    if (*buff == '.') {
        buff++;
        while ((*buff >= '0') && (*buff <= '9')) {
            frac_scale *= 10.0f;
            frac_part   = frac_part * 10.0f + (float)(*buff - '0');
            buff++;
        }
        
        int_part += frac_part / frac_scale;
    }
    *buffp = buff;

    return sign * int_part;
}

static inline bool is_float_start(char c) {
    return (c == '-') || (c == '+') || ((c >= '0') && (c <= '9'));
}

void display_all() {
    if(cmd_buff[0] == '\0') {
        display_splash();
        display_ip(2);
    } else {
        display_pad();
        display_ip(1);
        display_timer();
    }
}

static inline void display_command(const char * currc) {
    snprintf(display_line[3], 6, "%s      ", currc);
    display_changed = true;
}

static inline void display_pad() {
    if(pad_changed) {
        if(pad_num != '\0') snprintf(display_line[0], 11, "Arrived 0%c", pad_num);
        else strcpy(display_line[0], "          ");
        display_changed = true;
        pad_changed = false;
    }
}

static inline void display_splash() {
    strcpy(display_line[0], " Waiting  ");
    strcpy(display_line[1], "for input ");
    display_changed = true;
}

static inline void display_ip(unsigned int line) {
    if(ip_changed) {
        strncpy(display_line[line], ip_addr, 10);
        strncpy(display_line[line + 1], ip_addr + 10, 10);
        display_changed = true;
        ip_changed = false;
    }
}

static inline void display_timer() {
    if(on_time_changed) {
        snprintf(display_line[3] + 6, 3, "%ds", on_time>>1);
        display_changed = true;
        on_time_changed = false;
    }
}

