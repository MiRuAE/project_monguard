#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 4); // TX=2, RX=4 BLUETOOTH MODULE

#define NEUTRAL1 2750 // Calibrated neutral value for servo 1 (Left) (in microseconds)
#define NEUTRAL2 3450 // Calibrated neutral value for servo 2 (Right) (in microseconds)
#define ANGLE_RANGE  300 // Maximum deviation from neutral
#define DIN 13
#define CS 12
#define CLK 11
#define NUM_MATRICES 4
#define NUMBER_OF_ROWS 8

#define BUTTON_A "A: Yes"
#define BUTTON_B "B: Yes"
#define BUTTON_C "C: Yes"
#define BUTTON_D "D: Yes"

// ">" 표현
const uint8_t arrowRight[8] = {
  0b00011000,
  0b00001100,
  0b00000110,
  0b00000011,
  0b00000011,
  0b00000110,
  0b00001100,
  0b00011000
};

// "<" 표현
const uint8_t arrowLeft[8] = {
  0b00011000,
  0b00110000,
  0b01100000,
  0b11000000,
  0b11000000,
  0b01100000,
  0b00110000,
  0b00011000
};

const uint8_t arrowCenter1[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00001100,
  0b00000111
};

const uint8_t arrowCenter2[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00110000,
  0b11100000
};


// "^" 표현
const uint8_t smile[8] = {
  0b00000000,
  0b00011000,
  0b00111100,
  0b01100110,
  0b11000011,
  0b10000001,
  0b00000000,
  0b00000000
};

const uint8_t cry[8] = {
  0b00000000,
  0b11111111,
  0b01100110,
  0b11001100,
  0b01100110,
  0b00110011,
  0b01100110,
  0b00000000,
};

const uint8_t center1[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000001,
  0b00000011,
  0b00000110,
  0b00001100,
  0b00000000,
};

const uint8_t center2[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b10000000,
  0b11000000,
  0b01100000,
  0b00110000,
  0b00000000,
};

void setup() {
  Serial.begin(9600);       // 시리얼 통신 시작, 전송 속도 9600
  mySerial.begin(9600);    // 블루투스 시리얼 통신 시작, 전송 속도 9600

  // Configure Timer/Counter1 for Fast PWM mode
  cli(); // Disable global interrupts
  DDRB |= (1 << PB1) | (1 << PB2); // Set OC1A (PB1) and OC1B (PB2) as output

  // Configure Timer/Counter1
  TCCR1A = (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1); // Fast PWM, 10-bit
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Fast PWM, Prescaler 8

  ICR1 = 40000; // Top value for 50Hz PWM (20ms period)

  sei(); // Enable global interrupts

  // Initialize the PWM values to neutral position
  OCR1A = NEUTRAL1; // Timer counts in microseconds
  OCR1B = NEUTRAL2; // Timer counts in microseconds

  pinMode(CLK, OUTPUT);
  pinMode(DIN, OUTPUT);
  pinMode(CS, OUTPUT);
  delay(100);

  // LED 매트릭스 옵션 설정
  for (uint8_t i = 0; i < NUM_MATRICES; i++) {
    write_Max7219(i, 0x09, 0x00); // Decode Mode Register – No decode for digits
    write_Max7219(i, 0x0A, 0x0F); // Intensity Register – 0x00 to 0x0F
    write_Max7219(i, 0x0B, 0x07); // Scan Limit Register – All Output Port Enable
    write_Max7219(i, 0x0C, 0x01); // SHUTDOWN Register – Normal Operation
    write_Max7219(i, 0x0F, 0x00); // Display Test Register – Display Test Mode
  }

  displayArrows();
  
  Serial.println("Ready to receive commands:");
}

