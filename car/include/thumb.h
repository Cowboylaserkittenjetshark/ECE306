#ifndef THUMB_H
#define THUMB_H

#include <stdbool.h>

#define THUMB_MAX (4096)

volatile unsigned int thumb;

void init_thumb(void);
void display_thumb_status(void);

#endif
