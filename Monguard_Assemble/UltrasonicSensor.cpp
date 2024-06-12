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

<<<<<<< HEAD
    double duration = pulseIn(_echoPin, HIGH);
=======
    double duration = pulseInCustom(_echoPin, HIGH);
>>>>>>> aebe27f9fadd6d6131cd0d97ad2c729aa72d9a7d
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
<<<<<<< HEAD
=======

unsigned long UltrasonicSensor::pulseInCustom(uint8_t pin, uint8_t state, unsigned long timeout) {
  // 비트 마스크와 포트를 핀 7과 8에 대해 직접 설정
  uint8_t bit, port;
  if (pin == 7) {
    bit = _BV(PD7);  // Pin 7 is bit 7 on port D
    port = PIND;
  } else if (pin == 8) {
    bit = _BV(PB0);  // Pin 8 is bit 0 on port B
    port = PINB;
  } else {
    return 0;  // 지원되지 않는 핀에 대해 0을 반환
  }

  uint8_t stateMask = (state ? bit : 0);
  unsigned long width = 0;
  unsigned long numloops = 0;
  unsigned long maxloops = microsecondsToClockCycles(timeout) / 16;

  // Wait for any previous pulse to end
  while ((port & bit) == stateMask) {
    if (numloops++ == maxloops) {
      return 0;
    }
  }

  // Wait for the pulse to start
  while ((port & bit) != stateMask) {
    if (numloops++ == maxloops) {
      return 0;
    }
  }

  // Wait for the pulse to stop
  while ((port & bit) == stateMask) {
    if (numloops++ == maxloops) {
      return 0;
    }
    width++;
  }

  // Convert the reading to microseconds
  return clockCyclesToMicroseconds(width * 16);
}
>>>>>>> aebe27f9fadd6d6131cd0d97ad2c729aa72d9a7d
