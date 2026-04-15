#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

typedef enum menu_state { SPLASH, MAIN, RESISTOR, SHAPE, SONG } MenuState;

static volatile MenuState ms;
static volatile bool ms_changed;
static volatile unsigned int last_thumb;
static volatile int song_toggle;
static volatile unsigned int lyric_offset;
volatile unsigned int selected_index;

void init_menu(void);
void menu_process(void);
void menu_back(void);
void menu_enter(void);
static inline void display_menu_splash(void);
static inline void display_menu_main(void);
static inline void display_menu_resistor(void);
static inline void display_menu_shape(void);
static inline void display_menu_song(void);

#endif
