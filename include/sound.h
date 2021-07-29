#include <Arduino.h>
#include "pitches.h"

// definitions
#define MUSIC_PIN 10        // pin to output the sound to
#define NR_NOTES 192        // number of total notes in one loop of the track (2x treble + 1x bass)



// functions
// void feedback_click();

void music_LoopFunction();
void music_Pause();
void music_Play();
void music_BackToStart();
void music_set_speed(uint16_t speed);