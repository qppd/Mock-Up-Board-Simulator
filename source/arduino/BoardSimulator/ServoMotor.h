#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include <Servo.h>

class ServoMotor {
public:
    ServoMotor(uint8_t pin);
    void begin();
    void write(int angle);
    int read();
    void detach();
private:
    Servo _servo;
    uint8_t _pin;
    int _angle;
};

#endif // SERVOMOTOR_H
