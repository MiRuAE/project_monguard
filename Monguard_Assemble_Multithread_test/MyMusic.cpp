#include "MyMusic.h"

MyMusic::MyMusic(int pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
}

void MyMusic::playMelody(const int melody[], const int noteDurations[], int length) {
  for (int thisNote = 0; thisNote < length; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(_pin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(_pin);
  }
  delay(1200);
}

void MyMusic::playSmileMelody() {
  playMelody(smileMelody, smileNoteDurations, 7);
  delay(1000);
}

void MyMusic::playCryMelody() {
  playMelody(cryMelody, cryNoteDurations, 10);
  delay(1000);
}
