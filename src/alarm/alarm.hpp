#ifndef ALARM_H
#define ALARM_H

#include "Arduino.h"

#define ALARM_PIN 39
#define ALARM_DURATION_MS 2000

void setupAlarmPin(void);
void setOffAlarm(void);
void turnOffAlarm(void);

#endif