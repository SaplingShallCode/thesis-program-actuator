/**Actuator Board Drives:
 * - Relay Module
 * - Oled Module
 * - Motor Driver Module
 * - Time of Flight Sensors
**/


#include <Arduino.h>
#define STARTUP_DELAY_MS 3000; 

#include "conn/hserial.hpp"
#include "motor/motor.hpp"
#include "oled/oled.hpp"
#include "alarm/alarm.hpp"
// #include "tof/tof.hpp"
#include "prox/prox.hpp"

#define SSER_TX 11
#define SSER_RX 12

TaskHandle_t uartLoopTaskHandler;

/**Turns on the onboard led for the the actuator board. */
void showFeedback(uint32_t ms, bool stay) {
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(ms / portTICK_PERIOD_MS);

    if (!stay) {
        digitalWrite(LED_BUILTIN, LOW);
    }
}


void setup(void) {

    pinMode(LED_BUILTIN, OUTPUT);

    // The actuator board has a delayed start
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    
    Serial.begin(115200);
    setupHardwareSerial();
    showFeedback(1000, true);
    
    // setupTof();
    setupProx();
    setupMotorPins();
    setupOLED();
    setupAlarmPin();
    
    xTaskCreatePinnedToCore(
        uartLoop,
        "uartLoop",
        15000,
        NULL,
        1,
        &uartLoopTaskHandler,
        1
    );


    // +++++ To remove, part of v2, TOF Sensor +++++ //
    // xTaskCreatePinnedToCore(
    //     tofLoop,
    //     "tofLoop",
    //     4000,
    //     NULL,
    //     1,
    //     NULL,
    //     1
    // );


    xTaskCreatePinnedToCore(
        proxLoop,
        "proxLoop",
        4000,
        NULL, 
        1,
        NULL, 
        1
    );

    xTaskCreatePinnedToCore(
        motorEvent,
        "motorEvent",
        9000,
        NULL,
        1,
        NULL,
        1
    );
}


void loop(void) {}