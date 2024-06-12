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
  unsigned long _interval = 100; // 측정 간격 (ms)

  // 추가된 멤버 변수
  bool _isMeasuring;        // 측정 상태
  unsigned long _startMillis; // 측정 시작 시간
  unsigned long _echoStartTime; // 에코 HIGH 시작 시간
};

#endif
