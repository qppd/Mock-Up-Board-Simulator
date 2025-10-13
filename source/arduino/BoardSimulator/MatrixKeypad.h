#ifndef MATRIXKEYPAD_H
#define MATRIXKEYPAD_H

#include <Arduino.h>

class MatrixKeypad {
public:
    MatrixKeypad(uint8_t* rowPins, uint8_t* colPins, char** keymap, uint8_t rows, uint8_t cols);
    void begin();
    char getKey();
    bool isPressed();
    void setDebounceTime(unsigned long debounceTime);
    bool selfTest();

private:
    uint8_t* _rowPins;
    uint8_t* _colPins;
    char** _keymap;
    uint8_t _rows;
    uint8_t _cols;
    unsigned long _debounceTime;
    unsigned long _lastKeyTime;
    char _lastKey;
    bool _keyPressed;
    
    char scanKeys();
    bool isValidKey(char key);
};

#endif // MATRIXKEYPAD_H