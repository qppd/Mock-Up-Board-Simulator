#include "RFID_RC522.h"

RFID_RC522::RFID_RC522(uint8_t ssPin, uint8_t rstPin) {
    _ssPin = ssPin;
    _rstPin = rstPin;
    _mfrc522 = nullptr;
    _initialized = false;
    _lastUID = "";
    _lastCardType = "";
}

void RFID_RC522::begin() {
    SPI.begin();
    _mfrc522 = new MFRC522(_ssPin, _rstPin);
    _mfrc522->PCD_Init();
    _initialized = true;
    Serial.println("RFID_RC522: Initialized");
}

bool RFID_RC522::isCardPresent() {
    if (!_initialized) return false;
    return _mfrc522->PICC_IsNewCardPresent();
}

bool RFID_RC522::readCard() {
    if (!_initialized) return false;
    
    if (!_mfrc522->PICC_IsNewCardPresent()) {
        return false;
    }
    
    if (!_mfrc522->PICC_ReadCardSerial()) {
        return false;
    }
    
    // Get UID
    _lastUID = byteArrayToString(_mfrc522->uid.uidByte, _mfrc522->uid.size);
    
    // Get card type
    MFRC522::PICC_Type piccType = _mfrc522->PICC_GetType(_mfrc522->uid.sak);
    _lastCardType = _mfrc522->PICC_GetTypeName(piccType);
    
    return true;
}

String RFID_RC522::getCardUID() {
    return _lastUID;
}

String RFID_RC522::getCardType() {
    return _lastCardType;
}

bool RFID_RC522::authenticateCard(String knownUID) {
    if (_lastUID.length() == 0) return false;
    return (_lastUID.equals(knownUID));
}

bool RFID_RC522::selfTest() {
    if (!_initialized) return false;
    
    byte version = _mfrc522->PCD_ReadRegister(_mfrc522->VersionReg);
    return (version == 0x91 || version == 0x92); // Valid MFRC522 versions
}

void RFID_RC522::halt() {
    if (_initialized && _mfrc522) {
        _mfrc522->PICC_HaltA();
    }
}

String RFID_RC522::byteArrayToString(byte* buffer, byte bufferSize) {
    String uid = "";
    for (byte i = 0; i < bufferSize; i++) {
        if (i > 0) uid += ":";
        if (buffer[i] < 0x10) uid += "0";
        uid += String(buffer[i], HEX);
    }
    uid.toUpperCase();
    return uid;
}

bool RFID_RC522::isValidUID(String uid) {
    return (uid.length() > 0 && uid != "00:00:00:00");
}