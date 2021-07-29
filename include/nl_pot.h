/*
This file contains functions to read and correct for the non linear pots
used in the E-tonator
*/


#include <Arduino.h>

// defines the pins that the pots are connected to
#define POT_PIN_OUTER A7
#define POT_PIN_INNER A6

// Function to read nonlinear pot and
// rufly correct it to be linear
// returns value from 0 to 255
uint8_t nl_pot_read(uint8_t pot_pin);