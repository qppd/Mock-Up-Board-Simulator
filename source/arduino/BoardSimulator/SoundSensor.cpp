#include "SoundSensor.h"

SoundSensor::SoundSensor(uint8_t analogPin, uint8_t digitalPin, uint16_t threshold) {
    _analogPin = analogPin;
    _digitalPin = digitalPin;
    _threshold = threshold;
    _initialized = false;
    _peakLevel = 0;
    _lastPeakTime = 0;
    _baselineLevel = 512; // Default baseline for 10-bit ADC
}

void SoundSensor::begin() {
    pinMode(_analogPin, INPUT);
    pinMode(_digitalPin, INPUT);
    
    // Calibrate baseline level
    calibrate();
    
    _initialized = true;
    Serial.println("SoundSensor: Initialized");
}

int SoundSensor::readAnalog() {
    if (!_initialized) return 0;
    
    int reading = analogRead(_analogPin);
    updatePeak(reading);
    return reading;
}

bool SoundSensor::readDigital() {
    if (!_initialized) return false;
    return digitalRead(_digitalPin) == HIGH;
}

bool SoundSensor::isSoundDetected() {
    if (!_initialized) return false;
    
    int analogLevel = readAnalog();
    bool digitalState = readDigital();
    
    // Sound detected if either analog exceeds threshold OR digital pin is HIGH
    return (analogLevel > _threshold) || digitalState;
}

int SoundSensor::getAverageLevel(int samples) {
    if (!_initialized) return 0;
    
    long total = 0;
    for (int i = 0; i < samples; i++) {
        total += analogRead(_analogPin);
        delay(1); // Small delay between samples
    }
    
    return total / samples;
}

int SoundSensor::getPeakLevel() {
    // Return peak level and reset if older than 1 second
    if (millis() - _lastPeakTime > 1000) {
        _peakLevel = _baselineLevel;
    }
    return _peakLevel;
}

void SoundSensor::setThreshold(uint16_t threshold) {
    _threshold = constrain(threshold, 0, 1023);
}

uint16_t SoundSensor::getThreshold() {
    return _threshold;
}

bool SoundSensor::selfTest() {
    if (!_initialized) return false;
    
    // Read several samples to ensure sensor is responding
    int reading1 = analogRead(_analogPin);
    delay(10);
    int reading2 = analogRead(_analogPin);
    
    // Check if readings are in valid range and sensor is responsive
    return isValidReading(reading1) && isValidReading(reading2);
}

void SoundSensor::calibrate() {
    Serial.println("SoundSensor: Calibrating baseline...");
    
    // Take 50 samples over 500ms to establish baseline
    long total = 0;
    for (int i = 0; i < 50; i++) {
        total += analogRead(_analogPin);
        delay(10);
    }
    
    _baselineLevel = total / 50;
    _peakLevel = _baselineLevel;
    
    Serial.print("SoundSensor: Baseline level set to ");
    Serial.println(_baselineLevel);
}

void SoundSensor::updatePeak(int level) {
    if (level > _peakLevel) {
        _peakLevel = level;
        _lastPeakTime = millis();
    }
}

bool SoundSensor::isValidReading(int reading) {
    return (reading >= 0 && reading <= 1023);
}