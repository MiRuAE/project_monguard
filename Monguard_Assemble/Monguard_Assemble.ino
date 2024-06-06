#include <Arduino.h>
#include "MotorControl.h"
#include "BluetoothControl.h"
#include "faceControl.h"

#define RX_PIN 12
#define TX_PIN 13

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
          setFace(normalEyes, flatMouth); // ㅡ.ㅡ
          break;
        case 1:
          setFace(squintEyes, openMouth); // >..<
          break;
        case 2:
          setFace(normalEyes, smileMouth); // ^__^
          break;
        case 3:
          setFace(surprisedEyes, openMouth); // O_O
          break;
        case 4:
          winkFace(); // 윙크 표정
          break;
      }
    }

    // 모터 제어 함수 호출
    motorControl.setSpeed(1, V_Left); // 좌측 모터 속도 설정
    motorControl.setSpeed(2, V_Right); // 우측 모터 속도 설정
    motorControl.setDirection(1, dir); // 좌측 모터 방향 설정
    motorControl.setDirection(2, dir); // 우측 모터 방향 설정
  }
}
