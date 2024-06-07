#ifndef MPU9250LIBRARY_H
#define MPU9250LIBRARY_H

#include <Arduino.h>  // For Serial, micros, delay, byte
#include <Wire.h>
#include <MPU9250.h>
#include <math.h>  // For sqrt, fabs, atan2f, asinf

class MPU9250Library {
public:
    MPU9250Library(uint8_t address = 0x68, float threshold = 0.2);

    bool begin();
    void update();
    bool isThresholdExceeded();

private:
    MPU9250 mpu;
    float lastX;
    float threshold;
    bool thresholdExceeded;
};

#endif
