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

    // 얼굴 표정 부분
    setFace(normalEyes, flatMouth);    // ㅡ.ㅡ
    delay(2000);
    setFace(squintEyes, openMouth);    // >..<
    delay(2000);
    setFace(normalEyes, smileMouth); // ^__^
    delay(2000);
    setFace(surprisedEyes, openMouth); // O_O
    delay(2000);
    winkFace();  // 윙크 표정
    delay(2000);

    // 모터 제어 함수 호출
    motorControl.setSpeed(1, V_Left); // 좌측 모터 속도 설정
    motorControl.setSpeed(2, V_Right); // 우측 모터 속도 설정
    motorControl.setDirection(1, dir); // 좌측 모터 방향 설정
    motorControl.setDirection(2, dir); // 우측 모터 방향 설정
  }
}
