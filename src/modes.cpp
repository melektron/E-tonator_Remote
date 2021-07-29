#include "modes.h"




// Template for mode function structure
/*

// mode: MODE_
void loopFunc_MODE_() {}
void stateChange_MODE_() {}
void drawInitialize_MODE_() {}

*/




//=====================//
// mode specific stuff //
//=====================//

// mode: MODE_DEBUG
void loopFunc_MODE_DEBUG() {
  // code for loop counter to display the current execution speed of the programm
  // initialize the loop counter
  static unsigned long startTime = millis();
  static unsigned long loopCount = 0;

  // loop counter to count how often the loop runs per second
  loopCount++;
  if ( (millis()-startTime)>5000 ) {
  // clear the first row of the lcd screen
  lcd.setCursor(0, 0);
  lcd.print("                ");

  // turn the number of loops per second into string 
  char buf[17];
  sprintf(buf, "lps: %lu  ", loopCount/5);

  // and print it on the lcd
  lcd.setCursor(0, 0);
  lcd.print(buf);

  // reset the timer to make the code run again in 5 soconds
  startTime = millis();
  // reset the loop counter
  loopCount = 0;
  }


  // code for showing current potentiometer value
  static unsigned long last_pot_read = millis();   // stores time when the potentiometer was last read

  if (last_pot_read + MODE_DEBUT_POT_READ_DELAY < millis()) {
    lcd.setCursor(13, 0);           // set cursor to third-last column in the first line
    lcd.print(nl_pot_read(POT_PIN_INNER));     // print the value of the pot
    last_pot_read = millis();       // update the last time the pot was read
  }




}
void stateChange_MODE_DEBUG() {
  lcd.setCursor(0, 1);
  // print a block in the second row of display for every key that is pressed and a space for every key that is not pressed
  for (uint8_t i = 1; i < 13; i++) {    // we only loop to key 12 (in the for loop it has to bee one more) because key 13 and 14 are caught before this
                                        // function to open the menue and would be unnessesairy
    if (keystates[i] == PRESSED || keystates[i] == HOLD) {
      lcd.print((char)255);
    } else {
      lcd.print(' ');
    }
  }
  //Serial.println();
}
void drawInitialize_MODE_DEBUG() {
  return;
}


// mode: MODE_SAFE_F
void loopFunc_MODE_SAFE_F() {return;}
void stateChange_MODE_SAFE_F() {
  // this mode is identical to NORMAL_FIRE except
  // for the way it turns on the output on the receiver.
  // normal mode sends a command to turn the pin on, while
  // safe mode sends a "save" turn on command.
  // this tells the receiver to automatically turn of the output
  // after a defined (typically 1 second) time, even if the remote 
  // has not sent the turn off command
  
  // first we print the states of the safety levers to the
  // ready-indicators on the screen when they change their state.
  // (even if no fire switch was pressed, the state should still
  // update on the screen, that is what this does)
  lcd.setCursor(0, 1);
  for (uint8_t i = 7; i < 13; i++) {  // for each safety lever
    if (keystates[i] == PRESSED) {    // if it is pressed 
      lcd.print((char)F_STATE_READY); // print the READY symobol at the corresponding location on the display
    } else if (keystates[i] == RELEASED){ // if it has been released 
      lcd.print((char)F_STATE_SAFE);  // print the SAFE symbol at the corresponding location on the display
    }
    lcd.setCursor(3*(i-6), 1);        // set the cursot to the location of the next state-indicator
  }
  // the following code will handle the fire-button state changes
  lcd.setCursor(0, 1);
  for (uint8_t i = 1; i < 7; i++) {     // for each fire button
    if ((keystates[i] == PRESSED) && (keystates[i+6] == HOLD)) {    // if the button has been pressed and the corresponding safety lever is turned on
      lcd.print((char)F_STATE_ACTIVE);    // print the ACTIVE symbol (just a white block) on the screen
      Serial.print(";"); Serial.print(i); Serial.println("s");    // send the command to the receiver to turn on the output (here it sends "s" at the end to indicate that is is a "safe" turn-on)
    } else if (keystates[i] == RELEASED && keystates[i+6] == HOLD) {  // if the button has been released but the safety lever is still active
      lcd.print((char)F_STATE_READY);   // we print the READY symbol again to the screen 
      Serial.print(";"); Serial.print(i); Serial.println("0");  // and send the turn-off command
    } else if (keystates[i] == RELEASED && (keystates[i+6] == IDLE || keystates[i+6] == RELEASED)) {  // if the button has been released and the safety switch as well
      lcd.print((char)F_STATE_SAFE);  // we print the SAFE symbol instead of the READY symbol
      Serial.print(";"); Serial.print(i); Serial.println("0");    // and send the turn-off command
    }
    lcd.setCursor(3*i, 1);    // set the cursor to the position of the next button's state-indicator
  }
}
void drawInitialize_MODE_SAFE_F() {
  lcd.setCursor(0, 0);
  lcd.print("Safe Fire"); // print the mode name
  lcd.setCursor(0, 1);      
  for (uint8_t i = 7; i < 13; i++) { // for each safety-lever
    // we print it's current state to the state-indicator on the display
    if (keystates[i] == PRESSED || keystates[i] == HOLD) {
      lcd.print((char)F_STATE_READY);
    } else {
      lcd.print((char)F_STATE_SAFE);
    }
    lcd.setCursor(3*(i-6), 1);  // set the cursot to the location of the next state-indicator
  }
}


