#ifndef MotorControl_h
#define MotorControl_h

#include <Arduino.h>

class MotorControl {
  public:
    void init();
    void setSpeed(int motor, int speed);
    void setDirection(int motor, bool forward);
    
  private:
    const int motor1DirAPin = 0x00;  // MOTOR1 Direction pin A (Offset for PORTC)
    const int motor1DirBPin = 0x01;  // MOTOR1 Direction pin B (Offset for PORTC)
    const int motor1EnablePin = 0x40; // MOTOR1 Enable pin (Offset for PORTD)

    const int motor2DirAPin = 0x02;  // MOTOR2 Direction pin A (Offset for PORTC)
    const int motor2DirBPin = 0x03;  // MOTOR2 Direction pin B (Offset for PORTC)
    const int motor2EnablePin = 0x20; // MOTOR2 Enable pin (Offset for PORTD)
};

#endif
