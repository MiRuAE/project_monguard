#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

#include <Arduino.h>

class UltrasonicSensor {
public:
    UltrasonicSensor(uint8_t trigPin, uint8_t echoPin);
    void begin();
    double measureDistanceCm();
    int getXB();

private:
    uint8_t trigPin;
    uint8_t echoPin;
    int x_b;
};

#endif
