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

#endif
