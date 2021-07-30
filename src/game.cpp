#include "game.h"

/*
This file contains all the code for tetris game logic and should not
be platform specific for Microsoft Visual C++ on windows.

to link the code with your rendering engine, simply set the rendering and random number macros in the game.h file
to point to your engines functions.

By in cluding the game.h file in your engine, you can call the functions for user and other input like game tick which is
to be handled by your implementation.

This file also contains all the tetris blocks as 2 dimensional uint8_t arrays called the blockstate.
The first dimension defines the rotation of the block and is always 4.
the second dimension defines how many lines the block is tall. 
For compatability it is also always 4, even for smaller blocks. the block should start in
the top-right-hand corner of the array (see below)

for the renderer to know how tall a block is, there is a second uint8_t variable, that defines
the height and the width of the block. since they have to be the values of the highest states,
a block is allways a square. the lenght of a side of this square is defined by this second variable called blocksize.

for convenience the two parameters are packed up in the struct "G_tetris_block"
*/
 struct G_tetris_block {
	uint8_t blockstate[4][4];
	uint8_t blocksize;
};

 // here a instance of the G_tetris_block is created for each type of block in the tetris game
G_tetris_block G_block_T = {
	{
		{
			0b0100,
			0b1110,
			0b0000,
			0b0000
		},
		{
			0b0100,
			0b0110,
			0b0100,
			0b0000
		},
		{
			0b0000,
			0b1110,
			0b0100,
			0b0000
		},
		{
			0b0100,
			0b1100,
			0b0100,
			0b0000
		}
	},
	3
};

G_tetris_block G_block_SQ = {
	{
		{
			0b1100,
			0b1100,
			0b0000,
			0b0000
		},
		{
			0b1100,
			0b1100,
			0b0000,
			0b0000
		},
		{
			0b1100,
			0b1100,
			0b0000,
			0b0000
		},
		{
			0b1100,
			0b1100,
			0b0000,
			0b0000
		}
	},
	2
};

G_tetris_block G_block_I = {
	{
		{
			0b0000,
			0b1111,
			0b0000,
			0b0000
		},
		{
			0b0100,
			0b0100,
			0b0100,
			0b0100
		},
		{
			0b0000,
			0b0000,
			0b1111,
			0b0000
		},
		{
			0b0100,
			0b0100,
			0b0100,
			0b0100
		}
	},
	4
};

G_tetris_block G_block_L1 = {
	{
		{
			0b1000,
			0b1110,
			0b0000,
			0b0000
		},
		{
			0b0110,
			0b0100,
			0b0100,
			0b0000
		},
		{
			0b0000,
			0b1110,
			0b0010,
			0b0000
		},
		{
			0b0100,
			0b0100,
			0b1100,
			0b0000
		}
	},
	3
};

G_tetris_block G_block_L2 = {
	{
		{
			0b0010,
			0b1110,
			0b0000,
			0b0000
		},
		{
			0b0100,
			0b0100,
			0b0110,
			0b0000
		},
		{
			0b0000,
			0b1110,
			0b1000,
			0b0000
		},
		{
			0b1100,
			0b0100,
			0b0100,
			0b0000
		}
	},
	3
};

G_tetris_block G_block_Z1 = {
	{
		{
			0b0110,
			0b1100,
			0b0000,
			0b0000
		},
		{
			0b0100,
			0b0110,
			0b0010,
			0b0000
		},
		{
			0b0000,
			0b0110,
			0b1100,
			0b0000
		},
		{
			0b1000,
			0b1100,
			0b0100,
			0b0000
		}
	},
	3
};

G_tetris_block G_block_Z2 = {
	{
		{
			0b1100,
			0b0110,
			0b0000,
			0b0000
		},
		{
			0b0010,
			0b0110,
			0b0100,
			0b0000
		},
		{
			0b0000,
			0b1100,
			0b0110,
			0b0000
		},
		{
			0b0100,
			0b1100,
			0b1000,
			0b0000
		}
	},
	3
};

// this list countains pointers to all the block types for easier use when randomly selecting one of them
// this way a random block can be chosen by creating a random array index
G_tetris_block* G_blocklist[7] = {
	&G_block_T,
	&G_block_SQ,
	&G_block_I,
	&G_block_L1,
	&G_block_L2,
	&G_block_Z1,
	&G_block_Z2
};

