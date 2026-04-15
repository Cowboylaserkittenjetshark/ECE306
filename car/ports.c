#include "include/ports.h"
#include "msp430.h"
#include <driverlib.h>

void init_ports(void) {
  init_port_1();
  init_port_2();
  init_port_3();
  init_port_4();
  init_port_5();
  init_port_6();
}

void init_port_1(void) {
  P1OUT = 0x00; // P1 set Low
  P1DIR = 0x00; // Set P1 direction to output

  P1SEL0 &= ~RED_LED;
  P1SEL1 &= ~RED_LED;
  P1OUT |= RED_LED;
  P1DIR |= RED_LED;

  // ADC inputs
  P1SELC |= A1_SEEED;
  P1SELC |= V_DETECT_L;
  P1SELC |= V_DETECT_R;
  P1SELC |= A4_SEEED;
  P1SELC |= V_THUMB;
  
  P1SEL0 |= UCA0RXD;
  P1SEL1 &= ~UCA0RXD;
  P1OUT &= ~UCA0RXD;
  P1DIR &= ~UCA0RXD;

  P1SEL0 |= UCA0TXD;
  P1SEL1 &= ~UCA0TXD;
  P1OUT &= ~UCA0TXD;
  P1DIR &= ~UCA0TXD;
}

// Configure Port 2
void init_port_2(void) {
  P2OUT = 0x00; // P2 set Low
  P2DIR = 0x00; // Set P2 direction to output

  // Configure SLOW_CLK as default low GPIO
  GPIO_setAsOutputPin(PORT2, SLOW_CLK);
  GPIO_setOutputLowOnPin(PORT2, SLOW_CLK);

  // Configure CHECK_BAT as default low GPIO
  GPIO_setAsOutputPin(PORT2, CHECK_BAT);
  GPIO_setOutputLowOnPin(PORT2, CHECK_BAT);

  // Configure IR_LED as default low GPIO
  GPIO_setAsOutputPin(PORT2, IR_LED);
  GPIO_setOutputLowOnPin(PORT2, IR_LED);

  // Configure switch 2 for interrupts
  GPIO_setAsInputPinWithPullUpResistor(PORT2, SW2);
  GPIO_selectInterruptEdge(PORT2, SW2, GPIO_HIGH_TO_LOW_TRANSITION);
  GPIO_clearInterrupt(PORT2, SW2);
  GPIO_enableInterrupt(PORT2, SW2);

  // Configure IOT_RUN_RED as default low GPIO
  GPIO_setAsOutputPin(PORT2, IOT_RUN_RED);
  GPIO_setOutputHighOnPin(PORT2, IOT_RUN_RED);

  // Configure DAC_ENB as default high GPIO
  GPIO_setAsOutputPin(PORT2, DAC_ENB);
  GPIO_setOutputHighOnPin(PORT2, DAC_ENB);

  P2SEL0 &= ~LFXOUT; // LFXOUT Clock operation
  P2SEL1 |= LFXOUT;  // LFXOUT Clock operation

  P2SEL0 &= ~LFXIN; // LFXIN Clock operation
  P2SEL1 |= LFXIN;  // LFXIN Clock operation
}

// Configure port 3
void init_port_3(void) {
  P3OUT = 0x00; // P3 set Low
  P3DIR = 0x00; // Set P3 direction to output

  P3SEL0 &= ~TEST_PROBE;
  P3SEL1 &= ~TEST_PROBE;
  P3OUT &= ~TEST_PROBE;
  P3DIR &= ~TEST_PROBE;

  P3SEL0 |= OA2O;
  P3SEL1 |= OA2O;
  P3OUT &= ~OA2O;
  P3DIR |= OA2O;

  P3SEL0 |= OA2N;
  P3SEL1 |= OA2N;
  P3OUT &= ~OA2N;
  P3DIR &= ~OA2N;

  P3SEL0 |= OA2P;
  P3SEL1 |= OA2P;
  P3OUT &= ~OA2P;
  P3DIR &= ~OA2P;

  P3SEL0 &= ~SMCLK;
  P3SEL1 &= ~SMCLK;
  P3OUT &= ~SMCLK;
  P3DIR &= ~SMCLK;

  P3SELC |= DAC_CNTL;

  P3SEL0 &= ~IOT_LINK_GRN;
  P3SEL1 &= ~IOT_LINK_GRN;
  P3OUT &= ~IOT_LINK_GRN;
  P3DIR &= ~IOT_LINK_GRN;

  P3SEL0 &= ~IOT_EN;
  P3SEL1 &= ~IOT_EN;
  P3OUT &= ~IOT_EN;
  P3DIR |= IOT_EN;
  __delay_cycles(100);
  P3OUT |= IOT_EN;
}