// mode: NORMAL_FIRE
void loopFunc_MODE_NORMAL_F() {return;}
void stateChange_MODE_NORMAL_F() {
  // this mode will simply send a turn-on command to the output of the receiver
  // when a button is pushed (and the safety lever is activated) and send a 
  // turn-off signal if the button was released
  
  // first we print the states of the safety levers to the
  // ready-indicators on the screen when they change their state.
  // (even if no fire switch was pressed, the state should still
  // update on the screen, that is what this does)
  lcd.setCursor(0, 1);
  for (uint8_t i = 7; i < 13; i++) {  // for each safety lever
    if (keystates[i] == PRESSED) {    // if it is pressed 
      lcd.print((char)F_STATE_READY); // print the READY symobol at the corresponding location on the display
    } else if (keystates[i] == RELEASED){ // if it has been released 
      lcd.print((char)F_STATE_SAFE);  // print the SAFE symbol at the corresponding location on the display
    }
    lcd.setCursor(3*(i-6), 1);        // set the cursot to the location of the next state-indicator
  }
  // the following code will handle the fire-button state changes
  lcd.setCursor(0, 1);
  for (uint8_t i = 1; i < 7; i++) {     // for each fire button
    if ((keystates[i] == PRESSED) && (keystates[i+6] == HOLD)) {    // if the button has been pressed and the corresponding safety lever is turned on
      lcd.print((char)F_STATE_ACTIVE);    // print the ACTIVE symbol (just a white block) on the screen
      Serial.print(";"); Serial.print(i); Serial.println("1");    // send the command to the receiver to turn on the output 
    } else if (keystates[i] == RELEASED && keystates[i+6] == HOLD) {  // if the button has been released but the safety lever is still active
      lcd.print((char)F_STATE_READY);   // we print the READY symbol again to the screen 
      Serial.print(";"); Serial.print(i); Serial.println("0");  // and send the turn-off command
    } else if (keystates[i] == RELEASED && (keystates[i+6] == IDLE || keystates[i+6] == RELEASED)) {  // if the button has been released and the safety switch as well
      lcd.print((char)F_STATE_SAFE);  // we print the SAFE symbol instead of the READY symbol
      Serial.print(";"); Serial.print(i); Serial.println("0");    // and send the turn-off command
    }
    lcd.setCursor(3*i, 1);    // set the cursor to the position of the next button's state-indicator
  }
}
void drawInitialize_MODE_NORMAL_F() {
  lcd.setCursor(0, 0);
  lcd.print("Normal Fire"); // print the mode name
  lcd.setCursor(0, 1);      
  for (uint8_t i = 7; i < 13; i++) { // for each safety-lever
    // we print it's current state to the state-indicator on the display
    if (keystates[i] == PRESSED || keystates[i] == HOLD) {
      lcd.print((char)F_STATE_READY);
    } else {
      lcd.print((char)F_STATE_SAFE);
    }
    lcd.setCursor(3*(i-6), 1);  // set the cursot to the location of the next state-indicator
  }
}


