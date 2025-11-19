#ifndef PIR_MOTION_SENSOR_H
#define PIR_MOTION_SENSOR_H

#include <Arduino.h>

class PirMotionSensor {
private:
    uint8_t pirPin;
    bool lastMotionState;
    unsigned long lastMotionTime;
    unsigned long motionDebounceTime;

public:
    PirMotionSensor(uint8_t pin);
    void begin();
    bool readMotion();
    bool motionDetected();
    unsigned long getLastMotionTime();
    void setDebounceTime(unsigned long debounceMs);
};

#endif // PIR_MOTION_SENSOR_H
