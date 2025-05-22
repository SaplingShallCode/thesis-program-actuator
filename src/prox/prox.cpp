#include "prox.hpp"


/// @note a 0 state means that the sensor detects something.

static int proxAState = 0;
static int proxBState = 0;

void setupProx(void) {
    pinMode(PROX_A_PIN, INPUT);
    pinMode(PROX_B_PIN, INPUT);
} 

void proxLoop(void *param) {
    for (;;) {
        proxAState = digitalRead(PROX_A_PIN);
        proxBState = digitalRead(PROX_B_PIN);
    }
}

int getProxAState(void) {
    return proxAState;
}

int getProxBState(void) {
    return proxAState;
}