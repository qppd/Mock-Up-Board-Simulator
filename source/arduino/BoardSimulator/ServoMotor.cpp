#include "ServoMotor.h"

ServoMotor::ServoMotor(uint8_t pin) : _pin(pin), _angle(0) {}

void ServoMotor::begin() {
    _servo.attach(_pin);
}

void ServoMotor::write(int angle) {
    _angle = angle;
    _servo.write(angle);
}

int ServoMotor::read() {
    return _servo.read();
}

void ServoMotor::detach() {
    _servo.detach();
}
