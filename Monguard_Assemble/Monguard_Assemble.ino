#include <Arduino.h>
#include "MotorControl.h"
#include "BluetoothControl.h"
#include "faceControl.h"
#include "MyServoControl.h"
//#include "MyMusic.h"
#include "MPU9250Library.h"
#include "UltrasonicSensor.h"

#include "MarioMusic.h"

//MarioMusic marioMusic;

#define RX_PIN 12
#define TX_PIN 13
#define DIN 2
#define CS 3
#define CLK 4
#define NUM_MATRICES 4
#define NUMBER_OF_ROWS 8
#define pinTrig 8
#define pinEcho 7
#define BUZZER_PIN 11

int mode_count = 0;

BluetoothControl bluetoothControl(RX_PIN, TX_PIN); // BluetoothControl 객체 생성
MotorControl motorControl; // MotorControl 객체 생성
faceControl face(DIN, CS, CLK, NUM_MATRICES); // faceControl 객체 생성
UltrasonicSensor sensor(pinTrig, pinEcho); // 초음파 센서
MyServoControl myServo;
MPU9250Library mpuSensor;

//MyMusic music(BUZZER_PIN); //스피커


void setup() {
  Serial.begin(9600);
  bluetoothControl.begin(9600);
  Serial.println("Bluetooth communication initialized.");

  face.begin();
  motorControl.init(); // 모터 제어 라이브러리 초기화
  myServo.begin();

  face.setFace("squint");
  myServo.positionSet(10);

  mpuSensor.begin(); //mpu9250 시작
  //sensor.begin(); //초음파센서 시작

}

void loop() {

  DataPacket receivedPacket; // 데이터를 받을 패킷 구조체 생성

  // 블루투스로부터 데이터를 읽음
  if (bluetoothControl.readData(receivedPacket)) {
    // 데이터를 성공적으로 읽었을 때만 아래 코드 실행

    // 데이터 패킷에서 정보 추출
    char dir_FB = receivedPacket.DIR_FB;
    char dir_LR = receivedPacket.DIR_LR;
    char Mode = receivedPacket.Mode;
    int V_Left = receivedPacket.V_Left;
    int V_Right = receivedPacket.V_Right;
    char buttonA = receivedPacket.buttons[0];
    char buttonB = receivedPacket.buttons[1];
    char buttonC = receivedPacket.buttons[2];
    char buttonD = receivedPacket.buttons[3];
    char buttonE = receivedPacket.buttons[4];

    double distance;

    // 읽은 데이터 출력
    Serial.print("Received Dir: ");
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
    // Serial.print(" Mode: ");
    // Serial.print(Mode);
    // Serial.print(" Distance: ");
    // Serial.print(distance);
    //Serial.print(" Count: ");
    Serial.print(mode_count);
    Serial.println();
    

    // 모터 제어 함수 호출
    motorControl.setSpeed(1, V_Left); // 좌측 모터 속도 설정
    motorControl.setSpeed(2, V_Right); // 우측 모터 속도 설정
    motorControl.setDirection(1, dir_FB); // 좌측 모터 방향 설정
    motorControl.setDirection(2, dir_FB); // 우측 모터 방향 설정
    
    if (V_Left>V_Right){ //좌측으로 갈때 좌측 틸팅
      if (0 <= V_Left - V_Right && V_Left - V_Right <= 150) {
        myServo.tiltLeft(30 , 1);
      }
      if (151 <= V_Left - V_Right && V_Left - V_Right <= 255) {
        myServo.tiltLeft(100, 1);
      }
      else {
        myServo.tiltLeft(0, 1);
      }
    }

    if (V_Left <V_Right){ //우측으로 갈때 우측 틸팅
      if (0 <= V_Right - V_Left && V_Right - V_Left <= 150) {
        myServo.tiltRight(30, 1);
      }
      if (150 <= V_Right - V_Left && V_Right - V_Left <= 255) {
        myServo.tiltRight(100, 1);
      }
      else {
        myServo.tiltRight(0, 1);
      }
    }

    if (Mode == 'S'){ //sleep 모드 활성화
      face.setFace("normal");
      myServo.Sleep(1);
      mode_count += 1;
      //delay(5000);
    }
    mpuSensor.update();
    if (mode_count >4 && mpuSensor.isThresholdExceeded()){
        //Serial.print("aaaaaaaa");
        face.setFace("wink");
        
        myServo.positionSet(10);
        //delay(50000);
        mode_count = 0;
    }


    if (buttonA == 'A') {
      myServo.walkForward(5);
    }

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
    
    if (buttonC == 'C') {
      myServo.walkBackward(5);
    }

    if (buttonD == 'D') {
      //myServo.positionSet(5);
      // distance = sensor.measureDistanceCm(); //거리
      // int x_b = sensor.getXB(); // 초음파 아우풋 10cm 이내 값이 1 아니면 0
      
      // if (distance <= 10){ //초음파 10cm 이내의 물체가 발견되면 뒤로 가기
      // motorControl.setSpeed(1, 20); // 좌측 모터 속도 설정
      // motorControl.setSpeed(2, 20); // 우측 모터 속도 설정
      // motorControl.setDirection(1, "B"); // 좌측 모터 방향 설정
      // motorControl.setDirection(2, 'B');
      // face.setFace("surprised");
      // //delay(500);
    //}
    }

    if (buttonE == 'E') {
      myServo.positionSet(10);
      //marioMusic.playMelody();
      //delay(5000);
      // myServo.positionSet(10);
      // music.playSmileMelody();
      // delay(200);

      // music.playCryMelody();
      // delay(200);
    }
  }
}