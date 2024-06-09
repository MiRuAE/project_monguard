#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(uint8_t trigPin, uint8_t echoPin) 
    : trigPin(trigPin), echoPin(echoPin), x_b(0) {}

void UltrasonicSensor::begin() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

double UltrasonicSensor::measureDistanceCm() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    double duration = pulseIn(echoPin, HIGH);
    double cm = (duration / 2) * 0.0343;

    if (cm >= 10.0) {
        x_b = 1;
    } else {
        x_b = 0;
    }

    return cm;
}

int UltrasonicSensor::getXB() {
    return x_b;
}
