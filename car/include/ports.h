#ifndef PORTS_H
#define PORTS_H
#include <driverlib.h>

#define FALSE (0x00)     //
#define TRUE (0x01)      //
#define MOTOR (0x00)     //
#define SMCLK_OFF (0x00) //
#define SMCLK_ON (0x01)  //
#define PORTS (0x00)     // RED LED 0
#define PWM_MODE (0x01)  // GREEN LED 1
#define WHEEL_OFF (0x00)
#define WHEEL_PERIOD (10000)
#define RIGHT_FORWARD_SPEED (TB3CCR2)
#define RIGHT_REVERSE_SPEED (TB3CCR3)
#define LEFT_FORWARD_SPEED (TB3CCR4)
#define LEFT_REVERSE_SPEED (TB3CCR5)
#define STEP (2000)
#define FORWARD (0x00) // FORWARD
#define REVERSE (0x01) // REVERSE

// Alias driverlib GPIO ports to short names
#define PORT1 (GPIO_PORT_P1)
#define PORT2 (GPIO_PORT_P2)
#define PORT3 (GPIO_PORT_P3)
#define PORT4 (GPIO_PORT_P4)
#define PORT5 (GPIO_PORT_P5)
#define PORT6 (GPIO_PORT_P6)

#define PIN0 (GPIO_PIN0)
#define PIN1 (GPIO_PIN1)
#define PIN2 (GPIO_PIN2)
#define PIN3 (GPIO_PIN3)
#define PIN4 (GPIO_PIN4)
#define PIN5 (GPIO_PIN5)
#define PIN6 (GPIO_PIN6)
#define PIN7 (GPIO_PIN7)

// Port 1 Pins
#define RED_LED    (PIN0) // 1.0 RED LED 0
#define A1_SEEED   (PIN1) // 1.1 A1_SEEED
#define V_DETECT_L (PIN2) // 1.2 V_DETECT_L
#define V_DETECT_R (PIN3) // 1.3 V_DETECT_R
#define A4_SEEED   (PIN4) // 1.4 A4_SEEED
#define V_THUMB    (PIN5) // 1.5 V_THUMB
#define UCA0RXD    (PIN6) // 1.6 Back Channel UCA0RXD
#define UCA0TXD    (PIN7) // 1.7 Back Channel UCA0TXD

// Port 2 Pins
#define SLOW_CLK    (PIN0) // 2.0 LCD Reset
#define CHECK_BAT   (PIN1) // 2.1 CHECK_BAT
#define IR_LED      (PIN2) // 2.2 IR LED
#define SW2         (PIN3) // 2.3 SW2
#define IOT_RUN_RED (PIN4) // 2.4 IOT_RUN_RED
#define DAC_ENB     (PIN5) // 2.5 DAC_ENB
#define LFXOUT      (PIN6) // 2.6 XOUTR
#define LFXIN       (PIN7) // 2.7 XINR

// Port 3 Pins
#define TEST_PROBE   (PIN0) // 3.0 TEST PROBE
#define OA2O         (PIN1) // 3.1 OA2O
#define OA2N         (PIN2) // 3.2 Photodiode Circuit
#define OA2P         (PIN3) // 3.3 Photodiode Circuit
#define SMCLK        (PIN4) // 3.4 SMCLK
#define DAC_CNTL     (PIN5) // 3.5 DAC_CNTL
#define IOT_LINK_GRN (PIN6) // 3.6 IOT_LINK_GRN
#define IOT_EN       (PIN7) // 3.7 IOT_EN

// Port 4 Pins
#define RESET_LCD   (PIN0) // 4.0 DAC_CNTL1
#define SW1         (PIN1) // 4.1 SW1
#define UCA1RXD     (PIN2) // 4.2 Back Channel UCA1RXD
#define UCA1TXD     (PIN3) // 4.3 Back Channel UCA1TXD
#define UCB1_CS_LCD (PIN4) // 4.4 Chip Select
#define UCB1CLK     (PIN5) // 4.5 SPI mode - clock output UCB1CLK
#define UCB1SIMO    (PIN6) // 4.6 UCB1SIMO
#define UCB1SOMI    (PIN7) // 4.7 UCB1SOMI

// Port 5 Pins
#define V_BAT        (PIN0) // 5.0 V_BAT
#define V_5_0        (PIN1) // 5.1 V_5_0
#define V_DAC        (PIN2) // 5.2 V_DAC
#define V_3_3        (PIN3) // 5.3 V_3_3
#define IOT_BOOT (PIN4) // 5.4 IOT_BOOT

// Port 6 Pins
#define LCD_BACKLITE (PIN0) // 6.0 LCD_BACKLITE
#define R_FORWARD    (PIN1) // 6.1 P6_0_PWM
#define L_FORWARD    (PIN2) // 6.2 P6_1_PWM
#define R_REVERSE    (PIN3) // 6.3 P6_2_PWM
#define L_REVERSE    (PIN4) // 6.4 P6_3_PWM
#define P6_5         (PIN5) // 6.5
#define GRN_LED      (PIN6) // 6.6 GREEN LED

void init_ports(void);
void init_port_1(void);
void init_port_2(void);
void init_port_3(void);
void init_port_4(void);
void init_port_5(void);
void init_port_6(void);

#endif
