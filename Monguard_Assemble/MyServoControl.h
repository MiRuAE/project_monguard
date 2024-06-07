#ifndef MyServoControl_h
#define MyServoControl_h

#include <Arduino.h>

class MyServoControl {
public:
  MyServoControl();
  void begin();
  void walkForward();
  void walkBackward();
  void positionSet();
  void increaseAngle(uint16_t value);
  void decreaseAngle(uint16_t value);
  void tiltLeft(uint16_t value);
  void tiltRight(uint16_t value);
  void upDownTilt(uint16_t value);

private:
  const uint16_t NEUTRAL1 = 3925;
  const uint16_t NEUTRAL2 = 2375;
  const uint16_t ANGLE_RANGE = 300;
  
  uint16_t currentAngle1;
  uint16_t currentAngle2;
  
  void updateServo();
};

#endif
