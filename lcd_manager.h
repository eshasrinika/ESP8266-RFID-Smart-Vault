#ifndef LCD_MANAGER_H
#define LCD_MANAGER_H

#include <LiquidCrystal_I2C.h>
#include "states.h"

class LCDManager {
public:
    LCDManager(uint8_t addr = 0x27, uint8_t cols = 16, uint8_t rows = 2);

    void begin();
    void displayWelcome();
    void displayStatus(SystemState state);
    void displayUser(const char* name, const char* role);
    void displayLockout(int seconds);
    void clear();

private:
    LiquidCrystal_I2C _lcd;
};

#endif