// this is further simplified by the G_rand_block marco which will randomly chose a block
#define G_rand_block() G_rand_sample(G_tetris_block*, G_blocklist)


// Flags to define the current state of the game
bool G_running = false;		// Flag to define wether the game is already running or not. usefull for things to be called only on the first game tick
bool G_paused = false;		// Flag to define wether the game is currently paused. If this is set, every input event will simply be ignored (includeing game-tick)

// Variables that describe the state of the currently active block
G_tetris_block* current_block = NULL;
uint8_t current_row = 1;
uint8_t current_col = 1;
uint8_t current_rot = 0;
uint8_t previous_row = 1;
uint8_t previous_col = 1;
uint8_t previous_rot = 0;


// statistic Variables
uint16_t G_score = 0;
uint16_t G_rows_cleared = 0;


// the frame buffer that everything is drawn to.
// the frame buffer starts out with a border of active tiles
// NOTE: Will only be shown to the screen after calling the G_RenderFrame() Marco
// The functio that will be called by this marco is defined in the game.h file and depends on
// the rendering engine implementation
uint16_t G_frame_buffer[D_ROWS]{
	0b1111111111111111,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1111111111111111,
};



// Function to render a block on the frame buffer
void G_renderBlock(G_tetris_block *block, uint8_t rot, uint8_t frame_row, uint8_t frame_col)
{
	for (uint8_t row = 0; row < block->blocksize; row++)	// for each row in a block
	{
		uint16_t tmprow = G_frame_buffer[frame_row + row] | ((uint16_t)(block->blockstate[rot][row]) << 12) >> frame_col; // overlay the block row on the corresponding framebuffer row by bitshifting the block row right by the designated amount of columns
		G_frame_buffer[frame_row + row] = tmprow;	// replace the old framebuffer row with the newly created one
	}
}

// Function to derender a block from the frame buffer
void G_derenderBlock(G_tetris_block* block, uint8_t rot, uint8_t frame_row, uint8_t frame_col)
{
	for (uint8_t row = 0; row < block->blocksize; row++) // for each row in a block
	{
		uint16_t tmprow = G_frame_buffer[frame_row + row] & ~(((uint16_t)(block->blockstate[rot][row]) << 12) >> frame_col);	// disable all active tiles using bitwise-and with the inverted block row state, bitshifting to the right as in G_renderBlock()
		G_frame_buffer[frame_row + row] = tmprow;	// replace old framebuffer row with the new one
	}
}


// Function to check if a block would collide with already drawn blocks or the side of the screen
// derender the block to check for collisions or all previous states of it before calling
// returns: false for not colliding, true for colliding
bool G_checkCollision(G_tetris_block* block, uint8_t rot, uint8_t frame_row, uint8_t frame_col)
{
	for (uint8_t row = 0; row < block->blocksize; row++)
	{
		// first we check if the block collides with other already existing blocks on the buffer.
		// for this to work, the block itself or all previous states of itself have to be derendered before calling this function.
		if (G_frame_buffer[frame_row + row] & ((uint16_t)(block->blockstate[rot][row]) << 12) >> frame_col)
		{
			Serial.println("Block colliding");
			return 1;	// returns 1 for colliding
		}
		/*
		// next we check if the block has reached the bottom of the screen
		if (frame_row + row >= D_ROWS && (uint16_t)(block->blockstate[rot][row]))	// if a block row that contains active tiles is outside the bottom of the screen
		{
			return 1;
		}
		// next we check if the block has reached the right side of the screen
		uint32_t check = 0;
		check = ((uint16_t)(block->blockstate[rot][row]) << 12+16) >> frame_col;	// put values on a 32 bit variable to check for all bits overflowing the 16 bit space
		if (check & 0x0000ffff)		// by performing binary and with zeros in the first 16 bits and ones in the second 16 bits only ones that have overflowed the 16 bit space will stay and if this value is true, meaning there are any bits left there, the block has collided with the right wall
		{
			return 1;
		}*/
	}
	return 0;
}

// Function to remove a given row from the frame buffer, shift all the above rows down by one and add a new, empty row at the top
void G_removeRow(uint8_t row)
{
	for (uint8_t i = row; i > 2; i--) // for each row from the given row up to the topmost row
	{
		G_frame_buffer[i] = G_frame_buffer[i - 1];	// override it's value with the value from the row above
	}
	G_frame_buffer[1] = 0b1000000000000001;	// set the first row to a new clear row
}

