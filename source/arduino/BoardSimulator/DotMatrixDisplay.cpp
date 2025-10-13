#include "DotMatrixDisplay.h"

// Character patterns for 8x8 LED matrix (basic ASCII characters)
static uint8_t charPatterns[][8] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // Space
    {0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00}, // !
    {0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // "
    {0x7E, 0x81, 0xA5, 0x81, 0xBD, 0x99, 0x81, 0x7E}, // # (smiley)
    {0x3E, 0x7F, 0x7B, 0x7F, 0x7E, 0x6F, 0x7F, 0x3E}, // $ (heart)
    {0x00, 0x18, 0x3C, 0x7E, 0x7E, 0x3C, 0x18, 0x00}, // % (diamond)
    // Numbers 0-9
    {0x3C, 0x66, 0x6E, 0x76, 0x66, 0x66, 0x3C, 0x00}, // 0
    {0x18, 0x18, 0x38, 0x18, 0x18, 0x18, 0x7E, 0x00}, // 1
    {0x3C, 0x66, 0x06, 0x0C, 0x30, 0x60, 0x7E, 0x00}, // 2
    {0x3C, 0x66, 0x06, 0x1C, 0x06, 0x66, 0x3C, 0x00}, // 3
    {0x06, 0x0E, 0x1E, 0x66, 0x7F, 0x06, 0x06, 0x00}, // 4
    {0x7E, 0x60, 0x7C, 0x06, 0x06, 0x66, 0x3C, 0x00}, // 5
    {0x3C, 0x66, 0x60, 0x7C, 0x66, 0x66, 0x3C, 0x00}, // 6
    {0x7E, 0x66, 0x0C, 0x18, 0x18, 0x18, 0x18, 0x00}, // 7
    {0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00}, // 8
    {0x3C, 0x66, 0x66, 0x3E, 0x06, 0x66, 0x3C, 0x00}, // 9
    // Letters A-Z (basic patterns)
    {0x18, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00}, // A
    {0x7C, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x7C, 0x00}, // B
    {0x3C, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3C, 0x00}, // C
    {0x78, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0x78, 0x00}, // D
    {0x7E, 0x60, 0x60, 0x78, 0x60, 0x60, 0x7E, 0x00}, // E
    {0x7E, 0x60, 0x60, 0x78, 0x60, 0x60, 0x60, 0x00}, // F
};

DotMatrixDisplay::DotMatrixDisplay(uint8_t dataPin, uint8_t clockPin, uint8_t csPin, uint8_t numDevices) {
    _dataPin = dataPin;
    _clockPin = clockPin;
    _csPin = csPin;
    _numDevices = numDevices;
    _lc = nullptr;
    _initialized = false;
}

void DotMatrixDisplay::begin() {
    _lc = new LedControl(_dataPin, _clockPin, _csPin, _numDevices);
    
    for (int device = 0; device < _numDevices; device++) {
        _lc->shutdown(device, false); // Wake up display
        _lc->setIntensity(device, 8); // Set brightness (0-15)
        _lc->clearDisplay(device);    // Clear display
    }
    
    _initialized = true;
    Serial.println("DotMatrixDisplay: Initialized");
}

void DotMatrixDisplay::clear() {
    if (!_initialized) return;
    
    for (int device = 0; device < _numDevices; device++) {
        _lc->clearDisplay(device);
    }
}

void DotMatrixDisplay::setIntensity(uint8_t intensity) {
    if (!_initialized) return;
    
    intensity = constrain(intensity, 0, 15);
    for (int device = 0; device < _numDevices; device++) {
        _lc->setIntensity(device, intensity);
    }
}

void DotMatrixDisplay::displayChar(char character) {
    if (!_initialized) return;
    
    uint8_t* pattern = getCharPattern(character);
    if (pattern != nullptr) {
        for (int row = 0; row < 8; row++) {
            _lc->setRow(0, row, pattern[row]);
        }
    }
}

void DotMatrixDisplay::displayString(String text, unsigned long scrollDelay) {
    if (!_initialized || text.length() == 0) return;
    
    if (text.length() == 1) {
        displayChar(text.charAt(0));
    } else {
        scrollText(text, scrollDelay);
    }
}

void DotMatrixDisplay::setPixel(uint8_t row, uint8_t col, bool state) {
    if (!_initialized) return;
    
    row = constrain(row, 0, 7);
    col = constrain(col, 0, 7);
    
    _lc->setLed(0, row, col, state);
}

void DotMatrixDisplay::displayPattern(uint8_t pattern[8]) {
    if (!_initialized) return;
    
    for (int row = 0; row < 8; row++) {
        _lc->setRow(0, row, pattern[row]);
    }
}

void DotMatrixDisplay::scrollText(String text, unsigned long delay_ms) {
    if (!_initialized) return;
    
    // Simple scrolling implementation
    for (int i = 0; i < text.length(); i++) {
        displayChar(text.charAt(i));
        delay(delay_ms);
    }
}

bool DotMatrixDisplay::selfTest() {
    if (!_initialized) return false;
    
    // Test by lighting up all LEDs briefly
    for (int row = 0; row < 8; row++) {
        _lc->setRow(0, row, 0xFF);
    }
    delay(500);
    
    clear();
    delay(200);
    
    // Test individual pixels
    setPixel(0, 0, true);
    setPixel(7, 7, true);
    delay(300);
    
    clear();
    return true;
}

void DotMatrixDisplay::shutdown(bool enable) {
    if (!_initialized) return;
    
    for (int device = 0; device < _numDevices; device++) {
        _lc->shutdown(device, enable);
    }
}

void DotMatrixDisplay::displayByte(uint8_t row, uint8_t data) {
    if (!_initialized) return;
    _lc->setRow(0, row, data);
}

uint8_t* DotMatrixDisplay::getCharPattern(char character) {
    if (character >= '0' && character <= '9') {
        return charPatterns[character - '0' + 6]; // Numbers start at index 6
    } else if (character >= 'A' && character <= 'F') {
        return charPatterns[character - 'A' + 16]; // Letters start at index 16
    } else if (character >= 'a' && character <= 'f') {
        return charPatterns[character - 'a' + 16]; // Convert lowercase to uppercase
    } else if (character == ' ') {
        return charPatterns[0]; // Space
    } else {
        return charPatterns[1]; // Default to '!' for unknown characters
    }
}

bool DotMatrixDisplay::isValidChar(char character) {
    return (character >= '0' && character <= '9') ||
           (character >= 'A' && character <= 'Z') ||
           (character >= 'a' && character <= 'z') ||
           (character == ' ');
}