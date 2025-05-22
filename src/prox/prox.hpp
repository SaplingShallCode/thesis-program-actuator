#ifndef PROX_H
#define PROX_H

#include <Arduino.h>

#define PROX_A_PIN 13
#define PROX_B_PIN 14

void setupProx(void);
void proxLoop(void *param);
int getProxAState(void);
int getProxBState(void);

#endif