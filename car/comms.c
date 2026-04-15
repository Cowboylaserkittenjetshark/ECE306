#include "include/comms.h"
#include <stdio.h>
#include "include/global.h"
#include <string.h>
#include <stdbool.h>

void comms_process(void) {
    if(cmd_ready) {
        switch (cmd_buff[0]) {
            case '^':
                pc_log("TEST");
                break;
            case 'F':
                pc_log("FAST BAUD");
                break;
            case 'S':
                pc_log("SLOW BAUD");
                break;
            case 'D':
                if(cmd_buff_id < 3) {
                    pc_log("Command too short");
                } else {
                    snprintf(pc_tx_buff, TX_BUFF_LEN - 1, "Driving %c at %c0", cmd_buff[1], cmd_buff[2]);
                    pc_tx_id = 0;
                    pc_tx_blocked = true;
                    UCA1IE |= UCTXIE; // Enable transmit
                }
                break;
            case 'T':
                if(cmd_buff_id < 3) {
                    pc_log("Command too short");
                } else {
                    snprintf(pc_tx_buff, TX_BUFF_LEN - 1, "Turning %c at %c0", cmd_buff[1], cmd_buff[2]);
                    pc_tx_id = 0;
                    pc_tx_blocked = true;
                    UCA1IE |= UCTXIE; // Enable transmit
                }
                break;
            default:
                pc_log("Unknown");
                break;
        }
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
                    if(iot_rx_char == '^') {
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
            break;
        default: break;
    }
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