// Function to look for full rows and clear them
// returns the number of rows cleared
uint8_t G_clearRows()
{
	uint8_t rowsCleared = 0;	// counter for the cleared rows
	for (uint8_t row = 1; row < 19; row++) // for each row
	{
		if (G_frame_buffer[row] == 0xffff) // if every bit in the row is set
		{
			rowsCleared++;	// the row is full, we increment the rowsCleared counter
			G_removeRow(row); // and remove the full row
		}
	}
	return rowsCleared; // finaly, return the number of cleared rows
}

// this function spawns a new block at a random location at the top of the screen.
// returns true if ok, returns false if no block could be spawned because of collisions.
bool G_spawnBlock(G_tetris_block* block_type)
{
	// set the block state variables to new values
	current_block = block_type;		// set the given blocktype
	current_row = 1;				// the row has to be 1
	current_col = G_rand_block_init_pos();	// set the column to a random valid position given by the G_rand_block_init_pos() macro
	Serial.print("rand block pos: "); Serial.println(current_col);
	current_rot = 0;				// the rotation is always 0
	// the previous values have to be equal to the current values 
	previous_row = current_col;
	previous_col = 1;
	previous_rot = 0;

	// if the newly selected position is not colliding
	if (!G_checkCollision(current_block, current_rot, current_row, current_col))
	{
		G_renderBlock(current_block, current_rot, current_row, current_col);	// render the block
		return true;	// and return true for a successfull spawn
	}
	return false;	// otherwise return false
}




// block movement
// returns 1 if block could not move down and 0 it could move down
bool G_block_down()
{
	// first derender the old block
	G_derenderBlock(current_block, current_rot, current_row, current_col);
	previous_row = current_row;		// save current state to previous state
	current_row++;		// lower block position by one on the screen (corresponds to incrementing the row)
	if ((current_row > D_ROWS - 1) || G_checkCollision(current_block, current_rot, current_row, current_col))	// if the new block would be off the screen of would collide with other blocks on the screen
	{
		// return current row to old state
		current_row--;
		// and rerender the old block
		G_renderBlock(current_block, current_rot, current_row, current_col);
		// in this case, nothing has changed on the screen to it does not have to be updated
		// the down function returns 0 if it was not able to go down becaus of collisions
		return 0;	
	}
	else {
		// otherwise 
		// render the new block
		G_renderBlock(current_block, current_rot, current_row, current_col);
		// and update the screen
		G_RenderFrame(G_frame_buffer, current_row, current_col, G_score, G_rows_cleared);
	}
	// if the block could be moved down, 1 is returned 
	return 1;
}
void G_block_left()
{
	// first derender the old block
	G_derenderBlock(current_block, current_rot, current_row, current_col);
	previous_col = current_col;		// save current state to previous state
	current_col--;		// push block to the left (corresponds to decrementing the column)
	if ((current_col > D_COLS) || G_checkCollision(current_block, current_rot, current_row, current_col))
	{
		// return current col to old state
		current_col++;
		// and rerender the old block
		G_renderBlock(current_block, current_rot, current_row, current_col);
		// in this case, nothing has changed on the screen to it does not have to be updated
	}
	else
	{
		// otherwise
		// render the new block and update the screen
		G_renderBlock(current_block, current_rot, current_row, current_col);
		G_RenderFrame(G_frame_buffer, current_row, current_col, G_score, G_rows_cleared);
	}
}
void G_block_right()
{
	// first derender the old block
	G_derenderBlock(current_block, current_rot, current_row, current_col);
	previous_col = current_col;		// save current state to previous state
	current_col++;		// push block to the right (corresponds to incrementing the column)
	if ((current_col > D_COLS - 1) || G_checkCollision(current_block, current_rot, current_row, current_col))
	{
		// return current col to old state
		current_col--;
		// and rerender the old block
		G_renderBlock(current_block, current_rot, current_row, current_col);
		// in this case, nothing has changed on the screen to it does not have to be updated
	}
	else
	{
		// otherwise
		// render the new block and update the screen
		G_renderBlock(current_block, current_rot, current_row, current_col);
		G_RenderFrame(G_frame_buffer, current_row, current_col, G_score, G_rows_cleared);
	}
}
void G_block_rotate(int8_t direction)	// direction -1 turns block left, 1 turns block right (you can use G_LEFT and G_RIGHT constants)
{
	// first derender the old block
	G_derenderBlock(current_block, current_rot, current_row, current_col);
	previous_rot = current_rot;		// save current rotation to previous_rot
	current_rot += direction;		// change the block rotation by the amount in direction (can be positive or negative)
	
	if (current_rot > 3)	// if the rotation is bigger than 3 (-> 4) it should roll over and start with 0 again
	{
		current_rot = 0;
	}

	if (G_checkCollision(current_block, current_rot, current_row, current_col))	// if this rotation would collide with existing blocks
	{
		// return rotation to old state
		current_rot = previous_rot;
		// and rerender the old block
		G_renderBlock(current_block, current_rot, current_row, current_col);
		// in this case, nothing has changed on the screen to it does not have to be updated
		return;
	}
	

	// otherwise
	// render the new block and update the screen
	G_renderBlock(current_block, current_rot, current_row, current_col);
	G_RenderFrame(G_frame_buffer, current_row, current_col, G_score, G_rows_cleared);
}

