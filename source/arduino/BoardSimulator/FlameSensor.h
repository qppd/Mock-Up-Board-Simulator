#ifndef FLAMESENSOR_H
#define FLAMESENSOR_H

#include <Arduino.h>

class FlameSensor {
public:
    FlameSensor(uint8_t analogPin, uint8_t digitalPin);
    void begin();
    int readAnalog();
    bool readDigital();
private:
    uint8_t _analogPin;
    uint8_t _digitalPin;
};

#endif // FLAMESENSOR_H
