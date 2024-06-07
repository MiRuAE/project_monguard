#ifndef MyMPU9250_h
#define MyMPU9250_h

#include <Wire.h>
#include <MPU9250.h>

class MyMPU9250 {
  public:
    MyMPU9250(float threshold = 0.8);
    bool begin();
    void update();
    int getXA();  // New method to get the x_a value

  private:
    MPU9250 mpu;
    float lastX;
    float threshold;
    int x_a;  // New variable to store the threshold crossing status
};

#endif
