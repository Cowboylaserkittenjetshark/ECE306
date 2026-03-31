#ifndef DAC_H
#define DAC_H

#include "msp430.h"
#include <stdint.h>

// TODO Adjust
#define DAC_INIT   (2725) // 2 V
#define DAC_LIMIT  (850)  // 6.08 V
#define DAC_ADJUST (875)  // 6.00 V

void init_dac(void);

volatile unsigned int dac_data;

inline void dac_adjust(void);

#endif
