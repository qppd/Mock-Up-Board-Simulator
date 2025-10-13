#ifndef RFID_RC522_H
#define RFID_RC522_H

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

class RFID_RC522 {
public:
    RFID_RC522(uint8_t ssPin, uint8_t rstPin);
    void begin();
    bool isCardPresent();
    bool readCard();
    String getCardUID();
    String getCardType();
    bool authenticateCard(String knownUID);
    bool selfTest();
    void halt();

private:
    uint8_t _ssPin;
    uint8_t _rstPin;
    MFRC522* _mfrc522;
    bool _initialized;
    String _lastUID;
    String _lastCardType;
    
    String byteArrayToString(byte* buffer, byte bufferSize);
    bool isValidUID(String uid);
};

#endif // RFID_RC522_H