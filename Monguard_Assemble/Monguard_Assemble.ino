#include "MonguardLib.h"

// Correct object declaration
Monguard myMonguard(13);

void setup() {
  // Setup the pin mode
  myMonguard.begin();
}

void loop() {
  // Use the Monguard object to send Morse code
  myMonguard.dot(); myMonguard.dot(); myMonguard.dot();
  myMonguard.dash(); myMonguard.dash(); myMonguard.dash();
  myMonguard.dot(); myMonguard.dot(); myMonguard.dot();
  myMonguard.add(3,4);
  delay(3000);
}
