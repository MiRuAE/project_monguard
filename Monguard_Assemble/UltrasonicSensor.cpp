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

    double duration = pulseIn(_echoPin, HIGH);
    double cm = (duration / 2) * 0.0343;
    return cm;
  }
  return -1; // Indicates that the measurement was not updated
}

bool UltrasonicSensor::isObstacleDetected(double threshold) {
  double distance = measureDistanceCm();
  if (distance != -1) {
    return distance <= threshold;
  }
  return false;
}