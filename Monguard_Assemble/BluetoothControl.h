#ifndef BluetoothControl_h
#define BluetoothControl_h

#include <SoftwareSerial.h>

struct DataPacket {
  char DIR_FB;
  char DIR_LR;
  char Mode;
  int V_Left;
  int V_Right;
  char buttons[5];
};

class BluetoothControl {
  public:
    BluetoothControl(int rxPin, int txPin);
    void begin(long baudRate);
    bool readData(DataPacket &packet);

  private:
    SoftwareSerial bluetoothSerial;
};

#endif
