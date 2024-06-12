#include <Arduino.h>
#include "UltrasonicSensor.h"

#define TRIG_PIN 8
#define ECHO_PIN 7
#define OBSTACLE_THRESHOLD 10.0 // 장애물 감지 거리 임계값 (cm)

UltrasonicSensor ultrasonic(TRIG_PIN, ECHO_PIN);

void setup() {
  Serial.begin(9600);
  ultrasonic.begin();
}

void loop() {
  if (ultrasonic.isObstacleDetected(OBSTACLE_THRESHOLD)) {
    Serial.println("Obstacle detected! Stopping.");
    // 로봇 정지 코드 추가
  } else {
    Serial.println("No obstacle detected.");
    // 로봇 이동 코드 추가
  }

  // 100ms마다 거리를 측정하므로 loop()에서 더 짧은 간격으로 다른 작업을 수행 가능
  delay(100); // 주기적으로 거리 측정을 위해 간격을 둠
}
