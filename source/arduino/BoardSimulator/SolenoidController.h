#ifndef SOLENOIDCONTROLLER_H
#define SOLENOIDCONTROLLER_H

#include <Arduino.h>

class SolenoidController {
public:
    SolenoidController(uint8_t relayPin, uint8_t statusPin = 255);
    void begin();
    void activate();
    void deactivate();
    void pulse(unsigned long duration);
    bool isActive();
    bool getStatus();
    bool selfTest();
    void setActivationTime(unsigned long maxTime);

private:
    uint8_t _relayPin;
    uint8_t _statusPin;
    bool _isActive;
    bool _initialized;
    unsigned long _activationStartTime;
    unsigned long _maxActivationTime;
    unsigned long _pulseEndTime;
    bool _pulseMode;
    
    void updatePulse();
};

#endif // SOLENOIDCONTROLLER_H