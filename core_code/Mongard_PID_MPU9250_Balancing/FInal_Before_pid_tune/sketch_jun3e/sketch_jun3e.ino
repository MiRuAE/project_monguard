#include <MPU9250.h>
#include <Arduino.h>
#include <Wire.h>
#include <PID_v1.h>
#include <math.h> 

#define MIN_ABS_SPEED 15

#define MOTOR1_DIR_A_PIN 0x00
#define MOTOR1_DIR_B_PIN 0x01

#define MOTOR2_DIR_A_PIN 0x02 
#define MOTOR2_DIR_B_PIN 0x03 

MPU9250 mpu;

double setpoint = 1.0;
double input, output;
double Kp = 1.00; 
double Ki = 0.005; 
double Kd = 0.00; 
PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);


float pitchOffset = 0;

void setup() {
    Wire.begin();
    Serial.begin(115200); 
    delay(2000);

    Serial.println("Starting setup...");

    if (!mpu.setup(0x68)) {
        Serial.println("MPU9250 initialization failed!");
        while (1) {
            Serial.println("MPU9250 initialization failed! Check connections and restart.");
            delay(1000);
        }
    }

    Serial.println("MPU9250 initialized successfully");

    mpu.update();
    float initialYpr[3];
    initialYpr[0] = mpu.getYaw();
    initialYpr[1] = mpu.getPitch();
    initialYpr[2] = mpu.getRoll();

    Serial.print("Initial Yaw: ");
    Serial.print(initialYpr[0], 2);
    Serial.print(" Pitch: ");
    Serial.print(initialYpr[1], 2);
    Serial.print(" Roll: ");
    Serial.println(initialYpr[2], 2);

    pitchOffset = initialYpr[1];

    pid.SetMode(AUTOMATIC);
    pid.SetSampleTime(10);
    pid.SetOutputLimits(-255, 255);

    // 핀 모드 설정 (아날로그 핀을 출력으로 사용)
    pinMode(A0, OUTPUT); // ENA
    pinMode(A1, OUTPUT); // ENB
    pinMode(0, OUTPUT);  // IN1
    pinMode(1, OUTPUT);  // IN2
    pinMode(2, OUTPUT);  // IN3
    pinMode(3, OUTPUT);  // IN4

    output = 0;
    analogWrite(A0, 0); // ENA
    analogWrite(A1, 0); // ENB

    DDRC |= (1 << MOTOR1_DIR_A_PIN) | (1 << MOTOR1_DIR_B_PIN) | (1 << MOTOR2_DIR_A_PIN) | (1 << MOTOR2_DIR_B_PIN); 

    // 타이머 설정은 기존 코드와 동일하게 유지
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11); 
    TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS10);   
    ICR1 = 1023; 
}

void loop() {
    mpu.update();

    float ypr[3];
    ypr[0] = mpu.getYaw();
    ypr[1] = mpu.getPitch();
    ypr[2] = mpu.getRoll();

    float pitchWithOffset = ypr[1] - pitchOffset;

    input = pitchWithOffset;  // Calculate input after mpu.update()
    Serial.print("PID Input (Pitch with Offset): ");
    Serial.println(input);

    pid.Compute();

    double invertedOutput = -output;

    if (abs(invertedOutput) < MIN_ABS_SPEED) {
        invertedOutput = 0;
    }

    Serial.print("PID Output: ");
    Serial.println(invertedOutput);

    bool forward = (invertedOutput > 0);

    if (forward) {
        PORTC |= (1 << MOTOR1_DIR_A_PIN);
        PORTC &= ~(1 << MOTOR1_DIR_B_PIN);
    } else {
        PORTC &= ~(1 << MOTOR1_DIR_A_PIN);
        PORTC |= (1 << MOTOR1_DIR_B_PIN);
    }
    analogWrite(A0, abs(invertedOutput));  // ENA를 사용하여 모터1 속도 제어

    if (forward) {
        PORTC |= (1 << MOTOR2_DIR_A_PIN); // 포트 C로 변경
        PORTC &= ~(1 << MOTOR2_DIR_B_PIN); // 포트 C로 변경
    } else {
        PORTC &= ~(1 << MOTOR2_DIR_A_PIN); // 포트 C로 변경
        PORTC |= (1 << MOTOR2_DIR_B_PIN); // 포트 C로 변경
    }
    analogWrite(A1, abs(invertedOutput)); // ENB를 사용하여 모터2 속도 제어

    double motorSpeed = map(abs(invertedOutput), 0, 255, 0, 100);

    Serial.print("Motor Speed (%): ");
    Serial.println(motorSpeed);

    Serial.print("ypr\t");
    Serial.print(ypr[0], 2);
    Serial.print("\t");
    Serial.print(ypr[1], 2);
    Serial.print("\t");
    Serial.print(ypr[2], 2);
    Serial.print("\tPitch with Offset: ");
    Serial.println(pitchWithOffset, 2);

    delay(10);
}
