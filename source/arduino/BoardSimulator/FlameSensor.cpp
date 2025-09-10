#include "FlameSensor.h"

FlameSensor::FlameSensor(uint8_t analogPin, uint8_t digitalPin)
    : _analogPin(analogPin), _digitalPin(digitalPin) {}

void FlameSensor::begin() {
    pinMode(_digitalPin, INPUT);
}

int FlameSensor::readAnalog() {
    return analogRead(_analogPin);
}

bool FlameSensor::readDigital() {
    return digitalRead(_digitalPin) == HIGH;
}
