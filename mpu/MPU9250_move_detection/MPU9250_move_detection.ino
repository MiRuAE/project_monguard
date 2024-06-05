#include <Wire.h>
#include <MPU9250.h>

MPU9250 mpu;

float lastX = 0;
float threshold = 0.80; // 변화량 임계값 (원하는 값으로 조정)

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!mpu.setup(0x68)) {  // change to your own address
    while (1) {
      Serial.println("MPU 연결 불량");
      delay(5000);
    }
  }
}

void loop() {
  if (mpu.update()) {
    float currentX = mpu.getAccX();

    float deltaX = abs(currentX - lastX);

    if (deltaX*10 > threshold) {
      Serial.println("good");
    }

    Serial.print("X-axis acceleration: ");
    Serial.println(deltaX*10);

    lastX = currentX;
  }
  delay(100); // 데이터 갱신 주기 (원하는 값으로 조정)
}
