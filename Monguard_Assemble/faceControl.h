#ifndef faceControl_h
#define faceControl_h

#include <Arduino.h>
#include <SPI.h>

class faceControl {
public:
  faceControl(uint8_t DIN, uint8_t CS, uint8_t CLK, uint8_t NUM_MATRICES);
  void begin();
  void setFace(String expression);
  void clearDisplay();

private:
  uint8_t _DIN, _CS, _CLK, _NUM_MATRICES;
  const uint8_t NUMBER_OF_ROWS = 8;

  // Declare expression arrays in the header file
  extern const uint8_t normalEyes[8];
  extern const uint8_t squintEyes[8];
  extern const uint8_t surprisedEyes[8];
  extern const uint8_t winkLeft[8];
  extern const uint8_t winkRight[8];

  extern const uint8_t smileMouth[8];
  extern const uint8_t flatMouth[8];
  extern const uint8_t openMouth[8];
  extern const uint8_t sadMouth[8];

  const uint8_t* expressionList[6][2] = { // Increased size to accommodate 'wink'
    {normalEyes, flatMouth},      // ㅡ.ㅡ
    {squintEyes, openMouth},      // >..<
    {normalEyes, smileMouth},     // ^__^
    {surprisedEyes, openMouth},   // O_O
    {normalEyes, sadMouth},       // ㅠ.ㅠ
    {normalEyes, smileMouth}     // For wink (eyes handled separately)
  };

  void write_Max7219(uint8_t matrix, uint8_t address, uint8_t data);
  void showFace(const uint8_t eyes[8], const uint8_t mouth[8]);
  void winkFace();
};

#endif
