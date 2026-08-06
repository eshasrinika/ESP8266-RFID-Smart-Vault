#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <Arduino.h>

class InputManager {
public:
    InputManager(uint8_t pin = 0);
    void begin();
    unsigned long isButtonPressed(); // Now returns duration in ms

private:
    uint8_t _pin;
    bool _lastReading;
    bool _confirmedState;
    unsigned long _lastDebounceTime;
    const unsigned long _debounceDelay = 50;
};
#endif
