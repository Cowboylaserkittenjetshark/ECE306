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
  P1OUT &= ~RED_LED;
  P1DIR |= RED_LED;

  P1SEL0 |= A1_SEEED;
  P1SEL1 |= A1_SEEED;
  P1OUT &= ~A1_SEEED;
  P1DIR &= ~A1_SEEED;

  P1SEL0 |= V_DETECT_L;
  P1SEL1 |= V_DETECT_L;
  P1OUT &= ~V_DETECT_L;
  P1DIR &= ~V_DETECT_L;

  P1SEL0 |= V_DETECT_R;
  P1SEL1 |= V_DETECT_R;
  P1OUT &= ~V_DETECT_R;
  P1DIR &= ~V_DETECT_R;

  P1SEL0 |= A4_SEEED;
  P1SEL1 |= A4_SEEED;
  P1OUT &= ~A4_SEEED;
  P1DIR &= ~A4_SEEED;

  P1SEL0 |= V_THUMB;
  P1SEL1 |= V_THUMB;
  P1OUT &= ~V_THUMB;
  P1DIR &= ~V_THUMB;

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

  P2SEL0 &= ~SLOW_CLK; // SLOW_CLK GPIO operation
  P2SEL1 &= ~SLOW_CLK; // SLOW_CLK GPIO operation
  P2OUT &= ~SLOW_CLK;  // Initial Value = Low / Off
  P2DIR |= SLOW_CLK;   // Direction = output

  P2SEL0 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
  P2SEL1 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
  P2OUT &= ~CHECK_BAT;  // Initial Value = Low / Off
  P2DIR |= CHECK_BAT;   // Direction = output

  P2SEL0 &= ~IR_LED; // P2_2 GPIO operation
  P2SEL1 &= ~IR_LED; // P2_2 GPIO operation
  P2OUT &= ~IR_LED;  // Initial Value = Low / Off
  P2DIR |= IR_LED;   // Direction = output

  GPIO_setAsInputPinWithPullUpResistor(PORT2, SW2);
  GPIO_selectInterruptEdge(PORT2, SW2, GPIO_HIGH_TO_LOW_TRANSITION);
  GPIO_clearInterrupt(PORT2, SW2);
  GPIO_enableInterrupt(PORT2, SW2);

  P2SEL0 &= ~IOT_RUN_RED; // IOT_RUN_CPU GPIO operation
  P2SEL1 &= ~IOT_RUN_RED; // IOT_RUN_CPU GPIO operation
  P2OUT &= ~IOT_RUN_RED;  // Initial Value = Low / Off
  P2DIR |= IOT_RUN_RED;   // Direction = output

  P2SEL0 &= ~DAC_ENB; // DAC_ENB GPIO operation
  P2SEL1 &= ~DAC_ENB; // DAC_ENB GPIO operation
  P2OUT |= DAC_ENB;   // Initial Value = High
  P2DIR |= DAC_ENB;   // Direction = output

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

  P3SEL0 |= DAC_CNTL;
  P3SEL1 |= DAC_CNTL;
  P3OUT &= ~DAC_CNTL;
  P3DIR &= ~DAC_CNTL;

  P3SEL0 |= IOT_LINK_GRN;
  P3SEL1 |= IOT_LINK_GRN;
  P3OUT &= ~IOT_LINK_GRN;
  P3DIR &= ~IOT_LINK_GRN;

  P3SEL0 |= IOT_EN;
  P3SEL1 |= IOT_EN;
  P3OUT &= ~IOT_EN;
  P3DIR &= ~IOT_EN;
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

  P5SEL0 |= V_BAT;
  P5SEL1 |= V_BAT;
  P5OUT &= ~V_BAT;
  P5DIR &= ~V_BAT;

  P5SEL0 |= V_5_0;
  P5SEL1 |= V_5_0;
  P5OUT &= ~V_5_0;
  P5DIR &= ~V_5_0;

  P5SEL0 |= V_DAC;
  P5SEL1 |= V_DAC;
  P5OUT &= ~V_DAC;
  P5DIR &= ~V_DAC;

  P5SEL0 |= V_3_3;
  P5SEL1 |= V_3_3;
  P5OUT &= ~V_3_3;
  P5DIR &= ~V_3_3;

  P5SEL0 &= ~IOT_BOOT_CPU;
  P5SEL1 &= ~IOT_BOOT_CPU;
  P5OUT &= ~IOT_BOOT_CPU;
  P5DIR &= ~IOT_BOOT_CPU;
}

// Configure port 6
void init_port_6(void) {
  P6OUT = 0x00; // P6 set Low
  P6DIR = 0x00; // Set P6 direction to output
  
  // Configure LCD_BACKLITE as default low GPIO output
  GPIO_setAsOutputPin(PORT6, LCD_BACKLITE);
  GPIO_setOutputLowOnPin(PORT6, LCD_BACKLITE);
  
  // Configure R_FORWARD as default low GPIO output
  GPIO_setAsOutputPin(PORT6, R_FORWARD);
  GPIO_setOutputLowOnPin(PORT6, R_FORWARD);
  
  // Configure L_FORWARD as default low GPIO output
  GPIO_setAsOutputPin(PORT6, L_FORWARD);
  GPIO_setOutputLowOnPin(PORT6, L_FORWARD);
  
  // Configure R_REVERSE as default low GPIO output
  GPIO_setAsOutputPin(PORT6, R_REVERSE);
  GPIO_setOutputLowOnPin(PORT6, R_REVERSE);
  
  // Configure L_REVERSE as default low GPIO output
  GPIO_setAsOutputPin(PORT6, L_REVERSE);
  GPIO_setOutputLowOnPin(PORT6, L_REVERSE);

  // Unused, set to low input
  GPIO_setAsInputPin(PORT6, PIN5);
  GPIO_setOutputLowOnPin(PORT6, PIN5);

  // Configure GRN_LED as default high GPIO
  GPIO_setAsOutputPin(PORT6, GRN_LED);
  GPIO_setOutputHighOnPin(PORT6, GRN_LED);
}
