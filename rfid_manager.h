#ifndef RFID_MANAGER_H
#define RFID_MANAGER_H

#include <SPI.h>
#include <MFRC522.h>

class RFIDManager {
public:
    RFIDManager(uint8_t ssPin, uint8_t rstPin);
    void begin();
    bool isCardPresent();
    String getUID();
    void halt();

private:
    MFRC522 _mfrc522;
};

#endif
