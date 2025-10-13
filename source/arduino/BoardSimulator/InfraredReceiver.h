#ifndef INFRAREDRECEIVER_H
#define INFRAREDRECEIVER_H

#include <Arduino.h>
#include <IRremote.h>

class InfraredReceiver {
public:
    InfraredReceiver(uint8_t pin);
    void begin();
    bool isDataAvailable();
    uint32_t readCode();
    String decodeCommand(uint32_t code);
    bool selfTest();
    void enableRepeats(bool enable);

private:
    uint8_t _pin;
    IRrecv* _irReceiver;
    bool _initialized;
    bool _repeatsEnabled;
    uint32_t _lastCode;
    unsigned long _lastReceiveTime;
    
    bool isValidCode(uint32_t code);
};

#endif // INFRAREDRECEIVER_H