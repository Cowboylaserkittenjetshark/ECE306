#include "include/serial.h"
#include <stdio.h>
#include "include/global.h"
#include <string.h>

char out_test[] = {'N', 'C', 'S', 'U', ' ', '#', '1', '\n'};

void init_serial_comms(char speed) {
    idt = 0;
    idw = 0;
    snprintf(display_line[2], 11, "   Baud   ");
    display_changed = 1;
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
            snprintf(display_line[3], 11, "  460800  ");
            break;
        case 's':
        default:
            UCA0BRW = 4;
            UCA0MCTLW = 0x5551;
            snprintf(display_line[3], 11, "  115200  ");
            break;
    }
    display_changed = 1;

    UCA0CTLW0 &= ~UCSWRST;
    UCA0TXBUF = 0x00;
    UCA0IE |= UCRXIE;
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
            snprintf(display_line[3], 11, "  460800  ");
            break;
        case 's':
        default:
            UCA1BRW = 4;
            UCA1MCTLW = 0x5551;
            snprintf(display_line[3], 11, "  115200  ");
            break;
    }
    display_changed = 1;
   
    UCA1CTLW0 &= ~UCSWRST;
    UCA1TXBUF = 0x00;
    UCA1IE |= UCRXIE;
}

// void uca0_transmit() {
//     pb_index = 0;
// }

#pragma vector=EUSCI_A0_VECTOR
__interrupt void uca0_interrupt() {
    switch(__even_in_range(UCA0IV, 0x08)) {
        case 0: break;
        case 2:
            UCA1TXBUF = UCA0RXBUF;
            // display_line[1][0] = UCA0RXBUF;
            // display_changed = 1;
            break;
        case 4: break;
        default: break;
    }
} 

#pragma  vector = EUSCI_A1_VECTOR
__interrupt void uca1_interrupt() {
    switch(__even_in_range(UCA1IV, 0x08)) {
        case 0: break;
        case 2:
            UCA0TXBUF = UCA1RXBUF;
            if(idw > 9) {
                idw = 0;
                strcpy(display_line[1], "          ");
            }
            // if(idw>8 || UCA1RXBUF == 0x0A) clear_next = 1;
            // if (clear_next) {
            //     idw = 0;
            //     strcpy(display_line[1], "          ");
            //     clear_next = 0;
            // }
            display_line[1][idw++] = UCA1RXBUF;
            
            display_changed = 1;
            break;
        case 4:
            if(idt > 6) idt = 0;
            UCA1TXBUF = out_test[idt++];
            break;
        default: break;
    }
}