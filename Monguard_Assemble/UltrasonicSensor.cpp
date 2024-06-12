#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(int trigPin, int echoPin) 
  : _trigPin(trigPin), _echoPin(echoPin), _previousMillis(0), _isMeasuring(false) {}

void UltrasonicSensor::begin() {
  pinMode(_trigPin, OUTPUT);
  pinMode(_echoPin, INPUT);
}

double UltrasonicSensor::measureDistanceCm() {
  if (!_isMeasuring) {
    _isMeasuring = true;
    _startMillis = millis();

    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2); // 최소 2us LOW 유지
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trigPin, LOW);
  }

  unsigned long currentMillis = millis();
  if (_isMeasuring && (currentMillis - _startMillis > 60)) { // 최대 측정 시간 제한 (약 30cm)
    _isMeasuring = false;
    return -1; // Timeout 발생
  }

  if (digitalRead(_echoPin) == HIGH) {
    _echoStartTime = micros();
  } else if (_echoStartTime != 0) { // Echo HIGH -> LOW 감지
    _isMeasuring = false;
    double duration = micros() - _echoStartTime;
    return (duration / 2) * 0.0343; // 거리 계산 (cm)
  }
  
  return -1; // 아직 측정 중
}

bool UltrasonicSensor::isObstacleDetected(double threshold) {
  double distance = measureDistanceCm();
  if (distance != -1) {
    return distance <= threshold;
  }
  return false;
}
