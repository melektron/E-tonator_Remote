/*
This file contains all the platrom specific code for the Tetris game.
It also interfaces the game with the mode manager and contains the rendering functions so 
the mode file itself (modes.cpp) is not filled with game code.
*/
 
#include <LiquidCrystal.h>
#include <Keypad.h>
#include "game.h"

// objects and variables from other modules
extern LiquidCrystal lcd;
extern uint8_t  keystates[16];


// functions to be called by the game core (game.cpp)

void D_drawFrame(uint16_t frame[D_ROWS], uint8_t row, uint8_t col, uint16_t score, uint16_t rows_cleared);


// functions to be called by the menu mode manager

void D_I_loopFunc();
void D_I_keyStateChangeHandler();
void D_I_initialize();