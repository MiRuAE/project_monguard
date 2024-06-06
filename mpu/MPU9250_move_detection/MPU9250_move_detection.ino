#include <Servo.h>

// 서보모터 핀 설정
Servo servoLeft;
Servo servoRight;

const int leftServoPin = 9;
const int rightServoPin = 10;

void setup() {
  // 서보모터 초기화
  servoLeft.attach(leftServoPin);
  servoRight.attach(rightServoPin);

  // 시리얼 통신 초기화
  Serial.begin(9600);
  Serial.println("Enter position (0-180) for Left Servo and Right Servo separated by space:");
}

void loop() {
  // 시리얼 입력이 있는 경우 처리
  if (Serial.available() > 0) {
    // 시리얼로부터 입력된 문자열 읽기
    String input = Serial.readStringUntil('\n');
    
    // 공백을 기준으로 문자열 분리
    int spaceIndex = input.indexOf(' ');
    if (spaceIndex != -1) {
      String leftPosStr = input.substring(0, spaceIndex);
      String rightPosStr = input.substring(spaceIndex + 1);

      // 문자열을 정수로 변환
      int leftPos = leftPosStr.toInt();
      int rightPos = rightPosStr.toInt();

      // 각도가 유효한 범위인지 확인
      if (leftPos >= 0 && leftPos <= 180 && rightPos >= 0 && rightPos <= 180) {
        // 서보모터 위치 설정
        servoLeft.write(leftPos);
        servoRight.write(rightPos);
        Serial.print("Left Servo Position: ");
        Serial.println(leftPos);
        Serial.print("Right Servo Position: ");
        Serial.println(rightPos);
      } else {
        Serial.println("Invalid position. Please enter values between 0 and 180.");
      }
    } else {
      Serial.println("Invalid input. Please enter two numbers separated by space.");
    }
  }
}
