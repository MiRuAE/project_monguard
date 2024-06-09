#include <Arduino.h>
#include <FreeRTOS_AVR.h>
#include "MotorControl.h"
#include "BluetoothControl.h"
#include "faceControl.h"
#include "MyServoControl.h"
#include "MPU9250Library.h"
#include "UltrasonicSensor.h"
#include "MyMusic.h"

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
MyMusic music(BUZZER_PIN); // 음악 객체 생성

QueueHandle_t bluetoothQueue;
TaskHandle_t musicTaskHandle; // Music Task의 핸들 선언

void setup() {
  Serial.begin(9600);
  bluetoothControl.begin(9600);
  Serial.println(F("Bluetooth communication initialized."));

  face.begin();
  motorControl.init(); // 모터 제어 라이브러리 초기화
  myServo.begin();

  face.setFace("squint");
  myServo.positionSet(10);

  mpuSensor.begin(); //mpu9250 시작
  sensor.begin(); // 초음파센서 시작

  bluetoothQueue = xQueueCreate(10, sizeof(DataPacket)); // 블루투스 데이터 큐 생성

  xTaskCreate(bluetoothTask, "BluetoothTask", 128, NULL, 1, NULL); // 블루투스 태스크 생성
  xTaskCreate(motorTask, "MotorTask", 128, NULL, 1, NULL); // 모터 제어 태스크 생성
  xTaskCreate(faceTask, "FaceTask", 128, NULL, 1, NULL); // 얼굴 표정 제어 태스크 생성
  xTaskCreate(sensorTask, "SensorTask", 128, NULL, 1, NULL); // 센서 제어 태스크 생성
  xTaskCreate(servoTask, "ServoTask", 128, NULL, 1, NULL); // 서보모터 제어 태스크 생성
  xTaskCreate(musicTask, "MusicTask", 128, NULL, 1, &musicTaskHandle); // 음악 제어 태스크 생성 및 핸들 저장

  vTaskStartScheduler(); // FreeRTOS 스케줄러 시작
}

void loop() {
  // FreeRTOS에서는 loop()를 사용하지 않습니다.
}


void bluetoothTask(void *pvParameters) {
  DataPacket receivedPacket;
  for (;;) {
    if (bluetoothControl.readData(receivedPacket)) {
      xQueueSend(bluetoothQueue, &receivedPacket, portMAX_DELAY);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}


void motorTask(void *pvParameters) {
  DataPacket receivedPacket;
  for (;;) {
    if (xQueueReceive(bluetoothQueue, &receivedPacket, portMAX_DELAY)) {
      char dir_FB = receivedPacket.DIR_FB;
      int V_Left = receivedPacket.V_Left;
      int V_Right = receivedPacket.V_Right;

      motorControl.setSpeed(1, V_Left); // 좌측 모터 속도 설정
      motorControl.setSpeed(2, V_Right); // 우측 모터 속도 설정
      motorControl.setDirection(1, dir_FB); // 좌측 모터 방향 설정
      motorControl.setDirection(2, dir_FB); // 우측 모터 방향 설정

      if (V_Left > V_Right) { //좌측으로 갈때 좌측 틸팅
        if (0 <= V_Left - V_Right && V_Left - V_Right <= 150) {
          myServo.tiltLeft(30, 1);
        } else if (151 <= V_Left - V_Right && V_Left - V_Right <= 255) {
          myServo.tiltLeft(100, 1);
        } else {
          myServo.tiltLeft(0, 1);
        }
      }

      if (V_Left < V_Right) { //우측으로 갈때 우측 틸팅
        if (0 <= V_Right - V_Left && V_Right - V_Left <= 150) {
          myServo.tiltRight(30, 1);
        } else if (150 <= V_Right - V_Left && V_Right - V_Left <= 255) {
          myServo.tiltRight(100, 1);
        } else {
          myServo.tiltRight(0, 1);
        }
      }
    }
  }
}


void faceTask(void *pvParameters) {
  DataPacket receivedPacket;
  for (;;) {
    if (xQueueReceive(bluetoothQueue, &receivedPacket, portMAX_DELAY)) {
      char buttonB = receivedPacket.buttons[1];
      char Mode = receivedPacket.Mode;

      if (Mode == 'S') { //sleep 모드 활성화
        face.setFace("normal");
        myServo.Sleep(1);
        mode_count += 1;
      }

      if (mode_count > 4 && mpuSensor.isThresholdExceeded()) {
        face.setFace("wink");
        myServo.positionSet(10);
        mode_count = 0;
      }

      if (buttonB == 'B') { // 버튼 B가 눌린 상태
        int randomFace = random(6); // 0부터 5까지 랜덤 숫자 생성 (표정 6개)
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
    }
  }
}

void sensorTask(void *pvParameters) {
  for (;;) {
    double distance = sensor.measureDistanceCm();
    if (distance < 10.0) {
      face.setFace("surprised");
      // music.playSmileMelody();  // 주석 처리된 부분은 Music Task에서 수행
      xTaskNotify(musicTaskHandle, 1, eSetValueWithoutOverwrite); // Music Task에 알림
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}


void servoTask(void *pvParameters) {
  DataPacket receivedPacket;
  for (;;) {
    if (xQueueReceive(bluetoothQueue, &receivedPacket, portMAX_DELAY)) {
      char buttonA = receivedPacket.buttons[0];
      char buttonC = receivedPacket.buttons[2];
      char buttonE = receivedPacket.buttons[4];

      if (buttonA == 'A') {
        myServo.walkForward(5);
      }

      if (buttonC == 'C') {
        myServo.walkBackward(5);
      }

      if (buttonE == 'E') {
        myServo.positionSet(10);
        xTaskNotify(musicTaskHandle, 2, eSetValueWithoutOverwrite); // Music Task에 알림
      }
    }
  }
}


void musicTask(void *pvParameters) {
  uint32_t notificationValue;
  for (;;) {
    if (xTaskNotifyWait(0x00, 0xFFFFFFFF, &notificationValue, portMAX_DELAY)) {
      if (notificationValue == 1) {
        music.playSmileMelody();
      } else if (notificationValue == 2) {
        music.playCryMelody();
      }
    }
  }
}



