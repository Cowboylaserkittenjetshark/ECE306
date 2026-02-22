#ifndef CLOCKS_H
#define CLOCKS_H

#define MCLK_FREQ_MHZ (8) // MCLK = 8MHz
#define CLEAR_REGISTER (0X0000)

// Initialized clocks
void init_clocks(void);

// Software Trim to get the best DCOFTRIM value
void software_trim(void);

#endif
