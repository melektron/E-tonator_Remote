#include "sound.h"


/*
In order to reduce memory and storage useage, the melody is split into treble_melody and bass_melody 
(same goes for durations). In the original code, the entire song was built from one large block of notes.
The song played the treble theme twice, then the bass part once and then the treble theme twice again and
the bass part once. 
This code is supposed to run on an ATmega328p with 2k of RAM, and the notes took up a significant amount (over 50%)
of system memory. As the rest of this programm already used about 30% of ram, it came in at about 90% ram usage
thus limiting my ability to expand and add features (at the time there were only debug mode, normal mode and music).
(The notes cannot be stored in PROGMEM as it caused weired artifacts)

As shown by the explaination above, the entire identical music piece was played twice and was also stored twice in ram.
Since I am playing the song in a loop, the memory usage could be halfed by just removing the seconf half of the song, which
worked prefectly. Now only the following information is stored:
    * Main theme
    * Main theme
    * Bass part
As you can see, the main theme was still stored twice, so in order to fix this issue, I split up the song into two arrays
and simply played the main theme (treble_melody) twice before the bass part. this way the memory usage was reduced from about
1k to just 320 bytes. (still a lot for this chip but sill leaves over 50% of ram free for expansion)
*/


// Melody
uint16_t treble_melody[64] = {
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, 
  NOTE_B4, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5,
  NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_B2, 
  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5, 
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5, 
  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, NOTE_E3, NOTE_A4, 0,
};

uint16_t bass_melody[64] =  {
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_A3, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_GS3, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3, 
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_E4, NOTE_E3, NOTE_A4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_GS4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3,
};

//note durations: 4 = quarter note, 8 = eighth note, etc
uint8_t treble_durations[64] = {
  8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,4,8,8,16,16,8,8,8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,4,
};

uint8_t bass_durations[64] = {
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
};




uint16_t thisNote = 0; 
unsigned long delay_start_time = 0;
bool delay_active = 0;
uint16_t delay_time = 0;
bool music_active = false;
uint16_t music_speed = 1000; 


// void feedback_click() {
//   digitalWrite(MUSIC_PIN, HIGH);
//   delay(50);
//   digitalWrite(MUSIC_PIN, LOW);
// }

void music_BackToStart() {
  thisNote = 0;
  delay_start_time = 0;
  delay_active = 0;
  delay_time = 0;
}

void music_Play() {
  music_active = true;
}

void music_Pause() {
  music_active = false;
  // when the music gets paused all the timer variables will be reset as the times stored there are
  // no longer of any meaning.
  // It would probably work without doing this but it feels cleaner
  delay_start_time = 0;
  delay_active = 0;
  delay_time = 0;

  digitalWrite(MUSIC_PIN, LOW);
}

void music_set_speed(uint16_t speed) {
  music_speed = map(speed, 0, 100, 5000, 500);
}

void music_LoopFunction() {
  // only play music if the music is active
  if (music_active) {
      // non-blocking delay Function
    if (!delay_active) {
      // code to run befor delay
      /*
        to calculate the note duration, take one second divided by the note type.
        e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      */
      // depending on how far we are into the track, we play notes from different sub-tracks
      uint16_t currentNote;
      uint16_t noteDuration;
      if (thisNote < 64) {
        currentNote = treble_melody[thisNote];
        noteDuration = music_speed/treble_durations[thisNote];
      } else if (thisNote < 128) {
        currentNote = treble_melody[thisNote - 64];
        noteDuration = music_speed/treble_durations[thisNote - 64];
      } else {
        currentNote = bass_melody[thisNote - 128];
        noteDuration = music_speed/bass_durations[thisNote - 128];
      }
      tone(MUSIC_PIN, currentNote, noteDuration);
      
      /*
        to distinguish the notes, set a minimum time between them.
        the note's duration + 30% seems to work well:
      */
      delay_time = noteDuration * 1.30;

      // activate the nonblocking delay
      delay_active = true;
      delay_start_time = millis();
    } else {
      if (millis() >= delay_start_time + delay_time) {
        // deactivate the delay
        delay_active = false;
        // code to extcute when delay is finished
        noTone(MUSIC_PIN); //stop the tone playing:
        thisNote++;
      }
    }
    // if we have reached the end of the track we start at the beginning
    if (thisNote >= NR_NOTES){
      thisNote = 0;
    }
  }
}