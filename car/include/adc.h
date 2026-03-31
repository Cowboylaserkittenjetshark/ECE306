#ifndef ADC_H
#define ADC_H

// Initializes adc
void init_adc(void);

volatile unsigned int current_channel;
volatile unsigned int vbat;

#endif
