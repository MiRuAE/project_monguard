#include <Wire.h>
#include <MPU9250.h>
#include <PID_v1.h>
#include <LMotorController.h>

#define MIN_ABS_SPEED 20

// MPU9250 객체 생성
MPU9250 mpu;

double setpoint = 0;
double input, output;
double Kp = 50;
double Ki = 1.0;
double Kd = 1.0;
PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

// 포트 변경 (아날로그 A0, A1, A2, A3 및 디지털 5, 6)
int IN1 = A0;
int IN2 = A1;
int IN3 = A2;
int IN4 = A3;
int ENA = 5;
int ENB = 6;
LMotorController motorController(ENA, IN1, IN2, ENB, IN3, IN4, 0.6, 0.6);

void setup() {
  Wire.begin();
  Serial.begin(115200);
  delay(2000); // 전원 안정화를 위한 지연 시간

  Serial.println("Starting setup...");

  // MPU9250 초기화 (AD0 핀이 GND에 연결된 경우 주소는 0x68)
  while (!mpu.setup(0x68)) {  // 초기화 성공할 때까지 반복 시도
    Serial.println("MPU9250 initialization failed! Retrying...");
    delay(100);
  }

  Serial.println("MPU9250 initialized successfully");

  // PID 설정
  pid.SetMode(AUTOMATIC);
  pid.SetSampleTime(10);
  pid.SetOutputLimits(-255, 255);
}

void loop() {
  if (mpu.update()) {  // 데이터 업데이트 성공 시에만 처리

    // Yaw, Pitch, Roll 계산 (이전 버전 방식으로 복구)
    float ypr[3];
    ypr[0] = mpu.getYaw();
    ypr[1] = mpu.getPitch();
    ypr[2] = mpu.getRoll();

    // PID 입력을 Pitch 값으로 설정
    input = ypr[1];

    // PID 계산
    pid.Compute();

    // PID 출력 값을 반대로 변경하여 모터 제어
    double invertedOutput = -output;

    // 모터 제어
    motorController.move(invertedOutput, MIN_ABS_SPEED);

    // 디버깅 출력 (선택 사항)
    Serial.print("ypr\t");
    Serial.print(ypr[0], 2);  // Yaw (Z)
    Serial.print("\t");
    Serial.print(ypr[1], 2);  // Pitch (Y)
    Serial.print("\t");
    Serial.println(ypr[2], 2); // Roll (X)
  }

  delay(100);
}
