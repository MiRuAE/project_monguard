#ifndef FACE_CONTROL_H
#define FACE_CONTROL_H

#include <Arduino.h>
#include <SPI.h>

// 핀 정의
#define DIN 2
#define CS 3
#define CLK 4
#define NUM_MATRICES 4
#define NUMBER_OF_ROWS 8

// 얼굴 표정 정의 (각 표정은 8개의 행 데이터로 구성)
extern const uint8_t normalEyes[8];
extern const uint8_t squintEyes[8];
extern const uint8_t surprisedEyes[8];
extern const uint8_t winkLeft[8];
extern const uint8_t winkRight[8];

<<<<<<< HEAD
  // Declare expression arrays in the header file
  static const uint8_t normalEyes[8];
  static const uint8_t squintEyesLeft[8];
  static const uint8_t squintEyesRight[8];
  static const uint8_t surprisedEyes[8];
  static const uint8_t winkLeft[8];
  static const uint8_t winkRight[8];
  static const uint8_t sadEyes[8];
  static const uint8_t angryEyesLeft[8];
  static const uint8_t angryEyesRight[8];

  static const uint8_t smileMouth1[8];
  static const uint8_t smileMouth2[8];
  static const uint8_t flatMouth1[8];
  static const uint8_t flatMouth2[8];
  static const uint8_t openMouth1[8];
  static const uint8_t openMouth2[8];
  static const uint8_t winkMouth1[8];
  static const uint8_t winkMouth2[8];
  static const uint8_t sadMouth1[8];
  static const uint8_t sadMouth2[8];

  void write_Max7219(uint8_t matrix, uint8_t address, uint8_t data);
  void write_byte(uint8_t data);
  void normalface();
  void squintface();
  void surprisedface();
  void winkface();
  void sadface();
  void angryface();
};
=======
extern const uint8_t smileMouth[8];
extern const uint8_t flatMouth[8];
extern const uint8_t openMouth[8];
extern const uint8_t sadMouth[8];

// 함수 프로토타입
void initMatrices();
void setFace(const uint8_t eyes[8], const uint8_t mouth[8]);
void winkFace();
void clearDisplay();
void write_Max7219(uint8_t matrix, uint8_t address, uint8_t data); // 함수 프로토타입 추가
>>>>>>> 469c445ca884ca7cf1e5c0664834c3b258adb4aa

#endif
