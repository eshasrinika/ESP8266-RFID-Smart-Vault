#ifndef SERVO_MANAGER_H
#define SERVO_MANAGER_H

#include <Servo.h>

class ServoManager {
public:
    ServoManager();
    void begin(uint8_t pin); // Updated to accept pin
    void lock();
    void unlock();

private:
    Servo _servo;
    uint8_t _pin;
    const int LOCKED_POS = 0;
    const int UNLOCKED_POS = 90;
};

#endif
