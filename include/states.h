#ifndef STATES_H
#define STATES_H

enum class SystemState {
    IDLE,
    CARD_DETECTED,
    AUTHENTICATING,
    ACCESS_GRANTED,
    DOOR_OPEN,
    ACCESS_DENIED,
    LOCKED_OUT
};

#endif
