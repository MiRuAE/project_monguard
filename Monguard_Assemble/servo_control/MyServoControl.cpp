#include "MyServoControl.h"

MyServoControl::MyServoControl() {}

void MyServoControl::begin() {
//  Serial.begin(9600);

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
  OCR1A = NEUTRAL1;
  OCR1B = NEUTRAL2;
}

void MyServoControl::walkForward() {
  OCR1A = NEUTRAL1 + ANGLE_RANGE; // Move to max forward position
  OCR1B = NEUTRAL2 - ANGLE_RANGE;
}

void MyServoControl::walkBackward() {
  OCR1A = NEUTRAL1 - ANGLE_RANGE; // Move to max backward position
  OCR1B = NEUTRAL2 + ANGLE_RANGE;
}
/*
void MyServoControl::rightTilt(int xValue) {
  int offset = (ANGLE_RANGE * (504 - xValue)) / 504 / 39 * 300; // Calculate based on xValue
  Serial.println(offset);
  OCR1A = NEUTRAL1 + offset;
  OCR1B = NEUTRAL2 + offset;
}

void MyServoControl::leftTilt(int xValue) {
  int offset = (ANGLE_RANGE * (xValue - 504)) / 504 / 39 * 300; // Calculate based on xValue
  OCR1A = NEUTRAL1 - offset;
  OCR1B = NEUTRAL2 - offset;
}

void MyServoControl::handleXValue(int xValue) {
  if (xValue < 504) {
    rightTilt(xValue);
  } else if (xValue > 504) {
    leftTilt(xValue);
  } else {
    OCR1A = NEUTRAL1;
    OCR1B = NEUTRAL2;
  }
} 

void MyServoControl::handleButtonPress(char button) {
  if (button == 'A') {
    walkForward();
  } else if (button == 'C') {
    walkBackward();
  }
}*/
