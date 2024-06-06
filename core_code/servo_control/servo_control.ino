#include "MyServoControl.h"

// MyServoControl 객체 생성
MyServoControl myServo;

void setup() {
  // 시리얼 통신 초기화 (디버깅용)
  Serial.begin(9600);

  // 서보 모터 초기화
  myServo.begin();
  Serial.println("Servo initialized");

  // 서보 모터 중립 위치로 설정
  myServo.positionSet();
  Serial.println("Servo set to neutral position");
  delay(2000); // 2초 대기
}

void loop() {
  // 전진 테스트
  myServo.walkForward();
  Serial.println("Servo moving forward");
  delay(2000); // 2초 대기

  // 중립 위치로 복귀
  myServo.positionSet();
  Serial.println("Servo set to neutral position");
  delay(2000); // 2초 대기

  // 후진 테스트
  myServo.walkBackward();
  Serial.println("Servo moving backward");
  delay(2000); // 2초 대기

  // 중립 위치로 복귀
  myServo.positionSet();
  Serial.println("Servo set to neutral position");
  delay(2000); // 2초 대기

  // 각도 증가 테스트
  myServo.increaseAngle(100);
  Serial.println("Servo angle increased by 100");
  delay(2000); // 2초 대기

  // 각도 감소 테스트
  myServo.decreaseAngle(100);
  Serial.println("Servo angle decreased by 100");
  delay(2000); // 2초 대기

  // 중립 위치로 복귀
  myServo.positionSet();
  Serial.println("Servo set to neutral position");
  delay(2000); // 2초 대기

  // 왼쪽 기울기 테스트
  myServo.tiltLeft(100);
  Serial.println("Servo tilted left by 100");
  delay(2000); // 2초 대기

  // 중립 위치로 복귀
  myServo.positionSet();
  Serial.println("Servo set to neutral position");
  delay(2000); // 2초 대기

  // 오른쪽 기울기 테스트
  myServo.tiltRight(100);
  Serial.println("Servo tilted right by 100");
  delay(2000); // 2초 대기

  // 중립 위치로 복귀
  myServo.positionSet();
  Serial.println("Servo set to neutral position");
  delay(2000); // 2초 대기

  // 상향/하향 기울기 테스트
  myServo.upDownTilt(200);
  Serial.println("Servo up/down tilted by 200");
  delay(2000); // 2초 대기

  // 중립 위치로 복귀
  myServo.positionSet();
  Serial.println("Servo set to neutral position");
  delay(2000); // 2초 대기
}