// mode: TOGGLE
void loopFunc_MODE_TOGGLE() {return;}
void stateChange_MODE_TOGGLE() {
  // this mode will simply send a turn-on command to the output of the receiver
  // when a lever is turned on and send a turn-off command when a lever is
  // turned off
  
  lcd.setCursor(0, 1);
  for (uint8_t i = 7; i < 13; i++) {  // for each lever
    if (keystates[i] == PRESSED) {    // if it is pressed 
      lcd.print((char)F_STATE_ACTIVE); // print the ACTIVE symobol at the corresponding location on the display
      Serial.print(";"); Serial.print(i-6); Serial.println("1");    // send the command to the receiver to turn on the output 
    } else if (keystates[i] == RELEASED){ // if it has been released 
      lcd.print((char)F_STATE_READY);  // print the READY symbol at the corresponding location on the display
      Serial.print(";"); Serial.print(i-6); Serial.println("0");    // send the command to the receiver to turn off the output 
    }
    lcd.setCursor(3*(i-6), 1);        // set the cursot to the location of the next state-indicator
  }
}
void drawInitialize_MODE_TOGGLE() {
  lcd.setCursor(0, 0);
  lcd.print("Toggle"); // print the mode name
  lcd.setCursor(0, 1);
  // initialize the states of the levers on the display (will however not send a state change to the receiver, unless you change a lever after activating toggle mode)
  for (uint8_t i = 7; i < 13; i++) { // for each lever
    // we print it's current state to the state-indicator on the display
    if (keystates[i] == PRESSED || keystates[i] == HOLD) {
      lcd.print((char)F_STATE_ACTIVE);
    } else {
      lcd.print((char)F_STATE_READY);
    }
    lcd.setCursor(3*(i-6), 1);  // set the cursot to the location of the next state-indicator
  }
}


// mode: MODE_FELX
void loopFunc_MODE_FLEX() {return;}
void stateChange_MODE_FLEX() {
  if (keystates[2] == PRESSED) {
    lcd.setCursor(3, 1); lcd.write((byte)255);
    music_BackToStart();
  }
  if (keystates[3] == PRESSED) {
    lcd.setCursor(8, 1); lcd.write((byte)255);
    music_Play();
  }
  if (keystates[4] == PRESSED) {
    lcd.setCursor(14, 1); lcd.write((byte)255);
    music_Pause();
  }

  // button releases
  if (keystates[2] == RELEASED) {
    lcd.setCursor(3, 1); lcd.write((byte)LCD_CHAR_BACK_T_S);
  }
  if (keystates[3] == RELEASED) {
    lcd.setCursor(8, 1); lcd.write((byte)LCD_CHAR_PLAY);
  }
  if (keystates[4] == RELEASED) {
    lcd.setCursor(14, 1); lcd.write((byte)LCD_CHAR_PAUSE);
  }
  return;
}
void drawInitialize_MODE_FLEX() {
  lcd.setCursor(0, 0);
  lcd.print("Flex Mode!!");
  lcd.setCursor(3, 1);
  lcd.write((byte)LCD_CHAR_BACK_T_S);
  lcd.setCursor(8, 1);
  lcd.write((byte)LCD_CHAR_PLAY);
  lcd.setCursor(14, 1);
  lcd.write((byte)LCD_CHAR_PAUSE);
}