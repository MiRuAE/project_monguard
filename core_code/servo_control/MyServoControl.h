#ifndef MyServoControl_h
#define MyServoControl_h

#include <Arduino.h>

class MyServoControl {
public:
  MyServoControl();
  void begin();
// void handleXValue(int xValue);
  void walkForward();
  void walkBackward();
  void positionSet();
//  void handleButtonPress(char button);

private:
  const uint16_t NEUTRAL1 = 3925;
  const uint16_t NEUTRAL2 = 2375;
  const uint16_t ANGLE_RANGE = 300;
  
  void rightTilt(int xValue);
  void leftTilt(int xValue);
  void displayCry();
  void displaySmile();
  void displayArrows();
};

#endif
