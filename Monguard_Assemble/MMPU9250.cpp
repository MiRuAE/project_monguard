#include "MyMPU9250.h"

MyMPU9250::MyMPU9250(float threshold) : threshold(threshold), lastX(0) {}

bool MyMPU9250::begin() {
  Wire.begin();
  if (!mpu.setup(0x68)) {  // change to your own address
    return false;
  }
  return true;
}

void MyMPU9250::update() {
  if (mpu.update()) {
    float currentX = mpu.getAccX();
    float deltaX = abs(currentX - lastX);

    if (deltaX > threshold) {
      Serial.println("good");
    }

    Serial.print("X-axis acceleration: ");
    Serial.println(currentX);

    lastX = currentX;
  }
}
