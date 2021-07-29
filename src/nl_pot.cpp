/*
This file contains functions to read and correct for the non linear pots
used in the E-tonator
*/

#include "nl_pot.h"


// Function to read nonlinear pot and
// rufly correct it to be linear
// returns value from 0 to 255
uint8_t nl_pot_read(uint8_t pot_pin) {
    uint16_t potval = analogRead(pot_pin);   // read the value of the pot
    // map the pot value to the full 16 bit range and taking the square root to 
    // rufly account for the non linearity of the pot I am using.
    // with this there is still more precision in the lower range of the pot wich can be reversed by adding a second map and
    // sqrt wich seems to give you slightly higher precision in the higher ranges 
    // but I felt that it was better to have precision in lower ranges
    return sqrt(map(potval, 0, 1023, 65535, 0));
}