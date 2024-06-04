#include <Arduino.h>

// Pin definitions
#define MOTOR1_DIR_A_PIN A0  // MOTOR1 Direction pin A
#define MOTOR1_DIR_B_PIN A1  // MOTOR1 Direction pin B
#define MOTOR1_ENABLE_PIN 10 // MOTOR1 Enable pin (PWM)

#define MOTOR2_DIR_A_PIN A2  // MOTOR2 Direction pin A
#define MOTOR2_DIR_B_PIN A3  // MOTOR2 Direction pin B
#define MOTOR2_ENABLE_PIN 9  // MOTOR2 Enable pin (PWM)

#define POT_PIN A5            // Potentiometer input (Analog PIN A5)

void init_ADC() {
  ADMUX |= (0 << REFS1) | (1 << REFS0); // AVcc referenceßß
  ADMUX &= ~(1 << ADLAR);               // Right adjust result
  ADCSRA |= (1 << ADEN);                // ADC enable
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128
}

int read_ADC(uint8_t channel) {
  channel &= 0x07;                      // Ensure channel is between 0-7
  ADMUX = (ADMUX & 0xF8) | channel;     // Select ADC channel
  ADCSRA |= (1 << ADSC);                // Start ADC conversion
  while (ADCSRA & (1 << ADSC));         // Wait for conversion to complete
  return ADCW;                          // Read ADC value (10-bit)
}

void setup() {
  Serial.begin(9600);
  init_ADC();

  // Configure pins
  pinMode(MOTOR1_DIR_A_PIN, OUTPUT);
  pinMode(MOTOR1_DIR_B_PIN, OUTPUT);
  pinMode(MOTOR1_ENABLE_PIN, OUTPUT);

  pinMode(MOTOR2_DIR_A_PIN, OUTPUT);
  pinMode(MOTOR2_DIR_B_PIN, OUTPUT);
  pinMode(MOTOR2_ENABLE_PIN, OUTPUT);

  // Timer setup (adjust as needed for desired PWM frequency)
  TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10); // Fast PWM, 8-bit
  TCCR1B = (1 << CS11);                                  // Prescaler 8
}

void loop() {
  int pot_value = read_ADC(POT_PIN - A0);  
  int speed = map(pot_value, 0, 1023, 0, 255);

  // Set direction based on potentiometer value
  bool forward = (pot_value > 512);  

  // Motor 1
  digitalWrite(MOTOR1_DIR_A_PIN, forward);
  digitalWrite(MOTOR1_DIR_B_PIN, !forward);
  analogWrite(MOTOR1_ENABLE_PIN, speed);

  // Motor 2
  digitalWrite(MOTOR2_DIR_A_PIN, forward);
  digitalWrite(MOTOR2_DIR_B_PIN, !forward);
  analogWrite(MOTOR2_ENABLE_PIN, speed);

  Serial.print("Potentiometer: ");
  Serial.print(pot_value);
  Serial.print("  Speed: ");
  Serial.print(speed);
  Serial.print("  Direction: ");
  Serial.println(forward ? "Forward" : "Backward");
  delay(100); 
}
