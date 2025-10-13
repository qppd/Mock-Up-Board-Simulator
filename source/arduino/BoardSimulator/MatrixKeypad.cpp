#include "MatrixKeypad.h"

MatrixKeypad::MatrixKeypad(uint8_t* rowPins, uint8_t* colPins, char** keymap, uint8_t rows, uint8_t cols) {
    _rowPins = rowPins;
    _colPins = colPins;
    _keymap = keymap;
    _rows = rows;
    _cols = cols;
    _debounceTime = 50; // Default 50ms debounce
    _lastKeyTime = 0;
    _lastKey = '\0';
    _keyPressed = false;
}

void MatrixKeypad::begin() {
    // Set row pins as outputs and initialize HIGH
    for (uint8_t i = 0; i < _rows; i++) {
        pinMode(_rowPins[i], OUTPUT);
        digitalWrite(_rowPins[i], HIGH);
    }
    
    // Set column pins as inputs with pullup resistors
    for (uint8_t i = 0; i < _cols; i++) {
        pinMode(_colPins[i], INPUT_PULLUP);
    }
}

char MatrixKeypad::getKey() {
    char key = scanKeys();
    
    // Handle debouncing
    if (key != '\0') {
        unsigned long currentTime = millis();
        if (key != _lastKey || (currentTime - _lastKeyTime) > _debounceTime) {
            _lastKey = key;
            _lastKeyTime = currentTime;
            _keyPressed = true;
            return key;
        }
    } else {
        _keyPressed = false;
    }
    
    return '\0';
}

bool MatrixKeypad::isPressed() {
    return _keyPressed;
}

void MatrixKeypad::setDebounceTime(unsigned long debounceTime) {
    _debounceTime = debounceTime;
}

char MatrixKeypad::scanKeys() {
    // Scan each row
    for (uint8_t row = 0; row < _rows; row++) {
        // Set current row LOW, others HIGH
        for (uint8_t i = 0; i < _rows; i++) {
            digitalWrite(_rowPins[i], (i == row) ? LOW : HIGH);
        }
        
        // Small delay for signal stabilization
        delayMicroseconds(10);
        
        // Check each column
        for (uint8_t col = 0; col < _cols; col++) {
            if (digitalRead(_colPins[col]) == LOW) {
                // Key is pressed at this row/col
                return _keymap[row][col];
            }
        }
    }
    
    // No key pressed
    return '\0';
}

bool MatrixKeypad::isValidKey(char key) {
    for (uint8_t row = 0; row < _rows; row++) {
        for (uint8_t col = 0; col < _cols; col++) {
            if (_keymap[row][col] == key) {
                return true;
            }
        }
    }
    return false;
}

bool MatrixKeypad::selfTest() {
    // Test if all pins are properly configured
    for (uint8_t i = 0; i < _rows; i++) {
        if (digitalRead(_rowPins[i]) != HIGH) {
            return false;
        }
    }
    
    for (uint8_t i = 0; i < _cols; i++) {
        if (digitalRead(_colPins[i]) != HIGH) {
            return false;
        }
    }
    
    return true;
}