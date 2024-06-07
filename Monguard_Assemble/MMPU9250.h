#ifndef MyMPU9250_h
#define MyMPU9250_h

#include <Wire.h>
#include <MPU9250.h>

class MyMPU9250 {
  public:
    MyMPU9250(float threshold = 0.8);
    bool begin();
    void update();

  private:
    MPU9250 mpu;
    float lastX;
    float threshold;
};

#endif
