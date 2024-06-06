#include "faceControl.h"

// 얼굴 표정 정의
const uint8_t normalEyes[8] = {
  0b00111100, 0b01111110, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b01111110, 0b00111100
};
const uint8_t squintEyes[8] = {
  0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00000000, 0b00000000
};
const uint8_t surprisedEyes[8] = {
  0b00011000, 0b00111100, 0b01100110, 0b11000011, 0b11000011, 0b01100110, 0b00111100, 0b00011000
};
const uint8_t winkLeft[8] = {
  0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000
};
const uint8_t winkRight[8] = {
  0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000
};

const uint8_t smileMouth[8] = {
  0b00000000, 0b00000000, 0b00001100, 0b00011110, 0b00111111, 0b01111110, 0b11111100, 0b00000000
};
const uint8_t flatMouth[8] = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11111111
};
const uint8_t openMouth[8] = {
  0b00000000, 0b00011000, 0b00111100, 0b01111110, 0b01111110, 0b00111100, 0b00011000, 0b00000000
};
const uint8_t sadMouth[8] = {
  0b00000000, 0b11111100, 0b01111110, 0b00111111, 0b00011110, 0b00001100, 0b00000000, 0b00000000
};

// 모든 매트릭스 초기화
void initMatrices() {
  pinMode(CLK, OUTPUT);
  pinMode(DIN, OUTPUT);
  pinMode(CS, OUTPUT);
  delay(100);

  for (uint8_t i = 0; i < NUM_MATRICES; i++) {
    write_Max7219(i, 0x09, 0x00);  // 디코딩 모드: 디코딩 없음
    write_Max7219(i, 0x0A, 0x0F);  // 밝기 설정: 최대 밝기 (0x00 ~ 0x0F)
    write_Max7219(i, 0x0B, 0x07);  // 스캔 제한: 모든 행 스캔
    write_Max7219(i, 0x0C, 0x01);  // 종료 모드: 일반 작동 모드
    write_Max7219(i, 0x0F, 0x00);  // 디스플레이 테스트 모드: 해제
  }
  clearDisplay(); // 모든 LED 끄기
}

// 주어진 눈과 입 표정으로 얼굴 표시
void setFace(const uint8_t eyes[8], const uint8_t mouth[8]) {
  // 눈 표시 (양쪽 끝 매트릭스)
  for (uint8_t row = 0; row < NUMBER_OF_ROWS; row++) {
    write_Max7219(0, row + 1, eyes[row]);  // 왼쪽 눈
    write_Max7219(3, row + 1, eyes[row]);  // 오른쪽 눈
  }

  // 입 또는 코 표시 (가운데 두 매트릭스)
  for (uint8_t row = 0; row < NUMBER_OF_ROWS; row++) {
    write_Max7219(1, row + 1, mouth[row]);
    write_Max7219(2, row + 1, mouth[row]);
  }
}

// 윙크 표정 (동적 패턴)
void winkFace() {
  // 눈을 깜빡이는 패턴
  for (uint8_t i = 0; i < 3; i++) {
    setFace(winkLeft, smileMouth);  // 왼쪽 눈 깜빡임
    delay(500);
    setFace(normalEyes, smileMouth);  // 눈 원래대로
    delay(500);
  }

  // 오른쪽 눈을 깜빡이는 패턴
  for (uint8_t i = 0; i < 3; i++) {
    setFace(winkRight, smileMouth);  // 오른쪽 눈 깜빡임
    delay(500);
    setFace(normalEyes, smileMouth);  // 눈 원래대로
    delay(500);
  }
}

// 모든 LED 끄기
void clearDisplay() {
  for (uint8_t i = 0; i < NUM_MATRICES; i++) {
    for (uint8_t j = 0; j < NUMBER_OF_ROWS; j++) {
      write_Max7219(i, j + 1, 0x00);
    }
  }
}

// MAX7219 매트릭스에 데이터 쓰기 함수
void write_Max7219(uint8_t matrix, uint8_t address, uint8_t data) {
  digitalWrite(CS, LOW);  // CS 핀 낮춰서 통신 시작

  // 모든 매트릭스에 데이터 전송 (선택된 매트릭스만 유효한 데이터 받음)
  for (uint8_t i = 0; i < NUM_MATRICES; i++) {
    if (i == matrix) {
      shiftOut(DIN, CLK, MSBFIRST, address);  // 주소 전송
      shiftOut(DIN, CLK, MSBFIRST, data);     // 데이터 전송
    } else {
      shiftOut(DIN, CLK, MSBFIRST, 0);  // 더미 데이터 전송 (다른 매트릭스 무시)
      shiftOut(DIN, CLK, MSBFIRST, 0);
    }
  }

  digitalWrite(CS, HIGH);  // CS 핀 높여서 통신 종료
}
