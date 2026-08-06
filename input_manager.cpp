#include "input_manager.h"

InputManager::InputManager(uint8_t pin) 
    : _pin(pin), _lastReading(HIGH), _confirmedState(HIGH), _lastDebounceTime(0) {}

void InputManager::begin() {
    pinMode(_pin, INPUT_PULLUP);
}

// Fixed return type to match the header file
unsigned long InputManager::isButtonPressed() {
    bool currentReading = digitalRead(_pin);
    static unsigned long pressStartTime = 0;
    unsigned long duration = 0;

    if (currentReading != _lastReading) {
        _lastDebounceTime = millis();
    }

    if ((millis() - _lastDebounceTime) > _debounceDelay) {
        if (currentReading != _confirmedState) {
            _confirmedState = currentReading;
            
            if (_confirmedState == LOW) {
                // Button just went DOWN
                pressStartTime = millis();
            } else {
                // Button just went UP
                duration = millis() - pressStartTime;
            }
        }
    }

    _lastReading = currentReading;
    return duration;
}
