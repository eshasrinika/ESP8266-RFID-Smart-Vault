#include "rfid_manager.h"

RFIDManager::RFIDManager(uint8_t ssPin, uint8_t rstPin) 
    : _mfrc522(ssPin, rstPin) {}

void RFIDManager::begin() {
    SPI.begin();
    _mfrc522.PCD_Init();
}

bool RFIDManager::isCardPresent() {
    // Look for new cards and select one
    if (!_mfrc522.PICC_IsNewCardPresent()) return false;
    if (!_mfrc522.PICC_ReadCardSerial()) return false;
    return true;
}

String RFIDManager::getUID() {
    String uid = "";
    for (byte i = 0; i < _mfrc522.uid.size; i++) {
        uid += String(_mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
        uid += String(_mfrc522.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();
    return uid;
}

void RFIDManager::halt() {
    _mfrc522.PICC_HaltA();
    _mfrc522.PCD_StopCrypto1();
}
