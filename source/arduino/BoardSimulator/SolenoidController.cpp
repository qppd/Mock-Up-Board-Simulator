#include "SolenoidController.h"

SolenoidController::SolenoidController(uint8_t relayPin, uint8_t statusPin) {
    _relayPin = relayPin;
    _statusPin = statusPin;
    _isActive = false;
    _initialized = false;
    _activationStartTime = 0;
    _maxActivationTime = 10000; // Default 10 seconds max activation
    _pulseEndTime = 0;
    _pulseMode = false;
}

void SolenoidController::begin() {
    pinMode(_relayPin, OUTPUT);
    digitalWrite(_relayPin, LOW); // Ensure solenoid is off initially
    
    if (_statusPin != 255) {
        pinMode(_statusPin, INPUT_PULLUP);
    }
    
    _initialized = true;
    Serial.println("SolenoidController: Initialized");
}

void SolenoidController::activate() {
    if (!_initialized) return;
    
    digitalWrite(_relayPin, HIGH);
    _isActive = true;
    _activationStartTime = millis();
    _pulseMode = false;
    
    Serial.println("Solenoid: ACTIVATED");
}

void SolenoidController::deactivate() {
    if (!_initialized) return;
    
    digitalWrite(_relayPin, LOW);
    _isActive = false;
    _pulseMode = false;
    
    Serial.println("Solenoid: DEACTIVATED");
}

void SolenoidController::pulse(unsigned long duration) {
    if (!_initialized) return;
    
    activate();
    _pulseMode = true;
    _pulseEndTime = millis() + duration;
    
    Serial.print("Solenoid: PULSE for ");
    Serial.print(duration);
    Serial.println("ms");
}

bool SolenoidController::isActive() {
    if (!_initialized) return false;
    
    // Update pulse mode
    updatePulse();
    
    // Safety timeout check
    if (_isActive && (millis() - _activationStartTime) > _maxActivationTime) {
        Serial.println("Solenoid: Safety timeout - force deactivation");
        deactivate();
    }
    
    return _isActive;
}

bool SolenoidController::getStatus() {
    if (!_initialized || _statusPin == 255) {
        return _isActive; // Return relay state if no status pin
    }
    
    return digitalRead(_statusPin) == LOW; // Assuming active-low status
}

bool SolenoidController::selfTest() {
    if (!_initialized) return false;
    
    // Test relay switching
    bool initialState = digitalRead(_relayPin);
    
    digitalWrite(_relayPin, HIGH);
    delay(10);
    bool highState = digitalRead(_relayPin);
    
    digitalWrite(_relayPin, LOW);
    delay(10);
    bool lowState = digitalRead(_relayPin);
    
    // Restore initial state
    digitalWrite(_relayPin, initialState);
    
    return (highState == HIGH && lowState == LOW);
}

void SolenoidController::setActivationTime(unsigned long maxTime) {
    _maxActivationTime = maxTime;
}

void SolenoidController::updatePulse() {
    if (_pulseMode && _isActive && millis() >= _pulseEndTime) {
        deactivate();
    }
}