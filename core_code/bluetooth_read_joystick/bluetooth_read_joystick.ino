#include <SoftwareSerial.h>
#include <Arduino.h>

#define RX_PIN 12
#define TX_PIN 13

SoftwareSerial bluetoothSerial(RX_PIN, TX_PIN); // RX=12, TX=13 BLUETOOTH MODULE

struct DataPacket {
  char dir;
  int V_Left;
  int V_Right;
  char buttons[5]; // Increased to accommodate Button E
};

void setup() {
  Serial.begin(9600);
  bluetoothSerial.begin(9600);

  Serial.println("Bluetooth communication initialized.");
}

void loop() {
  if (bluetoothSerial.available() >= sizeof(DataPacket)) { // Wait until a complete data packet is available
    // Read the data packet
    DataPacket receivedPacket;
    bluetoothSerial.readBytes((char *)&receivedPacket, sizeof(DataPacket));

    // Parse the data packet
    char dir = receivedPacket.dir;
    int V_Left = receivedPacket.V_Left;
    int V_Right = receivedPacket.V_Right;
    char buttonA = receivedPacket.buttons[0];
    char buttonB = receivedPacket.buttons[1];
    char buttonC = receivedPacket.buttons[2];
    char buttonD = receivedPacket.buttons[3];
    char buttonE = receivedPacket.buttons[4];

    // Print the received data
    Serial.print("Received Dir: ");
    Serial.print(dir);
    Serial.print(" V_Left: ");
    Serial.print(V_Left);
    Serial.print(" V_Right: ");
    Serial.print(V_Right);
    Serial.print(" Buttons: ");
    Serial.print(buttonA);
    Serial.print(buttonB);
    Serial.print(buttonC);
    Serial.print(buttonD);
    Serial.print(buttonE);
    Serial.println();
  }
}
