
#include <Arduino.h>
#include <stdint.h>
#include <stdlib.h>


#define D_ROWS 20			// Number of rows there are in the tetris game (Should not be changed)
#define D_COLS 16			// Number of cols there are in the tetris game (Should not be changed)

#define G_LEFT -1
#define G_RIGHT 1


// === engine links === //

// function to be called when a frame has to be rendered (set this to the render function of your implementation
// of define a rendering function with the following signature and name in your code:
// void D_drawFrame(uint16_t frame[D_ROWS], uint8_t row, uint8_t col, uint16_t score, uint16_t rows_cleared)
extern void D_drawFrame(uint16_t frame[D_ROWS], uint8_t row, uint8_t col, uint16_t score, uint16_t rows_cleared);
#define G_RenderFrame(frame, row, col, score, rows) D_drawFrame(frame, row, col, score, rows)

// random number macros 
// replace with the corresponding rng functions of your implementation
// e.g: AVR Libc has a random() function wich may simplify some parts of the random number generation
#define G_rand_block_init_pos() (rand() % 11 + 1)								// function to return random integer from 1 to 11
#define G_randint(min, max) (rand() % (max - min) + min)						// function to return random integer from min to max
#define G_rand_sample(type, list) (list[rand() % (sizeof(list)/sizeof(type))])	// function to return a random entry of an array (only the rng function needs to be changed here)



// input functions
// these functions have to be called by the implementation or engine

void G_on_game_tick();	// a game tick should be envoked about every second. a tetris block automatically lowers on every game tick
void G_ResetGame();     // this function resets everything to the default state
// these functions handle user input
void G_key_up();		// used for rotation
void G_key_down();		// used for move down
void G_key_left();		// used for move left
void G_key_right();		// used for move right