// Configure port 4
void init_port_4(void) {
  P4OUT = 0x00; // P4 set Low
  P4DIR = 0x00; // Set P4 direction to output

  // Configure RESET_LCD as default low GPIO output
  GPIO_setAsOutputPin(PORT4, RESET_LCD);
  GPIO_setOutputLowOnPin(PORT4, RESET_LCD);

  // Configure switch 1
  GPIO_setAsInputPinWithPullUpResistor(PORT4, SW1);
  GPIO_selectInterruptEdge(PORT4, SW1, GPIO_HIGH_TO_LOW_TRANSITION);
  GPIO_clearInterrupt(PORT4, SW1);
  GPIO_enableInterrupt(PORT4, SW1);
  
  P4SEL0 |= UCA1TXD;  // USCI_A1 UART operation
  P4SEL1 &= ~UCA1TXD; // USCI_A1 UART operation

  P4SEL0 |= UCA1RXD;  // USCI_A1 UART operation
  P4SEL1 &= ~UCA1RXD; // USCI_A1 UART operation

  P4SEL0 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
  P4SEL1 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
  P4OUT |= UCB1_CS_LCD;   // Set SPI_CS_LCD Off [High]
  P4DIR |= UCB1_CS_LCD;   // Set SPI_CS_LCD direction to output

  P4SEL0 |= UCB1CLK;  // UCB1CLK SPI BUS operation
  P4SEL1 &= ~UCB1CLK; // UCB1CLK SPI BUS operation

  P4SEL0 |= UCB1SIMO;  // UCB1SIMO SPI BUS operation
  P4SEL1 &= ~UCB1SIMO; // UCB1SIMO SPI BUS operation

  P4SEL0 |= UCB1SOMI;  // UCB1SOMI SPI BUS operation
  P4SEL1 &= ~UCB1SOMI; // UCB1SOMI SPI BUS operation
}

// Configure port 5
void init_port_5(void) {
  P5OUT = 0x00; // P5 set Low
  P5DIR = 0x00; // Set P5 direction to output

  // ADC inputs
  P5SELC |= V_BAT;
  P5SELC |= V_5_0;
  P5SELC |= V_DAC;
  P5SELC |= V_3_3;

  P5SEL0 &= ~IOT_BOOT;
  P5SEL1 &= ~IOT_BOOT;
  P5OUT |= ~IOT_BOOT;
  P5DIR |= ~IOT_BOOT;
}

// Configure port 6
void init_port_6(void) {
  P6OUT = 0x00; // P6 set Low
  P6DIR = 0x00; // Set P6 direction to output
  
  // Configure LCD_BACKLITE as PWM output
  P6SEL0 |= LCD_BACKLITE;
  P6SEL1 &= ~LCD_BACKLITE;
  P6DIR |= LCD_BACKLITE;
  P6OUT &= ~LCD_BACKLITE;

  // Configure R_FORWARD as PWM output
  P6SEL0 |= R_FORWARD;
  P6SEL1 &= ~R_FORWARD;
  P6DIR |= R_FORWARD;
  P6OUT &= ~R_FORWARD;

  // Configure L_FORWARD as PWM output
  P6SEL0 |= L_FORWARD;
  P6SEL1 &= ~L_FORWARD;
  P6DIR |= L_FORWARD;
  P6OUT &= ~L_FORWARD;

  // Configure R_REVERSE as PWM output
  P6SEL0 |= R_REVERSE;
  P6SEL1 &= ~R_REVERSE;
  P6DIR |= R_REVERSE;
  P6OUT &= ~R_REVERSE;

  // Configure L_REVERSE as PWM output
  P6SEL0 |= L_REVERSE;
  P6SEL1 &= ~L_REVERSE;
  P6DIR |= L_REVERSE;
  P6OUT &= ~L_REVERSE;

  // Unused, set to low input
  GPIO_setAsInputPin(PORT6, PIN5);
  GPIO_setOutputLowOnPin(PORT6, PIN5);

  // Configure GRN_LED as default high GPIO
  GPIO_setAsOutputPin(PORT6, GRN_LED);
  GPIO_setOutputHighOnPin(PORT6, GRN_LED);
}
