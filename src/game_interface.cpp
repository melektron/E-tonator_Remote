#include "game_interface.h"


unsigned long lastTickTime = 0;         // var to save the last time a game tick happened
const uint16_t tick_delay = 1000;       // delay between game ticks
bool keyPrevPressed[4] = {0, 0, 0, 0};  // flags to store wether key have already been pressed in order to avoid retriggering on another key event for previously pressed events

const uint16_t autoRepeatDelay = 200;   // delay between auto repeat when holding down a key
unsigned long lastAutoRepeat[4] = {0, 0, 0, 0};

bool gamePaused = true;                 // flag to indicate wether or not the game is paused


// this is an array for the contents of custom characters.
// it will be initialized with all pixels off, therefore having an empty character.
// it will also be filled with chunks of the frame buffer and then written to the custom chars on the display for rendering
byte game_frame_section_default[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};

byte game_frame_section[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};

// this array contains the coordinates of the top left pixel of every frame chunk on the frame buffer
// in the order {row, col}
uint8_t frame_chunk_locations[8][2] = {
    {15, 0},
    {10, 0},
    {5,  0},
    {0, 0},
    {15, 8},
    {10, 8},
    {5,  8},
    {0,  8},

};



void resetGame() {
    lastTickTime = millis();
    gamePaused = true;
    G_ResetGame();
}


// this function gets executed by the game core whenever a frame has to be rendered
void D_drawFrame(uint16_t frame[D_ROWS], uint8_t row, uint8_t col, uint16_t score, uint16_t rows_cleared) {
    // rendering code to be added
    
    // for every one of the 8 frame chunks
    for (uint8_t chunk = 0; chunk < 8; chunk++) {
        
        // first we reset the general frame chunk buffer by overwriting it with the default frame chunk value (all pixels off)
        memcpy(game_frame_section, game_frame_section_default, sizeof(game_frame_section_default));
        
        // next we get the coordinates of the top left pixel of the chunk from the array
        const uint8_t chunk_tl_row = frame_chunk_locations[chunk][0];
        const uint8_t chunk_tl_col = frame_chunk_locations[chunk][1];

        // next we go through every row of the chunk on the frame buffer
        for (uint8_t pixel_row = chunk_tl_row; pixel_row < chunk_tl_row + 5; pixel_row++) {

            uint16_t frame_row = frame[pixel_row];    // copy the row into a variable for more flexibility
            uint8_t chunk_pixel_row = pixel_row - chunk_tl_row;     // and calculate the coordinates of the row in the chunk's coordinate system

            // and then go through every pixel in the row
            for (uint8_t pixel_col = chunk_tl_col; pixel_col < chunk_tl_col + 8; pixel_col++) {
                uint8_t chunk_pixel_col = pixel_col - chunk_tl_col;     // and calculate the col coordinate of the pixel in the chunk's coordinate system
                
                // the pixels are counted from the left to right, but we need them all the way in the rightmost bit.
                // so we shift them to the right for the entire width of the row minus the colomn number. 
                // lastly everything gets anded with a 16 bit 1 to get the pixel value as 1 or 0 in the 8 bit integer
                uint8_t pixel_state = (frame_row >> (15 - pixel_col)) & (uint16_t)1;
                //Serial.print(pixel_state);  // print the pixel for debugging

                // for writing a pixel to a chunk of the display, we need to rotate everything by 90 degrees
                // because the characters on the display will be oriented that way during gameplay.
                // therefore we swap rows and cols
                //          frame cols for the char rows
                //                         |
                //                        \ /
                game_frame_section[chunk_pixel_col] |= (pixel_state << chunk_pixel_row);
            }
            //Serial.println();   // also print the next line for debugging
        }
        // once the chunk buffer has been loaded with the correct pixel values
        // we update that chunk on the display
        lcd.createChar(chunk, game_frame_section);
    }
    
    // now we print the score and the cleared lines on the screen:
    lcd.setCursor(5, 0); lcd.print("SC:"); lcd.setCursor(8, 0); lcd.print(score);
    lcd.setCursor(5, 0); lcd.print("RC:"); lcd.setCursor(8, 0); lcd.print(rows_cleared);
    // print score for debugging
    //Serial.print("Score: "); Serial.println(score);
    return;
}

