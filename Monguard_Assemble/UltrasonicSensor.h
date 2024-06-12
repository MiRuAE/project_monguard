#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>

class UltrasonicSensor {
public:
  UltrasonicSensor(int trigPin, int echoPin);
  void begin();
  double measureDistanceCm();
  bool isObstacleDetected(double threshold);

private:
  int _trigPin;
  int _echoPin;
  unsigned long _previousMillis;
  const unsigned long _interval = 100; // 측정 간격 (ms)
  unsigned long pulseInCustom(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);
};

#endif // ULTRASONIC_SENSOR_H
