#include "include/display.h"
#include "include/global.h"
#include "include/ports.h"
#include <driverlib.h>

void display_process(void) {
  if (update_display) {
    update_display = 0;
    if (display_changed) {
      display_changed = 0;
      Display_Update(0, 0, 0, 0);
    }
  }
}

void toggle_backlight() {
  GPIO_toggleOutputOnPin(PORT6, LCD_BACKLITE);
}
