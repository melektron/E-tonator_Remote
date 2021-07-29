#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include "lcd_chars.h"
#include "sound.h"
#include "nl_pot.h"


// some definitions for some characters
#define F_STATE_SAFE 0b10100101
#define F_STATE_READY 0b11011011
#define F_STATE_ACTIVE 0b11111111



// variables and classes from the main file
extern LiquidCrystal lcd;
extern const byte ROWS;
extern const byte COLS;
extern uint8_t keystates[16];




//=====================//
// mode specific stuff //
//=====================//

// mode: MODE_DEBUG
#define MODE_DEBUT_POT_READ_DELAY 1000  // delay between pot reads
void loopFunc_MODE_DEBUG();
void stateChange_MODE_DEBUG();
void drawInitialize_MODE_DEBUG();

// mode: MODE_SAFE_F
void loopFunc_MODE_SAFE_F();
void stateChange_MODE_SAFE_F();
void drawInitialize_MODE_SAFE_F();

// mode: MODE_NORMAL_FIRE
void loopFunc_MODE_NORMAL_F();
void stateChange_MODE_NORMAL_F();
void drawInitialize_MODE_NORMAL_F();

// mode: MODE_TOGGLE
void loopFunc_MODE_TOGGLE();
void stateChange_MODE_TOGGLE();
void drawInitialize_MODE_TOGGLE();

// mode: MODE_FLEX
void loopFunc_MODE_FLEX();
void stateChange_MODE_FLEX();
void drawInitialize_MODE_FLEX();