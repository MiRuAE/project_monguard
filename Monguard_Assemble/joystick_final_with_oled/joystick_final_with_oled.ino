// 98d3:11:fc3cd1
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PORTD_BUTTON_A  0x04 // PIN2
#define PORTD_BUTTON_B  0x08 // PIN3
#define PORTD_BUTTON_C  0x10 // PIN4
#define PORTD_BUTTON_D  0x20 // PIN5
#define PORTB_BUTTON_E  0x01 // PIN8

#define X_CHANNEL 0x00 // ADC0
#define Y_CHANNEL 0x01 // ADC1

//int sleep_count = 0; // check sleep mode

SoftwareSerial mySerial(13, 12); // TX=11, RX=12 BLUETOOTH MODULE

void init_ADC() {
  ADMUX |= (0 << REFS1) | (1 << REFS0); // AVcc reference
  ADMUX &= ~(1 << ADLAR);               // Right adjust result
  ADCSRA |= (1 << ADEN);                // ADC enable
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128
}

int read_ADC(uint8_t channel) {
  channel &= 0x07;                      // Ensure channel is between 0-7
  ADMUX = (ADMUX & 0xF8) | channel;     // Select ADC channel
  ADCSRA |= (1 << ADSC);                // Start ADC conversion
  while (ADCSRA & (1 << ADSC));         // Wait for conversion to complete
  return ADC;
}

struct DataPacket {
  char DIR_FB;
  char DIR_LR;
  char Mode;
  int V_Left;
  int V_Right;
  char buttons[5]; // Increased to accommodate Button E
};

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  init_ADC();

  // Set pins 2, 3, 4, 5, 8 as inputs with internal pull-ups
  DDRD &= ~(PORTD_BUTTON_A | PORTD_BUTTON_B | PORTD_BUTTON_C | PORTD_BUTTON_D);
  PORTD |= (PORTD_BUTTON_A | PORTD_BUTTON_B | PORTD_BUTTON_C | PORTD_BUTTON_D);
  DDRB &= ~PORTB_BUTTON_E;
  PORTB |= PORTB_BUTTON_E;

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the display
  display.clearDisplay();
  display.display();

  // Set text size and color
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}
void loop() {
  // Read joystick values
  int X = read_ADC(X_CHANNEL);
  int Y = read_ADC(Y_CHANNEL);

  int steer = map(X, 0, 1023, -255, 255); // Steering split
  int speed;
  char mode;
  char dir_FB;
  char dir_LR;

  if (Y > 508) { 
    dir_FB = 'F'; // Front
    speed = map(Y, 508, 1023, 0, 255);
  } else if (Y < 503) {
    dir_FB = 'B'; // Back
    speed = map(Y, 0, 503, 255, 0);
  } else {
    dir_FB = 'N'; // Neutral
    speed = 0;
  }

  if (X > 506) {
    dir_LR = 'R'; // Right
  } else if (X < 500) {
    dir_LR = 'L'; // Left
  } else {
    dir_LR = 'N'; // Neutral
  }

  int V_Left, V_Right;

  if (dir_LR == 'R') { // Turn right
    V_Left = speed;
    V_Right = speed - steer;
  } else if (dir_LR == 'L') { // Turn left
    V_Left = speed + steer;
    V_Right = speed;
  } else { // Forward/Backward without turning
    V_Left = speed;
    V_Right = speed;
  }

  V_Left = constrain(V_Left, 0, 220);
  V_Right = constrain(V_Right, 0, 220);

  // Read button states using PIND and PINB registers and format into a single byte
  char buttons[5] = {'0', '0', '0', '0', '0'}; // Increased to accommodate Button E
  if (!(PIND & PORTD_BUTTON_A)) {
    buttons[0] = 'A';
  }
  if (!(PIND & PORTD_BUTTON_B)) {
    buttons[1] = 'B';
  }
  if (!(PIND & PORTD_BUTTON_C)) {
    buttons[2] = 'C';
  }
  if (!(PIND & PORTD_BUTTON_D)) {
    buttons[3] = 'D';
  }
  if (!(PINB & PORTB_BUTTON_E)) {
    buttons[4] = 'E';
  }

  if(!(PIND & PORTD_BUTTON_D)) {
    mode = 'S'; // Sleep mode
    // sleep_count += 1;
    // if(sleep_count == 30) {
    //   mode = 'S'; // Sleep mode
    //   //sleep_count = 0;
    // }
  } else {
    mode = 'W'; // Awake mode
    //sleep_count = 0;
  }

  // Send data packet: [dir_FB, dir_LR, V_Left, V_Right, buttonState]
  DataPacket dataPacket;
  dataPacket.DIR_FB = dir_FB;
  dataPacket.DIR_LR = dir_LR;
  dataPacket.Mode = mode;
  dataPacket.V_Left = V_Left;
  dataPacket.V_Right = V_Right;
  dataPacket.buttons[0] = buttons[0];
  dataPacket.buttons[1] = buttons[1];
  dataPacket.buttons[2] = buttons[2];
  dataPacket.buttons[3] = buttons[3];
  dataPacket.buttons[4] = buttons[4];

  // Send data packet via Bluetooth
  mySerial.write((uint8_t *)&dataPacket, sizeof(dataPacket));

  // Print debug information
  Serial.print("DIR_FB: ");
  Serial.print(dir_FB);
  Serial.print(" DIR_LR: ");
  Serial.print(dir_LR);
  Serial.print(" V_Left: ");
  Serial.print(V_Left);
  Serial.print(" V_Right: ");
  Serial.print(V_Right);
  Serial.print(" Buttons: ");
  Serial.print(buttons);
  Serial.print(" Sleep: ");
  Serial.println(mode);

  // Display information on OLED
  display.clearDisplay();

  // Convert direction to full text
  String fullDir_FB;
  if (dir_FB == 'F') {
    fullDir_FB = "Forward";
  } else if (dir_FB == 'B') {
    fullDir_FB = "Backward";
  } else {
    fullDir_FB = "Stop";
  }
  
  display.setCursor(0, 0); // 커서위치 지정

  if(mode == 'W') {
    // Print only pressed buttons
    display.setTextSize(1);
    //display.println("Direction: ");
    //display.print(fullDir_FB);
    display.setTextSize(2);
    display.print(fullDir_FB);
    display.println(dir_LR);
    display.setTextSize(1);
    display.print("V_Left: ");
    display.println(V_Left);
    display.print("V_Right: ");
    display.println(V_Right);
    display.println("Buttons: ");

    // Check and print pressed buttons
    for (int i = 0; i < 5; i++) {
      if (buttons[i] != '0') {
        display.setTextSize(2);
        display.print(buttons[i]);
      }
    }
  } else {
    display.setTextSize(2);
    display.println("Sleep Mode");
  }
  

  display.display();

  delay(50);
}
