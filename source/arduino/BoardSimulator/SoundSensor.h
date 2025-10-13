#ifndef SOUNDSENSOR_H
#define SOUNDSENSOR_H

#include <Arduino.h>

class SoundSensor {
public:
    SoundSensor(uint8_t analogPin, uint8_t digitalPin, uint16_t threshold = 512);
    void begin();
    int readAnalog();
    bool readDigital();
    bool isSoundDetected();
    int getAverageLevel(int samples = 10);
    int getPeakLevel();
    void setThreshold(uint16_t threshold);
    uint16_t getThreshold();
    bool selfTest();
    void calibrate();

private:
    uint8_t _analogPin;
    uint8_t _digitalPin;
    uint16_t _threshold;
    bool _initialized;
    int _peakLevel;
    unsigned long _lastPeakTime;
    int _baselineLevel;
    
    void updatePeak(int level);
    bool isValidReading(int reading);
};

#endif // SOUNDSENSOR_H