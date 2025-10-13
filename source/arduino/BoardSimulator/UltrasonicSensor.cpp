#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(uint8_t trigPin, uint8_t echoPin, unsigned long maxDistance) {
    _trigPin = trigPin;
    _echoPin = echoPin;
    _maxDistance = maxDistance;
    _initialized = false;
    _lastDistance = 0;
    _timeout = (maxDistance * 2 * 10) / 343 * 1000; // Calculate timeout in microseconds
}

void UltrasonicSensor::begin() {
    pinMode(_trigPin, OUTPUT);
    pinMode(_echoPin, INPUT);
    digitalWrite(_trigPin, LOW);
    
    _initialized = true;
    Serial.println("UltrasonicSensor: Initialized");
}

long UltrasonicSensor::readDistance() {
    if (!_initialized) return -1;
    
    long distance = measureDistance();
    _lastDistance = distance;
    return distance;
}

long UltrasonicSensor::readDistanceFiltered(uint8_t samples) {
    if (!_initialized) return -1;
    
    long total = 0;
    uint8_t validReadings = 0;
    
    for (uint8_t i = 0; i < samples; i++) {
        long distance = measureDistance();
        if (isValidDistance(distance)) {
            total += distance;
            validReadings++;
        }
        delay(10); // Small delay between readings
    }
    
    if (validReadings == 0) return -1;
    
    _lastDistance = total / validReadings;
    return _lastDistance;
}

bool UltrasonicSensor::isObjectDetected(long threshold) {
    if (!_initialized) return false;
    
    long distance = readDistance();
    return (distance > 0 && distance < threshold);
}

bool UltrasonicSensor::isInRange() {
    if (!_initialized) return false;
    
    long distance = readDistance();
    return isValidDistance(distance);
}

long UltrasonicSensor::getAverageDistance(uint8_t samples) {
    return readDistanceFiltered(samples);
}

void UltrasonicSensor::setMaxDistance(unsigned long maxDistance) {
    _maxDistance = maxDistance;
    _timeout = (maxDistance * 2 * 10) / 343 * 1000; // Recalculate timeout
}

unsigned long UltrasonicSensor::getMaxDistance() {
    return _maxDistance;
}

bool UltrasonicSensor::selfTest() {
    if (!_initialized) return false;
    
    // Test trigger pin
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trigPin, LOW);
    
    // Check if echo pin responds
    unsigned long startTime = micros();
    while (digitalRead(_echoPin) == LOW && (micros() - startTime) < 1000) {
        // Wait for echo to go HIGH or timeout
    }
    
    if ((micros() - startTime) >= 1000) {
        return false; // Echo pin didn't respond
    }
    
    // Take a test reading
    long testDistance = measureDistance();
    return (testDistance >= 0); // Valid reading indicates working sensor
}

long UltrasonicSensor::measureDistance() {
    // Clear trigger pin
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2);
    
    // Send 10us pulse
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trigPin, LOW);
    
    // Measure echo pulse duration
    unsigned long duration = pulseIn(_echoPin, HIGH, _timeout);
    
    if (duration == 0) {
        return -1; // No echo received (out of range or error)
    }
    
    // Calculate distance in cm
    // Speed of sound = 343 m/s = 0.0343 cm/μs
    // Distance = (duration / 2) * 0.0343
    long distance = (duration * 0.0343) / 2;
    
    // Validate distance
    if (!isValidDistance(distance)) {
        return -1;
    }
    
    return distance;
}

bool UltrasonicSensor::isValidDistance(long distance) {
    return (distance > 2 && distance <= _maxDistance); // Min 2cm to avoid false readings
}

long UltrasonicSensor::filterReading(long newReading) {
    // Simple filter - if reading differs significantly from last, take average
    if (_lastDistance > 0 && abs(newReading - _lastDistance) > 50) {
        return (_lastDistance + newReading) / 2;
    }
    return newReading;
}