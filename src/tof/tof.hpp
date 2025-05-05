#ifndef TOF_H
#define TOF_H

#include <Arduino.h>
#include "Adafruit_VL53L0X.h"

extern uint8_t rackAPosTracker;
extern uint8_t rackBPosTracker; 

void setupTof(void);
void tofLoop(void *);

#endif