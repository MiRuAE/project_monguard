#include <Arduino.h>
#include "MotorControl.h"
#include "BluetoothControl.h"
#include "faceControl.h"
#include "MyServoControl.h"
#include "MyMusic.h"
#include "MPU9250Library.h"

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
MyServoControl myServo;
MPU9250Library mpuSensor;
int count = 0; //sleep mode용 카운트

void setup() {
  Serial.begin(9600);
  bluetoothControl.begin(9600);
  Serial.println("Bluetooth communication initialized.");

  face.begin();
  motorControl.init(); // 모터 제어 라이브러리 초기화
  myServo.begin();

  face.setFace("squint");
  myServo.positionSet(10);

  mpuSensor.begin();

}

void loop() {
  DataPacket receivedPacket; // 데이터를 받을 패킷 구조체 생성

  // 블루투스로부터 데이터를 읽음
  if (bluetoothControl.readData(receivedPacket)) {
    // 데이터를 성공적으로 읽었을 때만 아래 코드 실행

    // 데이터 패킷에서 정보 추출
    char dir_FB = receivedPacket.DIR_FB;
    char dir_LR = receivedPacket.DIR_LR;
    int V_Left = receivedPacket.V_Left;
    int V_Right = receivedPacket.V_Right;
    char buttonA = receivedPacket.buttons[0];
    char buttonB = receivedPacket.buttons[1];
    char buttonC = receivedPacket.buttons[2];
    char buttonD = receivedPacket.buttons[3];
    char buttonE = receivedPacket.buttons[4];

    // 읽은 데이터 출력
    //Serial.print("Received Dir: ");
    Serial.print(dir_FB);
    Serial.print(dir_LR);
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
    Serial.print(count);
    Serial.println();
    
    // 버튼 B가 눌렸을 때 얼굴 표정을 랜덤으로 변경
    if (buttonB == 'B') { // 버튼 B가 눌린 상태
      int randomFace = random(6); // 0부터 4까지 랜덤 숫자 생성 (표정 5개)
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
        case 5:
          face.setFace("angry");
          break;
      }
    }

    // 모터 제어 함수 호출
    motorControl.setSpeed(1, V_Left); // 좌측 모터 속도 설정
    motorControl.setSpeed(2, V_Right); // 우측 모터 속도 설정
    motorControl.setDirection(1, dir_FB); // 좌측 모터 방향 설정
    motorControl.setDirection(2, dir_FB); // 우측 모터 방향 설정
    
    if (V_Left>V_Right){ //좌측으로 갈때 좌측 틸팅
      if (0 <= V_Left - V_Right && V_Left - V_Right <= 51) {
        myServo.tiltLeft(60, 1);
      }
      if (52 <= V_Left - V_Right && V_Left - V_Right <= 103) {
        myServo.tiltLeft(120, 1);
      }
      if (104 <= V_Left - V_Right && V_Left - V_Right <= 155) {
        myServo.tiltLeft(180, 1);
      }
      if (156 <= V_Left - V_Right && V_Left - V_Right <= 207) {
        myServo.tiltLeft(240, 1);
      }
      else {
        myServo.tiltLeft(300, 1);
      }
    }

    if (V_Left <V_Right){ //우측으로 갈때 우측 틸팅
      if (0 <= V_Right - V_Left && V_Right - V_Left <= 51) {
        myServo.tiltRight(60, 1);
      }
      if (52 <= V_Right - V_Left && V_Right - V_Left <= 103) {
        myServo.tiltRight(120, 1);
      }
      if (104 <= V_Right - V_Left && V_Right - V_Left <= 155) {
        myServo.tiltRight(180, 1);
      }
      if (156 <= V_Right - V_Left && V_Right - V_Left <= 207) {
        myServo.tiltRight(240, 1);
      }
      else {
        myServo.tiltRight(300, 1);
      }
    }

    if (V_Left <V_Right){ //우측으로 갈때 우측 틸팅
      if (0 <= V_Right - V_Left && V_Right - V_Left <= 51) {
        myServo.tiltRight(60, 1);
      }
      if (52 <= V_Right - V_Left && V_Right - V_Left <= 103) {
        myServo.tiltRight(120, 1);
      }
      if (104 <= V_Right - V_Left && V_Right - V_Left <= 155) {
        myServo.tiltRight(180, 1);
      }
      if (156 <= V_Right - V_Left && V_Right - V_Left <= 207) {
        myServo.tiltRight(240, 1);
      }
      else {
        myServo.tiltRight(300, 1);
      }
    }

    if (dir_FB == 'N' && dir_LR == 'N'){ //sleep 모드 활성화
      count += 1;
      delay(1000);
      if (count == 100){
        count = 0;
        mpuSensor.update();
        face.setFace("normal");
        if (mpuSensor.isThresholdExceeded()){
          Serial.print("aaaaaaaa");
          return 0;
        }
      else {
        return 0;
      }
      }
    else {
      return 0;
    }
    }

    if (buttonA == 'A') {
      myServo.walkForward(5);
    }
    
    if (buttonC == 'C') {
      myServo.walkBackward(5);
    }

    if (buttonD == 'D') {
      myServo.positionSet(5);
    }
  }
}
