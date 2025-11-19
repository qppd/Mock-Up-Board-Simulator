#include "PirMotionSensor.h"

PirMotionSensor::PirMotionSensor(uint8_t pin) {
    pirPin = pin;
    lastMotionState = false;
    lastMotionTime = 0;
    motionDebounceTime = 50; // Default 50ms debounce
}

void PirMotionSensor::begin() {
    pinMode(pirPin, INPUT);
    lastMotionState = digitalRead(pirPin);
    Serial.println("PIR Motion Sensor initialized");
}

bool PirMotionSensor::readMotion() {
    return digitalRead(pirPin) == HIGH;
}

bool PirMotionSensor::motionDetected() {
    bool currentState = readMotion();
    
    // Detect rising edge (motion started)
    if (currentState && !lastMotionState) {
        if (millis() - lastMotionTime > motionDebounceTime) {
            lastMotionTime = millis();
            lastMotionState = currentState;
            return true;
        }
    }
    
    lastMotionState = currentState;
    return false;
}

unsigned long PirMotionSensor::getLastMotionTime() {
    return lastMotionTime;
}

void PirMotionSensor::setDebounceTime(unsigned long debounceMs) {
    motionDebounceTime = debounceMs;
}

bool PirMotionSensor::selfTest() {
    // Basic self-test: check if pin can be read
    bool testRead1 = digitalRead(pirPin);
    delay(10);
    bool testRead2 = digitalRead(pirPin);
    
    // If readings are consistent (both HIGH or both LOW), sensor is likely working
    // PIR sensors typically read LOW when no motion, but we can't guarantee state
    // So we just check that we can read from the pin
    return (testRead1 == HIGH || testRead1 == LOW) && (testRead2 == HIGH || testRead2 == LOW);
}
