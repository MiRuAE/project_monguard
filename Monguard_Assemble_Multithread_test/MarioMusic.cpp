#include "MarioMusic.h"

MarioMusic::MarioMusic() {
  // Set pin 11 as output using direct register manipulation
  DDRB |= (1 << DDB3); // Set PB3 (pin 11) as output
}

void MarioMusic::playMelody() {
  for (int thisNote = 0; thisNote < 98; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(11, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(11);
  }
  delay(15000);
}
