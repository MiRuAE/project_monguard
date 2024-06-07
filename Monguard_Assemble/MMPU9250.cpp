#include "MyMPU9250.h"

MyMPU9250::MyMPU9250(float threshold) : threshold(threshold), lastX(0), x_a(0) {}

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
      x_a = 1;  // Set x_a to 1 if threshold is exceeded
    } else {
      x_a = 0;  // Set x_a to 0 if threshold is not exceeded
    }

    Serial.print("X-axis acceleration: ");
    Serial.println(currentX);

    lastX = currentX;
  }
}

int MyMPU9250::getXA() {
  return x_a;  // Return the current value of x_a
}