void loop() {
  // 블루투스에서 데이터 수신 시 처리
  if (mySerial.available()) {
    String receivedData = mySerial.readStringUntil('\n'); // 줄바꿈까지 문자열 읽기
    Serial.println(receivedData); // 수신된 데이터를 출력

    
    // X와 Y 값 추출
    if (receivedData.startsWith("X:")) {
      int xValue = receivedData.substring(2).toInt(); // Extract the number after "X:"
      Serial.print("X Value: ");
      Serial.println(xValue);
      handleXValue(xValue);
    }
    if (receivedData.startsWith("Y:")) {
      int yValue = receivedData.substring(2).toInt(); // Extract the number after "Y:"
      Serial.print("Y Value: ");
      Serial.println(yValue);
    }
    
    // 버튼 상태에 따른 동작 수행
    if (receivedData.indexOf(BUTTON_A) != -1) {
//      Serial.println("Button A Pressed");
      walkForward();
    } else if (receivedData.indexOf(BUTTON_B) != -1) {
//      Serial.println("Button B Pressed");
        int expression = random(2);
        if(expression == 1) {
          displayCry();
        } else {
          displaySmile();
        }
        delay(1000);
        displayArrows();
    } else if (receivedData.indexOf(BUTTON_C) != -1) {
//      Serial.println("Button C Pressed");
      walkBackward();
    } else if (receivedData.indexOf(BUTTON_D) != -1) {
//      Serial.println("Button D Pressed");
//        playMelody();
      // 필요한 동작 추가
    }
  }
  
  // 시리얼 모니터에서 입력된 데이터를 블루투스로 전송
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}

void Righttilt(int xValue) {
  int offset = (ANGLE_RANGE * (504 - xValue)) / 504; // Calculate based on xValue
  OCR1A = NEUTRAL1 + offset;
  OCR1B = NEUTRAL2 + offset;
}

void Lefttilt(int xValue) {
  int offset = (ANGLE_RANGE * (xValue - 504)) / 519; // Calculate based on xValue
  OCR1A = NEUTRAL1 - offset;
  OCR1B = NEUTRAL2 - offset;
}

void handleXValue(int xValue) {
  if (xValue < 504) {
    Righttilt(xValue);
  } else if (xValue > 504) {
    Lefttilt(xValue);
  } else {
    OCR1A = NEUTRAL1;
    OCR1B = NEUTRAL2;
  }
}

void walkForward() {
  OCR1A = NEUTRAL1 + ANGLE_RANGE; // Move to max forward position
  OCR1B = NEUTRAL2 - ANGLE_RANGE;
  delay(500); // Hold forward position for 500ms (adjust if needed)
  OCR1A = NEUTRAL1; // Return to neutral
  OCR1B = NEUTRAL2;
}

void walkBackward() {
  OCR1A = NEUTRAL1 - ANGLE_RANGE; // Move to max backward position
  OCR1B = NEUTRAL2 + ANGLE_RANGE;
  delay(500); // Hold backward position for 500ms (adjust if needed)
  OCR1A = NEUTRAL1; // Return to neutral
  OCR1B = NEUTRAL2;
}

void displayArrows() {
  for (uint8_t row = 0; row < NUMBER_OF_ROWS; row++) {
    write_Max7219(0, row + 1, arrowRight[row]); // First Matrix
    write_Max7219(1, row + 1, arrowCenter1[row]); // Second Matrix
    write_Max7219(2, row + 1, arrowCenter2[row]); // Third Matrix
    write_Max7219(3, row + 1, arrowLeft[row]); // Fourth Matrix
  }
}

void displaySmile() {
  for (uint8_t row = 0; row < NUMBER_OF_ROWS; row++) {
    write_Max7219(0, row + 1, smile[row]);
    write_Max7219(1, row + 1, arrowCenter1[row]);
    write_Max7219(2, row + 1, arrowCenter2[row]);
    write_Max7219(3, row + 1, smile[row]);
  }
}

void displayCry() {
  for(uint8_t row = 0; row < NUMBER_OF_ROWS; row++) {
    write_Max7219(0, row + 1, cry[row]);
    write_Max7219(1, row + 1, center1[row]);
    write_Max7219(2, row + 1, center2[row]);
    write_Max7219(3, row + 1, cry[row]);
  }
}

void write_Max7219(uint8_t matrix, uint8_t address, uint8_t data) {
  digitalWrite(CS, LOW);
  for (uint8_t i = 0; i < NUM_MATRICES; i++) {
    if (i == matrix) {
      write_byte(address);
      write_byte(data);
    } else {
      write_byte(0);
      write_byte(0);
    }
  }
  digitalWrite(CS, HIGH);
}

void write_byte(uint8_t data) {
  for (uint8_t i = 0; i < 8; i++) {
    bool bit = (data & (1 << (7 - i)));
    digitalWrite(DIN, bit);
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }
}
