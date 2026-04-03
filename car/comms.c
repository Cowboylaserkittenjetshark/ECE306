#include "include/comms.h"
#include <stdio.h>
#include "include/global.h"
#include <string.h>
#include <stdbool.h>

void comms_process(void) {
    switch(uca0_state) {
        case RX:
            if(uca0_next_state) {
                uca0_next_state = false;
                strncpy(display_line[0], "Received  ", 11);
                strncpy(display_line[1], "          ", 11);
                snprintf(display_line[3], 11, "%s          ", cmdbuf);
                display_changed = true;
                uca0_state = IDLE; // Switch to IDLE state
            }
            break;
        case IDLE:
            if(uca0_next_state) {
                uca0_next_state = false;
                strncpy(display_line[0], "Transmit  ", 11);
                strncpy(display_line[3], "          ", 11);
                snprintf(display_line[1], 11, "%s          ", cmdbuf);
                display_changed = true;
                UCA0IE |= UCTXIE; // Enable transmit
                uca0_state = TX;  // Switch to TX state
            }
            break;
        case TX:
            if(uca0_next_state) {
                uca0_next_state = false;
                strncpy(display_line[0], "Waiting   ", 11);
                strncpy(display_line[1], "          ", 11);
                strncpy(display_line[3], "          ", 11);
                display_changed = true;
                UCA0TXBUF = '\n';
                UCA0IE |= UCRXIE; // Enable receive
                uca0_state = RX;  // Switch to RX state
            }
            break;
    }
}

void init_serial_comms(char speed) {
    init_serial_uca0(speed);
    // init_serial_uca1(speed);
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
            snprintf(display_line[2], 11, "  460800  ");
            break;
        case 's':
        default:
            UCA0BRW = 4;
            UCA0MCTLW = 0x5551;
            snprintf(display_line[2], 11, "  115200  ");
            break;
    }
    display_changed = 1;

    UCA0CTLW0 &= ~UCSWRST;
    UCA0TXBUF = 0x00;
    UCA0IE |= UCRXIE;

    uca0_state = RX;
    uca0_next_state = false;
    cmdid = 0;
}

// void init_serial_uca1(char speed) {
//     UCA1CTLW0 = 0;
//     UCA1CTLW0 |= UCSWRST;
//     UCA1CTLW0 |= UCSSEL__SMCLK;
//     UCA1CTLW0 &= ~UCMSB;
//     UCA1CTLW0 &= ~UCSPB;
//     UCA1CTLW0 &= ~UCPEN;
//     UCA1CTLW0 &= ~UCSYNC;
//     UCA1CTLW0 &= ~UC7BIT;
//     UCA1CTLW0 |= UCMODE_0;

//     switch (speed) {
//         case 'f':
//             UCA1BRW = 17;
//             UCA1MCTLW = 0x4A00;
//             snprintf(display_line[2], 11, "  460800  ");
//             break;
//         case 's':
//         default:
//             UCA1BRW = 4;
//             UCA1MCTLW = 0x5551;
//             snprintf(display_line[2], 11, "  115200  ");
//             break;
//     }
//     display_changed = 1;
   
//     UCA1CTLW0 &= ~UCSWRST;
//     UCA1TXBUF = 0x00;
//     UCA1IE |= UCRXIE;
// }

#pragma vector=EUSCI_A0_VECTOR
__interrupt void uca0_interrupt() {
    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG)) {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            if(cmdid == CMD_LENGTH || UCA0RXBUF == '\r') {
                uca0_next_state = true; // Receive done, move to next state
                UCA0IE &= ~UCRXIE;      // Disable receive
                cmdbuf[cmdid] = '\0';
                cmdid = 0;
            } else {
                cmdbuf[cmdid++] = UCA0RXBUF;
            }
            break;
        case USCI_UART_UCTXIFG:
            if(cmdid == CMD_LENGTH || cmdbuf[cmdid] == '\0') {
                uca0_next_state = true; // Transmit done, move to next state
                UCA0IE &= ~UCTXIE;      // Disable transmit
                cmdid = 0;
            } else {
                UCA0TXBUF = cmdbuf[cmdid++];
            }
            break;
        default: break;
    }
} 

// #pragma  vector = EUSCI_A1_VECTOR
// __interrupt void uca1_interrupt() {
//     switch(__even_in_range(UCA1IV, 0x08)) {
//         case 0: break;
//         case 2: break;
//         case 4: break;
//         default: break;
//     }
// }
