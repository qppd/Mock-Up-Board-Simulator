#ifndef JOYSTICKCONTROLLER_H
#define JOYSTICKCONTROLLER_H

#include <Arduino.h>

class JoystickController {
public:
    JoystickController(uint8_t pinVRx, uint8_t pinVRy, uint8_t pinSW);
    void begin();
    int readX();
    int readY();
    bool readSwitch();
private:
    uint8_t _pinVRx;
    uint8_t _pinVRy;
    uint8_t _pinSW;
};

#endif // JOYSTICKCONTROLLER_H
