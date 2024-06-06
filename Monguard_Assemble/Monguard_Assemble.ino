#include <Arduino.h>
#include "MotorControl.h"
#include "BluetoothControl.h"
#include "faceControl.h"
/*
#define NEUTRAL1 3925 // Calibrated neutral value for servo 1 (Left) (green, blue, orange) (in microseconds)
#define NEUTRAL2 2375 // Calibrated neutral value for servo 2 (Right) (black, red, orange) (in microseconds)
<<<<<<< HEAD
#define ANGLE_RANGE 300// Maximum deviation from neutral
*/
//=======
#include "MotorControl.h"
#include "BluetoothControl.h"
#include "faceControl.h"
//>>>>>>> 469c445ca884ca7cf1e5c0664834c3b258adb4aa
=======
#define ANGLE_RANGE 300 // Maximum deviation from neutral
>>>>>>> cd879afc0c03ae7f5d50ab9c7a3f15c921545232

#define RX_PIN 12
#define TX_PIN 13
#define DIN 2
#define CS 3
#define CLK 4
#define NUM_MATRICES 4
#define NUMBER_OF_ROWS 8

BluetoothControl bluetoothControl(RX_PIN, TX_PIN); // BluetoothControl 객체 생성
MotorControl motorControl; // MotorControl 객체 생성
faceControl face(DIN, CS, CLK, NUM_MATRICES); // faceControl 객체 생성

void setup() {
  Serial.begin(9600);
  bluetoothControl.begin(9600);

  // Configure Timer/Counter1 for Fast PWM mode
  cli(); // Disable global interrupts
  DDRB |= (1 << PB1) | (1 << PB2); // Set OC1A (PB1) and OC1B (PB2) as output

  // Configure Timer/Counter1
  TCCR1A = (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1); // Fast PWM, 10-bit
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Fast PWM, Prescaler 8

  ICR1 = 40000; // Top value for 50Hz PWM (20ms period)

  sei(); // Enable global interrupts

  position_set();
  
  Serial.println("Bluetooth communication initialized.");
  face.begin();
  motorControl.init(); // 모터 제어 라이브러리 초기화

  face.setFace("squint");
<<<<<<< HEAD
//=======
// BluetoothControl 객체 생성
BluetoothControl bluetoothControl(RX_PIN, TX_PIN);
// MotorControl 객체 생성
MotorControl motorControl;

void setup() {
  Serial.begin(9600);
  bluetoothControl.begin(9600);
  Serial.println("Bluetooth communication initialized.");
  initMatrices();
  Serial.println("Matrix initialized");
  motorControl.init(); // 모터 제어 라이브러리 초기화
  randomSeed(analogRead(0)); // 랜덤 시드 초기화
>>>>>>> 469c445ca884ca7cf1e5c0664834c3b258adb4aa
=======
>>>>>>> cd879afc0c03ae7f5d50ab9c7a3f15c921545232
}

void loop() {
  DataPacket receivedPacket; // 데이터를 받을 패킷 구조체 생성

  // 블루투스로부터 데이터를 읽음
  if (bluetoothControl.readData(receivedPacket)) {
    // 데이터를 성공적으로 읽었을 때만 아래 코드 실행

    // 데이터 패킷에서 정보 추출
    char dir = receivedPacket.dir;
    int V_Left = receivedPacket.V_Left;
    int V_Right = receivedPacket.V_Right;
    char buttonA = receivedPacket.buttons[0];
    char buttonB = receivedPacket.buttons[1];
    char buttonC = receivedPacket.buttons[2];
    char buttonD = receivedPacket.buttons[3];
    char buttonE = receivedPacket.buttons[4];

    // 읽은 데이터 출력
    Serial.print("Received Dir: ");
    Serial.print(dir);
    Serial.print(" V_Left: ");
    Serial.print(V_Left);
    Serial.print(" V_Right: ");
    Serial.print(V_Right);
    Serial.print(" Buttons: ");
    Serial.print(buttonA);
    Serial.print(buttonB);
    Serial.print(buttonC);
    Serial.print(buttonD);
    Serial.print(buttonE);
    Serial.println();

    // 버튼 B가 눌렸을 때 얼굴 표정을 랜덤으로 변경
    if (buttonB == 'B') { // 버튼 B가 눌린 상태
      int randomFace = random(5); // 0부터 4까지 랜덤 숫자 생성 (표정 5개)
      switch (randomFace) {
        case 0:
          face.setFace("normal");
          break;
        case 1:
          face.setFace("squint");
          break;
        case 2:
          face.setFace("smile");
          break;
        case 3:
          face.setFace("surprised");
          break;
        case 4:
          face.setFace("wink");
          break;
      }
    }

    // 모터 제어 함수 호출
    motorControl.setSpeed(1, V_Left); // 좌측 모터 속도 설정
    motorControl.setSpeed(2, V_Right); // 우측 모터 속도 설정
    motorControl.setDirection(1, dir); // 좌측 모터 방향 설정
    motorControl.setDirection(2, dir); // 우측 모터 방향 설정

    if (buttonA == 'A') {
      Serial.println("Button A Pressed");
      walkForward();
    }

    if (buttonC == 'C') {
      Serial.println("Button C Pressed");
      walkBackward();
    }
  }
}

void position_set() {
  OCR1A = NEUTRAL1;
  OCR1B = NEUTRAL2;
}

void walkForward() {
  OCR1A = NEUTRAL1 + ANGLE_RANGE; // Move to max forward position
  OCR1B = NEUTRAL2 - ANGLE_RANGE;
}

void walkBackward() {
  OCR1A = NEUTRAL1 - ANGLE_RANGE; // Move to max backward position
  OCR1B = NEUTRAL2 + ANGLE_RANGE;
}
