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

void MyServoControl::positionSet(uint16_t speed) {
  updateServo(NEUTRAL1, NEUTRAL2, speed);
}

void MyServoControl::walkForward(uint16_t speed) {
  updateServo(NEUTRAL1 + ANGLE_RANGE, NEUTRAL2 - ANGLE_RANGE, speed);
}

void MyServoControl::walkBackward(uint16_t speed) {
  updateServo(NEUTRAL1 - ANGLE_RANGE, NEUTRAL2 + ANGLE_RANGE, speed);
}

void MyServoControl::increaseAngle(uint16_t value, uint16_t speed) {
  updateServo(constrain(currentAngle1 + value, NEUTRAL1 - ANGLE_RANGE, NEUTRAL1 + ANGLE_RANGE),
              constrain(currentAngle2 + value, NEUTRAL2 - ANGLE_RANGE, NEUTRAL2 + ANGLE_RANGE),
              speed);
}

void MyServoControl::decreaseAngle(uint16_t value, uint16_t speed) {
  updateServo(constrain(currentAngle1 - value, NEUTRAL1 - ANGLE_RANGE, NEUTRAL1 + ANGLE_RANGE),
              constrain(currentAngle2 - value, NEUTRAL2 - ANGLE_RANGE, NEUTRAL2 + ANGLE_RANGE),
              speed);
}

void MyServoControl::tiltLeft(uint16_t value, uint16_t speed) {
  updateServo(constrain(currentAngle1 + value, NEUTRAL1 - ANGLE_RANGE, NEUTRAL1 + ANGLE_RANGE),
              constrain(currentAngle2 + value, NEUTRAL2 - ANGLE_RANGE, NEUTRAL2 + ANGLE_RANGE),
              speed);
}

void MyServoControl::tiltRight(uint16_t value, uint16_t speed) {
  updateServo(constrain(currentAngle1 - value, NEUTRAL1 - ANGLE_RANGE, NEUTRAL1 + ANGLE_RANGE),
              constrain(currentAngle2 - value, NEUTRAL2 - ANGLE_RANGE, NEUTRAL2 + ANGLE_RANGE),
              speed);
}

void MyServoControl::upDownTilt(uint16_t value, uint16_t speed) {
  updateServo(constrain(currentAngle1 + value, NEUTRAL1 - ANGLE_RANGE, NEUTRAL1 + ANGLE_RANGE),
              constrain(currentAngle2 - value, NEUTRAL2 - ANGLE_RANGE, NEUTRAL2 + ANGLE_RANGE),
              speed);
}

void MyServoControl::updateServo(uint16_t targetAngle1, uint16_t targetAngle2, uint16_t speed) {
  int step1 = (targetAngle1 > currentAngle1) ? 1 : -1;
  int step2 = (targetAngle2 > currentAngle2) ? 1 : -1;

  while (currentAngle1 != targetAngle1 || currentAngle2 != targetAngle2) {
    if (currentAngle1 != targetAngle1) {
      currentAngle1 += step1;
    }
    if (currentAngle2 != targetAngle2) {
      currentAngle2 += step2;
    }

    OCR1A = currentAngle1;
    OCR1B = currentAngle2;

    delay(speed);
  }
}
