#ifndef INTERRUPTS_H
#define INTERRUPTS_H

void enable_interrupts(void);
__interrupt void Timer0_B0_ISR(void);
__interrupt void switch_interrupt(void);

#endif
