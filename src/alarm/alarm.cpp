#include "alarm.hpp"

void setupAlarmPin(void) {
    pinMode(ALARM_PIN, OUTPUT);
}

void setOffAlarm(void) {
    digitalWrite(ALARM_PIN, HIGH);
    vTaskDelay(ALARM_DURATION_MS / portTICK_PERIOD_MS);
    turnOffAlarm();
    // digitalWrite(ALARM_PIN, LOW);
}


void turnOffAlarm(void) {
    digitalWrite(ALARM_PIN, LOW);
}