#include "include/thumb.h"
#include "include/global.h" // Display

#include <stdio.h> // snprintf

void init_thumb() {
    thumb = THUMB_MAX;
}

void display_thumb_status() {
    snprintf(display_line[1] + 5, 5, "T:%d ", (thumb * 10) / 4095);
    display_line[1][9] = ' ';
    display_changed = true;
}