#include "lcd_manager.h"

LCDManager::LCDManager(uint8_t addr, uint8_t cols, uint8_t rows) 
    : _lcd(addr, cols, rows) {}

void LCDManager::begin() {
    _lcd.init();
    _lcd.backlight();
    displayWelcome();
}

void LCDManager::displayWelcome() {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("     VAULT      ");
    _lcd.setCursor(0, 1);
    _lcd.print("READY TO SCAN ");
}

void LCDManager::displayStatus(SystemState state) {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    
    switch(state) {
        case SystemState::AUTHENTICATING:
            _lcd.print("Authenticating...");
            break;
        case SystemState::ACCESS_DENIED:
            _lcd.print("ACCESS DENIED");
            _lcd.setCursor(0, 1);
            _lcd.print("Unknown Card");
            break;
        case SystemState::LOCKED_OUT:
            _lcd.print("SYSTEM LOCKED");
            break;
        default:
            break;
    }
}

void LCDManager::displayUser(const char* name, const char* role) {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print(name);
    _lcd.setCursor(0, 1);
    _lcd.print(role);
}

void LCDManager::displayLockout(int seconds) {
    _lcd.setCursor(0, 0);
    _lcd.print(" SYSTEM LOCKED ");
    _lcd.setCursor(0, 1);
    _lcd.print("WAIT: ");
    _lcd.print(seconds);
    _lcd.print(" SECS   ");
}

void LCDManager::clear() {
    _lcd.clear();
}
