#ifndef DOTMATRIXDISPLAY_H
#define DOTMATRIXDISPLAY_H

#include <Arduino.h>
#include <LedControl.h>

class DotMatrixDisplay {
public:
    DotMatrixDisplay(uint8_t dataPin, uint8_t clockPin, uint8_t csPin, uint8_t numDevices = 1);
    void begin();
    void clear();
    void setIntensity(uint8_t intensity);
    void displayChar(char character);
    void displayString(String text, unsigned long scrollDelay = 200);
    void setPixel(uint8_t row, uint8_t col, bool state);
    void displayPattern(uint8_t pattern[8]);
    void scrollText(String text, unsigned long delay_ms = 200);
    bool selfTest();
    void shutdown(bool enable);

private:
    uint8_t _dataPin;
    uint8_t _clockPin;
    uint8_t _csPin;
    uint8_t _numDevices;
    LedControl* _lc;
    bool _initialized;
    
    void displayByte(uint8_t row, uint8_t data);
    uint8_t* getCharPattern(char character);
    bool isValidChar(char character);
};

#endif // DOTMATRIXDISPLAY_H