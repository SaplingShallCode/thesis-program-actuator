#include "alarm.hpp"
#include "motor/motor.hpp"

void setupAlarmPin(void) {
    pinMode(ALARM_PIN, OUTPUT);
}

void setOffAlarm(void) {

    if (getStateLockA() || getStateLockB()) {
        digitalWrite(ALARM_PIN, HIGH);
        vTaskDelay(ALARM_DURATION_MS / portTICK_PERIOD_MS);
    }
    turnOffAlarm();
}

void turnOffAlarm(void) {
    digitalWrite(ALARM_PIN, LOW);
}