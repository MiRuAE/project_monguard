#ifndef ADCControl_h
#define ADCControl_h

#include <Arduino.h>

class ADCControl {
  public:
    void init();
    int read(uint8_t channel);
};

#endif
