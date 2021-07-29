#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include "menu.h"
#include "modes.h"
#include "sound.h"
#include "lcd_chars.h"
#include "nl_pot.h"




// initialize the LCD lib
const int rs = A4, en = A5, d4 = A3, d5 = A2, d6 = A1, d7 = A0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);




// Initialize the keypad lib
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns



// array to hold keymap for keypad lib
char keymap[ROWS][COLS] = {
{1,2,3,4},
{7,8,9,10},
{12,11,6,5},
{13,14,15,16}    // 15 and 16 are not used here but have to be filled because the lib expects a complete 4x4 matrix 
};

byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the kpd


// array to hold key states
uint8_t keystates[16];
bool keymap_invalid = false;


Keypad kpd = Keypad( makeKeymap(keymap), rowPins, colPins, ROWS, COLS );




// ================= //
//    Functions      //
// ================= //


void handle_keystate_change() {
  

  if (menu_mode) {
    // if we are in menu mode and any buttons are pressed, the menu handler should be called
    menu_handler();

  } else {
    // this code get's executed when buttons are pressed while not in the menue

    if (keystates[13] == PRESSED || keystates[14] == PRESSED) {
      menu_mode = true;
      if (current_menu_selection == nr_menu_entries - 1) {
        draw_menu(M_DOWN, current_menu_selection);
      }
      draw_menu(M_UP, current_menu_selection);

    } else {
      // if no menu activate keys where pressed, it depends on the current mode (menu_selection) how to handle the key press
      // note that this will automatically run as soon as ou exit the menu, thank's to the stat change on the key RELSEASE
      if (current_menu_selection == MODE_DEBUG) {
        stateChange_MODE_DEBUG();
      } else if (current_menu_selection == MODE_NORMAL_F) {
        stateChange_MODE_NORMAL_F();
      } else if (current_menu_selection == MODE_FLEX) {
        stateChange_MODE_FLEX();
      } else if (current_menu_selection == MODE_SAFE_F) {
        stateChange_MODE_SAFE_F();
      } else if (current_menu_selection == MODE_TOGGLE) {
        stateChange_MODE_TOGGLE();
      } else if (current_menu_selection == MODE_TETRIS) {
        stateChange_MODE_TETRIS();
      }
    }
  }

}





void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // initialize custom chars
  lcd.createChar(LCD_CHAR_PLAY, LCD_CHAR_play);
  lcd.createChar(LCD_CHAR_PAUSE, LCD_CHAR_pause);
  lcd.createChar(LCD_CHAR_BACK_T_S, LCD_CHAR_back_to_start);

  // setup serial communication
  Serial.begin(115200);


  // initialize Pins
  pinMode(POT_PIN_INNER, INPUT);
  pinMode(POT_PIN_OUTER, INPUT);



  // clear the display at startup
  lcd.clear();
  // print test char in the bottom right of screen for fun
  lcd.setCursor(13, 1);
  lcd.print((char)LCD_CHAR_PLAY);
  lcd.print((char)LCD_CHAR_PAUSE);
  lcd.print((char)LCD_CHAR_BACK_T_S);
}


void loop() {
  /*
  static char last_pressed_key;

  char pressedKey = kpd.getKey();
  if (pressedKey != NO_KEY && pressedKey != last_pressed_key) {
    // clear the second row except for last char
    lcd.setCursor(0, 1);
    lcd.print("               ");
    
    // print key to screen and serial monitor
    char buf[17];
    sprintf(buf, "Got Key: %u", (uint8_t)pressedKey);
    lcd.setCursor(0, 1);
    lcd.print(buf);
    Serial.println(buf);

    last_pressed_key = pressedKey;
  }*/


  // fill keystates array with any active keys
  if(kpd.getKeys()) {
    for (uint8_t i = 0; i < LIST_MAX; i++) {
      if (kpd.key[i].stateChanged) {
        keymap_invalid = true;
        //Serial.print((int)kpd.key[i].kchar);
        switch (kpd.key[i].kstate) {
          case PRESSED:
          //Serial.println(" PRESSED");
          keystates[(uint8_t)kpd.key[i].kchar] = PRESSED;
          break;

          case HOLD:
          //Serial.println(" HOLD");
          keystates[(uint8_t)kpd.key[i].kchar] = HOLD;
          break;

          case RELEASED:
          //Serial.println(" RELEASED");
          keystates[(uint8_t)kpd.key[i].kchar] = RELEASED;
          break;

          case IDLE:
          //Serial.println(" IDLE");
          keystates[(uint8_t)kpd.key[i].kchar] = IDLE;
          break;
        }
      }
    }
  }

  if (keymap_invalid) {
    handle_keystate_change();
    keymap_invalid = false;
  }


  // the music should run even when not in the menu so it is outside of the current_menu_selection selector
  music_LoopFunction();

  // mode dependet code to execute without user input
  if (!menu_mode) {
    if (current_menu_selection == MODE_DEBUG) {
      loopFunc_MODE_DEBUG();
    } else if (current_menu_selection == MODE_NORMAL_F) {
      loopFunc_MODE_NORMAL_F();
    } else if (current_menu_selection == MODE_FLEX) {
      loopFunc_MODE_FLEX();
    } else if (current_menu_selection == MODE_SAFE_F) {
      loopFunc_MODE_SAFE_F();
    } else if (current_menu_selection == MODE_TOGGLE) {
      loopFunc_MODE_TOGGLE();
    } else if (current_menu_selection == MODE_TETRIS) {
      loopFunc_MODE_TETRIS();
    }
    // code for other modes here
  }
}
