#include "menu.h"

char menu_entries [10][15] {
    "Debug Keypad",
    "Safe Fire",
    "Normal Fire",
    "Toggle",
    "Timed Fire",
    "Flex Mode!!"
};

int8_t nr_menu_entries = 6;


bool menu_mode = false;   // flag to indicate if menue is currently active
int8_t current_menu_selection = 0;



void getMenuEntry(char* outStr, uint8_t entry) {
    strcpy(outStr, menu_entries[entry]);
}

void draw_menu (uint8_t direction, int8_t menu_pos) {
  lcd.clear();
  if (direction == M_UP) {
    lcd.setCursor(0, 0);
    lcd.print("> ");
    lcd.print(menu_entries[current_menu_selection]);
    lcd.setCursor(2, 1);
    lcd.print(menu_entries[current_menu_selection + 1]);
  } else if (direction == M_DOWN) {
    lcd.setCursor(0, 1);
    lcd.print("> ");
    lcd.print(menu_entries[current_menu_selection]);
    lcd.setCursor(2, 0);
    lcd.print(menu_entries[current_menu_selection - 1]);
  }
}

void menu_handler(){
  if (keystates[13] == PRESSED) {   // if the down key is pressed
    current_menu_selection++;       // the memory selection counter gets incremented with confusingly makes the memory list go one down
    if (current_menu_selection > nr_menu_entries - 1) {   // if we were already on the lowest entry, we cannot go any further down
      current_menu_selection = nr_menu_entries - 1;
    }
    draw_menu(M_DOWN, current_menu_selection);            // then we draw the menue with the arrow in the down position

  } else if (keystates[14] == PRESSED) {                // same goes for up key
    current_menu_selection--;
    if (current_menu_selection < 0) {
      current_menu_selection = 0;
    }
    draw_menu(M_UP, current_menu_selection);

  }
  if (keystates[6] == RELEASED) {    // key 6 is the "OK" key in menu mode. it makes the menu close and clears the screen for whatever code to 
                                    // run in the selected mode to fill it again and run an drawInitialize function for each mode
    menu_mode = false;
    lcd.clear();
    if (current_menu_selection == MODE_DEBUG) {
      drawInitialize_MODE_DEBUG();
    } else if (current_menu_selection == MODE_NORMAL_F) {
      drawInitialize_MODE_NORMAL_F();
    } else if (current_menu_selection == MODE_FLEX) {
      drawInitialize_MODE_FLEX();
    } else if (current_menu_selection == MODE_SAFE_F) {
      drawInitialize_MODE_SAFE_F();
    } else if (current_menu_selection == MODE_TOGGLE) {
      drawInitialize_MODE_TOGGLE();
    }
  }
}