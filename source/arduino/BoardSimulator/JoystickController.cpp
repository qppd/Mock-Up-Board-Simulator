#include "JoystickController.h"

JoystickController::JoystickController(uint8_t pinVRx, uint8_t pinVRy, uint8_t pinSW)
    : _pinVRx(pinVRx), _pinVRy(pinVRy), _pinSW(pinSW) {}

void JoystickController::begin() {
    pinMode(_pinSW, INPUT_PULLUP); // Usually joystick switch is active low
}

int JoystickController::readX() {
    return analogRead(_pinVRx);
}

int JoystickController::readY() {
    return analogRead(_pinVRy);
}

bool JoystickController::readSwitch() {
    return digitalRead(_pinSW) == LOW; // Pressed = LOW
}
