#include "MyServoControl.h"

MyServoControl::MyServoControl() : currentAngle1(NEUTRAL1), currentAngle2(NEUTRAL2) {}

void MyServoControl::begin() {
  // Serial.begin(9600);

  // Configure Timer/Counter1 for Fast PWM mode
  cli(); // Disable global interrupts
  DDRB |= (1 << PB1) | (1 << PB2); // Set OC1A (PB1) and OC1B (PB2) as output

  // Configure Timer/Counter1
  TCCR1A = (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1); // Fast PWM, 10-bit
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Fast PWM, Prescaler 8

  ICR1 = 40000; // Top value for 50Hz PWM (20ms period)

  sei(); // Enable global interrupts

  // Initialize the PWM values to neutral position
  OCR1A = NEUTRAL1; // Timer counts in microseconds
  OCR1B = NEUTRAL2; // Timer counts in microseconds
}

void MyServoControl::positionSet() {
  currentAngle1 = NEUTRAL1;
  currentAngle2 = NEUTRAL2;
  updateServo();
}

void MyServoControl::walkForward() {
  currentAngle1 = NEUTRAL1 + ANGLE_RANGE; // Move to max forward position
  currentAngle2 = NEUTRAL2 - ANGLE_RANGE;
  updateServo();
}

void MyServoControl::walkBackward() {
  currentAngle1 = NEUTRAL1 - ANGLE_RANGE; // Move to max backward position
  currentAngle2 = NEUTRAL2 + ANGLE_RANGE;
  updateServo();
}

void MyServoControl::increaseAngle(uint16_t value) {
  currentAngle1 = constrain(currentAngle1 + value, NEUTRAL1 - ANGLE_RANGE, NEUTRAL1 + ANGLE_RANGE);
  currentAngle2 = constrain(currentAngle2 + value, NEUTRAL2 - ANGLE_RANGE, NEUTRAL2 + ANGLE_RANGE);
  updateServo();
}

void MyServoControl::decreaseAngle(uint16_t value) {
  currentAngle1 = constrain(currentAngle1 - value, NEUTRAL1 - ANGLE_RANGE, NEUTRAL1 + ANGLE_RANGE);
  currentAngle2 = constrain(currentAngle2 - value, NEUTRAL2 - ANGLE_RANGE, NEUTRAL2 + ANGLE_RANGE);
  updateServo();
}

void MyServoControl::tiltRight(uint16_t value) {
  currentAngle1 = constrain(currentAngle1 + value, NEUTRAL1 - ANGLE_RANGE, NEUTRAL1 + ANGLE_RANGE);
  currentAngle2 = constrain(currentAngle2 + value, NEUTRAL2 - ANGLE_RANGE, NEUTRAL2 + ANGLE_RANGE);
  updateServo();
}

void MyServoControl::tiltLeft(uint16_t value) {
  currentAngle1 = constrain(currentAngle1 - value, NEUTRAL1 - ANGLE_RANGE, NEUTRAL1 + ANGLE_RANGE);
  currentAngle2 = constrain(currentAngle2 - value, NEUTRAL2 - ANGLE_RANGE, NEUTRAL2 + ANGLE_RANGE);
  updateServo();
}

void MyServoControl::upDownTilt(uint16_t value) {
  currentAngle1 = constrain(currentAngle1 + value, NEUTRAL1 - ANGLE_RANGE, NEUTRAL1 + ANGLE_RANGE);
  currentAngle2 = constrain(currentAngle2 - value, NEUTRAL2 - ANGLE_RANGE, NEUTRAL2 + ANGLE_RANGE);
  updateServo();
}

void MyServoControl::updateServo() {
  OCR1A = currentAngle1;
  OCR1B = currentAngle2;
}
