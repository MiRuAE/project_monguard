#include <MPU9250.h>
#include <Arduino.h>
#include <Wire.h>
#include <PID_v1.h>

#define MIN_ABS_SPEED 15

#define MOTOR1_DIR_A_PIN 0x00
#define MOTOR1_DIR_B_PIN 0x01
#define MOTOR1_ENABLE_PIN 0x02

#define MOTOR2_DIR_A_PIN 0x02 // 포트 C로 변경
#define MOTOR2_DIR_B_PIN 0x03 // 포트 C로 변경
#define MOTOR2_ENABLE_PIN 0x01 // 포트 C로 변경

MPU9250 mpu;

double setpoint = 1.0;
double input, output;
double Kp = 1.00;  // PID 계수 조정 필요
double Ki = 0.005; // PID 계수 조정 필요
double Kd = 0.00;  // PID 계수 조정 필요
PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

int ENA = 5;
int IN1 = 6;
int IN2 = 7;
int IN3 = 8;
int IN4 = 9;
int ENB = 10;

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

    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    output = 0;
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);

    DDRC |= (1 << MOTOR1_DIR_A_PIN) | (1 << MOTOR1_DIR_B_PIN) | (1 << MOTOR2_DIR_A_PIN) | (1 << MOTOR2_DIR_B_PIN); 
    DDRB |= (1 << MOTOR1_ENABLE_PIN) | (1 << MOTOR2_ENABLE_PIN); 

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
    OCR1B = abs(invertedOutput);  

    if (forward) {
        PORTC |= (1 << MOTOR2_DIR_A_PIN); // 포트 C로 변경
        PORTC &= ~(1 << MOTOR2_DIR_B_PIN); // 포트 C로 변경
    } else {
        PORTC &= ~(1 << MOTOR2_DIR_A_PIN); // 포트 C로 변경
        PORTC |= (1 << MOTOR2_DIR_B_PIN); // 포트 C로 변경
    }
    OCR1A = abs(invertedOutput); 

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
