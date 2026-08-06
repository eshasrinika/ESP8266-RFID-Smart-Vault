#include "servo_manager.h"
#include <Arduino.h>

ServoManager::ServoManager() : _pin(0) {}

void ServoManager::begin(uint8_t pin) {
    _pin = pin;
    _servo.attach(_pin);
    lock(); // Ensure it starts locked
}

void ServoManager::lock() {
    _servo.write(LOCKED_POS);
}

void ServoManager::unlock() {
    _servo.write(UNLOCKED_POS);
}
