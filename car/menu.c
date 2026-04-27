#include "include/menu.h"
#include "include/thumb.h" // For thumb wheel value
#include "include/global.h" // Display
#include "include/display.h" // Display utilities

#include <stdio.h> // snprintf
#include <string.h> // strncpy

static const char main_menu_items[3][11] = {"Resistors", "Shapes", "Song"};
static const char colors[10][11] = {"Black", "Brown", "Red", "Orange", "Yellow", "Green", "Blue", "Violet", "Gray", "White"};
static const char shapes[12][11] = {"Circle", "Square", "Triangle", "Octagon", "Pentagon", "Hexagon", "Cube", "Oval", "Sphere", "Cylinder"};
static const char lyrics[225] = "We're the Red and White from State And we know we are the best. A hand behind our back, We can take on all the rest. Come over the hill, Caroline. Devils and Deacs stand in line. The Red and White from N.C. State. Go State!";
void init_menu() {
    ms = SPLASH;
    ms_changed = false;
    last_thumb = thumb;
    song_toggle = false;
    lyric_offset = 0;
    selected_index = ((thumb >> 2) * 10) / (THUMB_MAX >> 2);

    display_menu_splash();
}

void menu_process() {
    switch (ms) {
        case SPLASH:
            if(ms_changed) display_menu_splash();
            break;
        case MAIN:
            if(ms_changed || last_thumb != thumb) display_menu_main();
            break;
        case RESISTOR:
            if(ms_changed || last_thumb != thumb) display_menu_resistor();
            break;
        case SHAPE:
            if(ms_changed || last_thumb != thumb) display_menu_shape();
            break;
        case SONG:
            if(ms_changed || last_thumb != thumb) display_menu_song();
            break;
    }
    last_thumb = thumb;
}

void display_menu_splash() {
    lcd_BIG_mid();
    strncpy(display_line[0], "  Ashton  ", 10);
    strncpy(display_line[1], "Homework 9", 10);
    strncpy(display_line[2], "Henderlite", 10);
    display_changed = true;
}

void display_menu_main() {
    lcd_BIG_mid();
    if(thumb < (THUMB_MAX / 3)) {
        strncpy(display_line[0], "          ", 10);
        snprintf(display_line[1], 11, "%s          ", main_menu_items[0]);
        snprintf(display_line[2], 11, "%s          ", main_menu_items[1]);
    } else if(thumb < (THUMB_MAX / 3) * 2) {
        snprintf(display_line[0], 11, "%s          ", main_menu_items[0]);
        snprintf(display_line[1], 11, "%s          ", main_menu_items[1]);
        snprintf(display_line[2], 11, "%s          ", main_menu_items[2]);
    } else {
        snprintf(display_line[0], 11, "%s          ", main_menu_items[1]);
        snprintf(display_line[1], 11, "%s          ", main_menu_items[2]);
        strncpy(display_line[2], "          ", 10);
    }
    display_changed = true;
}

void display_menu_resistor() {
    selected_index = ((thumb >> 2) * 10) / (THUMB_MAX >> 2);
    if(selected_index > 9) selected_index = 9;
    lcd_4line();
    snprintf(display_line[0], 11, "Color:          ");
    snprintf(display_line[1], 11, "%s          ", colors[selected_index]);
    snprintf(display_line[2], 11, "Value:          ");
    snprintf(display_line[3], 11, "%d          ", selected_index);
}

void display_menu_shape() {
    lcd_BIG_mid();
    selected_index = ((thumb >> 2) * 10) / (THUMB_MAX >> 2);
    if(selected_index > 9) selected_index = 9;
    if(selected_index == 0) strncpy(display_line[0], "          ", 10);
    else snprintf(display_line[0], 11, "%s          ", shapes[selected_index - 1]);
    snprintf(display_line[1], 11, "%s          ", shapes[selected_index]);
    if(selected_index == 9) strncpy(display_line[2], "          ", 10);
    else snprintf(display_line[2], 11, "%s          ", shapes[selected_index + 1]);
    display_changed = true;
}

void display_menu_song() {
    lcd_BIG_mid();
    if(song_toggle) {
        song_toggle = false;
        strncpy(display_line[0], "    Red   ", 11);
        strncpy(display_line[2], "   White  ", 11);
    } else {
        song_toggle = true;
        strncpy(display_line[0], "   White  ", 11);
        strncpy(display_line[2], "    Red   ", 11);
    }
    if(thumb > last_thumb) {
        if(lyric_offset > 254) lyric_offset = 254;
        snprintf(display_line[1], 11, "%s          ", lyrics + lyric_offset);
        lyric_offset += 1;
    }
    display_changed = true;
}

void menu_back() {
    switch (ms) {
        case SPLASH: break;
        case MAIN:
            ms = SPLASH;
            ms_changed = true;
            break;
        default:
            ms = MAIN;
            ms_changed = true;
            lyric_offset = 0;
            strncpy(display_line[3], "          ", 10);
            display_changed = true;
            break;
    }
}

void menu_enter() {
    switch (ms) {
        case SPLASH:
            ms = MAIN;
            ms_changed = true;
            break;
        case MAIN:
            if(thumb < (THUMB_MAX / 3)) ms = RESISTOR;
            else if(thumb < (THUMB_MAX / 3) * 2) ms = SHAPE;
            else ms = SONG;
            ms_changed = true;
            break;
        default: break;
    }
}
