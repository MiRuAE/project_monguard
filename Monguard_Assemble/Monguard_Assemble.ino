#include <Arduino.h>
#include "MotorControl.h"
#include "ADCControl.h"

#define POT_PIN 0x05 // Potentiometer input (Analog PIN A5)

MotorControl motorControl;
ADCControl adcControl;

void setup() {
  Serial.begin(9600);
  motorControl.init();
  adcControl.init();
}

void loop() {
  int pot_value = adcControl.read(POT_PIN - A0);  
  int speed = map(pot_value, 0, 1023, 0, 255);
  bool forward = (pot_value > 512);

  // Motor 1 control
  motorControl.setDirection(1, forward);
  motorControl.setSpeed(1, speed);

  // Motor 2 control
  motorControl.setDirection(2, forward);
  motorControl.setSpeed(2, speed);

  Serial.print("Potentiometer: ");
  Serial.print(pot_value);
  Serial.print("  Speed: ");
  Serial.print(speed);
  Serial.print("  Direction: ");
  Serial.println(forward ? "Forward" : "Backward");
  delay(100); 
}
