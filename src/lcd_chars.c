#include "lcd_chars.h"


// custom chars
byte LCD_CHAR_pause[8] = {
  B11011,
  B11011,
  B11011,
  B11011,
  B11011,
  B11011,
  B11011,
};

byte LCD_CHAR_play[8] = {
  B10000,
  B11000,
  B11100,
  B11110,
  B11100,
  B11000,
  B10000,
};

byte LCD_CHAR_back_to_start[8] = {
  B10001,
  B10011,
  B10111,
  B11111,
  B10111,
  B10011,
  B10001,
};