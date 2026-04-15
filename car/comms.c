#include "include/comms.h"
#include <stdio.h>
#include "include/global.h"
#include <string.h>
#include <stdbool.h>

volatile bool uca1txie = false;

void comms_process(void) {
    if(cmd_ready) {
        switch (cmd) {
            case TEST:
                pc_log("I'm a list comprehension");
                break;
            case SET_BAUD_FAST:
                pc_log("Setting high baud");
                break;
            case SET_BAUD_SLOW:
                pc_log("Setting low baud");
                break;
            case DRIVE:
                pc_log("Driving");
                break;
            case TURN:
                pc_log("Turning");
                break;
            case UNKNOWN:
                pc_log("Unknown command");
                break;
            default: break;
        }
        cmd = NONE;
        cmd_ready = false;
    }
}

void init_serial_comms(char speed) {
    init_serial_uca0(speed);
    init_serial_uca1(speed);
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

    uca0_state = NOR;
    cmd = NONE;
    cmd_arg_id = 0;
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
    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG)) {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            switch (uca0_state) {
                case NOR:
                    if(UCA0RXBUF == '^') uca0_state = CMD;
                    else UCA1TXBUF = UCA0RXBUF;
                    break;
                case CMD:
                    switch (UCA0RXBUF) {
                        case '^':
                            iot_cmd_done(TEST);
                            break;
                        case 'F':
                            iot_cmd_done(SET_BAUD_FAST);
                            break;
                        case 'S':
                            iot_cmd_done(SET_BAUD_SLOW);
                            break;
                        case 'D':
                            cmd = DRIVE;
                            uca0_state = ARGS;
                            break;
                        case 'T':
                            cmd = TURN;
                            uca0_state = ARGS;
                            break;
                        default:
                            iot_cmd_done(UNKNOWN);
                            break;
                    }
                    break;
                case ARGS:
                    switch (UCA0RXBUF) {
                        case '\r':
                        case '\n':
                            iot_cmd_done(UNKNOWN);
                            break;
                        default:
                            switch (cmd) {
                                case DRIVE:
                                case TURN:
                                    if(cmd_arg_id >= 2) {
                                        cmd_arg_id = 0;
                                        uca0_state = END_CR;
                                    } else {
                                        cmd_args[cmd_arg_id] = UCA0RXBUF;
                                        cmd_arg_id += 1;
                                    }
                                    break;
                                default:
                                    iot_cmd_done(UNKNOWN);
                                    break;
                            }
                            break;
                    }
                    break;
                case END_CR:
                    if(UCA0RXBUF == '\r') uca0_state = END_LF;
                    break;
                case END_LF:
                    if(UCA0RXBUF == '\n') uca0_state = NOR;
                    break;
                default:
                    uca0_state = NOR;
                    break;
            }
            break;
        case USCI_UART_UCTXIFG:
            break;
        default: break;
    }
}

static inline void iot_cmd_done(const Command c) {
    cmd = c;
    cmd_arg_id = 0;
    uca0_state = END_CR;
    cmd_ready = true;
}

#pragma vector=EUSCI_A1_VECTOR
__interrupt void uca1_interrupt() {
    // switch(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG)) {
    //     case USCI_NONE: break;
    //     case USCI_UART_UCRXIFG:
    //         if(cmd_mode) {
    //             switch (UCA1RXBUF) {
    //                 case '^':
    //                     cmd = TEST;
    //                     break;
    //                 case 'F':
    //                     cmd = SET_BAUD_FAST;
    //                     break;
    //                 case 'S':
    //                     cmd = SET_BAUD_SLOW;
    //                     break;
    //                 default:
    //                     cmd = UNKNOWN;
    //                     break;
    //             }
    //             cmd_mode = false;
    //         } else {
    //             if(UCA1RXBUF == '^'){
    //                 cmd_mode = true;
    //                 display_line[2][0] = 'T';
    //                 display_changed = true;
    //             }
    //             else if(!pc_tx_blocked) UCA0TXBUF = UCA1RXBUF;
    //         }
    //         break;
    //     case USCI_UART_UCTXIFG:
    //         if(pc_tx_id < TX_BUFF_LEN && pc_tx_buff[pc_tx_id] != '\0') {
    //             UCA1TXBUF = pc_tx_buff[pc_tx_id];
    //             pc_tx_id += 1;
    //         } else {
    //             UCA1IE &= ~UCTXIE; // Disable transmit
    //             uca1txie = false;
    //             pc_tx_buff[0] = '\0';
    //             pc_tx_id = 0;
    //             pc_tx_blocked = false;
    //         }
    //         break;
    //     default: break;
    // }
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
                uca1txie = false;
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
    pc_tx_blocked = false;
    UCA1IE |= UCTXIE; // Enable transmit
    uca1txie = true;
}