// function to return the frame buffer contents to the original state
void G_resetFrameBuffer()
{
	// set the first and last row to all tiles active, just to make sure
	// even though they should never change
	G_frame_buffer[0] = 0xffff;
	G_frame_buffer[19] = 0xffff;
	// set the rest of the rows with only the first and last tile active
	for (uint8_t i = 1; i < 19; i++)
	{
		G_frame_buffer[i] = 0b1000000000000001;
	}
}

// controll event handlers
// can be called by the engine implementation
void G_Pause()
{
	G_paused = true;
}
void G_Continue()
{
	G_paused = false;
}
void G_ResetGame()
{
	// resets everything to the original state
	G_resetFrameBuffer();

	current_block = NULL;
	current_row = 1;
	current_col = 1;
	current_rot = 0;
	previous_row = 1;
	previous_col = 1;
	previous_rot = 0;

	G_running = false;
	G_paused = false;

	G_score = 0;
	G_rows_cleared = 0;
}

// input event handlers
// have to be called by the engine implementation
void G_on_game_tick()
{
	if (!G_paused)
	{
		Serial.println("received gametick");
		// if the game is not paused
		if (G_running)
		{
			// if the the game is currently running
			if (!G_block_down()) {	// try to move the currently active block down by one
				// if the block could not be moved because of a collision we clear all potentially full rows
				uint8_t rows = G_clearRows();
				// and spawn a new block
				if (G_spawnBlock(G_rand_block()))
				{
					// if the block could be spawned the socore will be handled
					// for every spawned block the score will be increased by 1
					// for every cleared row the score will be increased by 10
					// for every row more than one cleared at once, 2 will be added to the score
					G_score++;
					G_score += rows * 10;
					G_rows_cleared += rows;	// the G_rows_cleared statistic contains the number of rows cleared
					if (rows > 1) { G_score += 2 * (rows - 1);}

					G_RenderFrame(G_frame_buffer, current_row, current_col, G_score, G_rows_cleared);
				}
				else
				{
					// if no new block could be spawned because there was no space for it, the screen is filled up and 
					// the game is over (player has lost the game)
					// -- GAME OVER ROUTINE --
					G_ResetGame();
					G_RenderFrame(G_frame_buffer, 0, 0, 0, 0);
				}
			}
		}
		else
		{
			if (G_spawnBlock(/*G_rand_block()*/&G_block_T))
			{
				Serial.println("Initial Render");
				G_RenderFrame(G_frame_buffer, current_row, current_col, G_score, G_rows_cleared);
				G_running = true;
			}
		}
	}
}

void G_key_up()
{
	if (!G_paused)
	{
		if (G_running)
		{
			// if the game is not paused and is currently running
			// rotate the block
			G_block_rotate(G_RIGHT);
		}
	}
}
void G_key_down()
{
	if (!G_paused)
	{
		if (G_running)
		{
			// if the game is not paused and is currently running
			// move the block down
			G_block_down();
		}
	}
}
void G_key_left()
{
	if (!G_paused)
	{
		if (G_running)
		{
			// if the game is not paused and is currently running
			// move the block left
			G_block_left();
		}
	}
}
void G_key_right()
{
	if (!G_paused)
	{
		if (G_running)
		{
			// if the game is not paused and is currently running
			// move the block right
			G_block_right();
		}
	}
}