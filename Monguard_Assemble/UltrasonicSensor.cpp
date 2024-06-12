#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(int trigPin, int echoPin) 
  : _trigPin(trigPin), _echoPin(echoPin), _previousMillis(0) {}

void UltrasonicSensor::begin() {
  pinMode(_trigPin, OUTPUT);
  pinMode(_echoPin, INPUT);
}

double UltrasonicSensor::measureDistanceCm() {
  unsigned long currentMillis = millis();
  if (currentMillis - _previousMillis >= _interval) {
    _previousMillis = currentMillis;

    digitalWrite(_trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trigPin, LOW);

    double duration = measurePulseDuration();
    if (duration > 0) {
      double cm = (duration / 2.0) * 0.0343;
      return cm;
    }
  }
  return -1; // Indicates that the measurement was not updated
}

unsigned long UltrasonicSensor::measurePulseDuration() {
  unsigned long start = 0;
  unsigned long end = 0;
  unsigned long timeout = 1000000L; // 1 second timeout
  unsigned long timeoutStart = micros();

  // Wait for the pulse to start
  while (digitalRead(_echoPin) == LOW) {
    if (micros() - timeoutStart > timeout) {
      return 0; // timeout
    }
  }
  start = micros();

  // Wait for the pulse to stop
  while (digitalRead(_echoPin) == HIGH) {
    if (micros() - timeoutStart > timeout) {
      return 0; // timeout
    }
  }
  end = micros();

  return end - start;
}

bool UltrasonicSensor::isObstacleDetected(double threshold) {
  double distance = measureDistanceCm();
  if (distance != -1) {
    return distance <= threshold;
  }
  return false;
}
