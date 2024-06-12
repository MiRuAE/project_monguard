#include "MyMusic.h"

MyMusic::MyMusic(int pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
  _melodyIndex = 0;
  _previousMillis = 0;
}

void MyMusic::playMelody() {
  startMelody();
}

void MyMusic::startMelody() {
  _melodyIndex = 0;
  _previousMillis = millis();
}

void MyMusic::update() {
  if (_melodyIndex >= 98) {
    noTone(_pin);
    return;
  }

  unsigned long currentMillis = millis();

  if (currentMillis - _previousMillis >= (1000 / noteDurations[_melodyIndex]) * 1.30) {
    _previousMillis = currentMillis;

    int noteDuration = 1000 / noteDurations[_melodyIndex];
    tone(_pin, melody[_melodyIndex], noteDuration);
    _melodyIndex++;
  }
}