// functions to be called by the menu mode manager

void D_I_loopFunc() {
    if (!gamePaused) {
        // in the game loop, game ticks should be envoked every second
        if (millis() > lastTickTime + tick_delay) {
            G_on_game_tick();
            lastTickTime = millis();
            //Serial.println("Game Tick");
        }
    }
}

void D_I_keyStateChangeHandler() {
    if (!gamePaused) {
        // we don't use else if here because multiple of these button states could be pressed
        // if any keys are pressed we execute the corresponding code of the game core
        // and set a flag that this key has been pressed. that way, when another key triggers an update
        // the the previous key action will not get executed again before the key has been released
        if (keystates[1] == PRESSED) {
            if (!keyPrevPressed[0]) {G_key_down();}
            keyPrevPressed[0] = true;
        }
        if (keystates[2] == PRESSED) {
            if (!keyPrevPressed[1]) {G_key_left();}
            keyPrevPressed[1] = true;
        }
        if (keystates[3] == PRESSED) {
            if (!keyPrevPressed[2]) {G_key_right();}
            keyPrevPressed[2] = true;
        }
        if (keystates[4] == PRESSED) {
            if (!keyPrevPressed[4]) {G_key_up();}
            keyPrevPressed[3] = true;
        }

        // once the keys have been released, we reset the flag
        if (keystates[1] == RELEASED) {
            keyPrevPressed[0] = false;
        }
        if (keystates[2] == RELEASED) {
            keyPrevPressed[1] = false;
        }
        if (keystates[3] == RELEASED) {
            keyPrevPressed[2] = false;
        }
        if (keystates[4] == RELEASED) {
            keyPrevPressed[3] = false;
        }

        // if key are pressed long enough to go into the hold mode, we execute the corresponding
        // action every <autoRepeatDelay> ms.
        if (keystates[1] == HOLD) {
            if (millis() > lastAutoRepeat[0] + autoRepeatDelay) {
                G_key_down();
                lastAutoRepeat[0] = millis();
            }
        }
        if (keystates[2] == HOLD) {
            if (millis() > lastAutoRepeat[1] + autoRepeatDelay) {
                G_key_left();
                lastAutoRepeat[1] = millis();
            }
        }
        if (keystates[3] == HOLD) {
            if (millis() > lastAutoRepeat[2] + autoRepeatDelay) {
                G_key_right();
                lastAutoRepeat[2] = millis();
            }
        }
        if (keystates[4] == HOLD) {
            if (millis() > lastAutoRepeat[3] + autoRepeatDelay) {
                G_key_up();
                lastAutoRepeat[3] = millis();
            }
        }

        // if the switch nr 6 is held for long enough the game will be reset
        if (keystates[11] == 6) {
            // reseting all game states (score, frame buffer, ...)
            resetGame();
        }

        // if the game paused switch is in the on position, we pause the game
        if (keystates[12] == PRESSED) {
            gamePaused = true;
            G_Pause();
        }
    } else {
        // if the game is not paused and the game paused switch is off, we activate the game again
        if (keystates[12] == RELEASED) {
            gamePaused = false;
            G_Continue();
        }
    }
}

// on initializing the game, we reset everything to the defaults
void D_I_initialize() {
    // clearing the display and setting it up with the custom chars in the first 4 chars of each line for rendering
    // this will make a layout like this: 

    // 0123____________ //
    // 4567____________ //
    
    // where the numbers indicate the character code in decimal for the character. 
    // This way, if a custom character is written to char code 0, it will be rendered in the top left.
    lcd.clear();
    for (uint8_t i = 0; i < 8; i++) {
        lcd.setCursor(i % 4, i / 4);
        lcd.write((uint8_t)i);
    }

    // set the seed of the rng to the current system time in ms
    srand(millis());
}