#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include "modes.h"

#define M_DOWN 1
#define M_UP 2

// definitions for modes (position of corresponding entries in menu_entries)
#define MODE_DEBUG 0
#define MODE_SAFE_F 1
#define MODE_NORMAL_F 2
#define MODE_TOGGLE 3
#define MODE_TIMED_F 4
#define MODE_FLEX 5


// variables that have to be accesed externaly
extern char menu_entries[10][15];
extern int8_t nr_menu_entries;
extern bool menu_mode;   // flag to indicate if menue is currently active
extern int8_t current_menu_selection;


// variables and classes from the main file
extern LiquidCrystal lcd;
extern const byte ROWS;
extern const byte COLS;
extern uint8_t keystates[16];



// functions that have to be accesed externaly
void getMenuEntry(char* outStr, uint8_t entry);
void draw_menu (uint8_t direction, int8_t menu_pos);
void menu_handler();
