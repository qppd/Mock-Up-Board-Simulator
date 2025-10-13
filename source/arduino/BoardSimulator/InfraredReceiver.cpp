#include "InfraredReceiver.h"

InfraredReceiver::InfraredReceiver(uint8_t pin) {
    _pin = pin;
    _irReceiver = nullptr;
    _initialized = false;
    _repeatsEnabled = false;
    _lastCode = 0;
    _lastReceiveTime = 0;
}

void InfraredReceiver::begin() {
    _irReceiver = new IRrecv(_pin);
    _irReceiver->enableIRIn();
    _initialized = true;
    Serial.println("InfraredReceiver: Initialized");
}

bool InfraredReceiver::isDataAvailable() {
    if (!_initialized) return false;
    return _irReceiver->decode();
}

uint32_t InfraredReceiver::readCode() {
    if (!_initialized || !_irReceiver->decode()) {
        return 0;
    }
    
    uint32_t code = _irReceiver->decodedIRData.decodedRawData;
    _irReceiver->resume(); // Prepare for next value
    
    // Handle repeat codes
    if (!_repeatsEnabled && code == _lastCode && 
        (millis() - _lastReceiveTime) < 200) {
        return 0; // Ignore repeat within 200ms
    }
    
    _lastCode = code;
    _lastReceiveTime = millis();
    
    return code;
}

String InfraredReceiver::decodeCommand(uint32_t code) {
    // Common IR remote codes (NEC protocol)
    switch (code) {
        case 0xFF629D: return "POWER";
        case 0xFF22DD: return "PREV";
        case 0xFF02FD: return "NEXT";
        case 0xFFC23D: return "PLAY_PAUSE";
        case 0xFFE01F: return "VOL_DOWN";
        case 0xFFA857: return "VOL_UP";
        case 0xFF906F: return "EQ";
        case 0xFF6897: return "0";
        case 0xFF30CF: return "1";
        case 0xFF18E7: return "2";
        case 0xFF7A85: return "3";
        case 0xFF10EF: return "4";
        case 0xFF38C7: return "5";
        case 0xFF5AA5: return "6";
        case 0xFF42BD: return "7";
        case 0xFF4AB5: return "8";
        case 0xFF52AD: return "9";
        default: 
            return "UNKNOWN_0x" + String(code, HEX);
    }
}

bool InfraredReceiver::selfTest() {
    if (!_initialized) return false;
    
    // Check if IR receiver is responding
    pinMode(_pin, INPUT);
    int reading = digitalRead(_pin);
    return (reading == HIGH); // IR receiver should be HIGH when idle
}

void InfraredReceiver::enableRepeats(bool enable) {
    _repeatsEnabled = enable;
}

bool InfraredReceiver::isValidCode(uint32_t code) {
    return (code != 0 && code != 0xFFFFFFFF);
}