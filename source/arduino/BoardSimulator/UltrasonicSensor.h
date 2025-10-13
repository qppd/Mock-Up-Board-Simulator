#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

#include <Arduino.h>

class UltrasonicSensor {
public:
    UltrasonicSensor(uint8_t trigPin, uint8_t echoPin, unsigned long maxDistance = 400);
    void begin();
    long readDistance();
    long readDistanceFiltered(uint8_t samples = 3);
    bool isObjectDetected(long threshold);
    bool isInRange();
    long getAverageDistance(uint8_t samples = 5);
    void setMaxDistance(unsigned long maxDistance);
    unsigned long getMaxDistance();
    bool selfTest();

private:
    uint8_t _trigPin;
    uint8_t _echoPin;
    unsigned long _maxDistance;
    bool _initialized;
    long _lastDistance;
    unsigned long _timeout;
    
    long measureDistance();
    bool isValidDistance(long distance);
    long filterReading(long newReading);
};

#endif // ULTRASONICSENSOR_H