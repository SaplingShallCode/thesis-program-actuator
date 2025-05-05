#include "oled.hpp"

#include "qrcodeoled.h"
#include "SH1106Wire.h"

static bool showDisplay = true;

SH1106Wire display(
    OLED_I2C_ADDRESS,
    OLED_SDA,
    OLED_SDL
);

QRcodeOled qrCodeHandle(&display);

void setupOLED(void) {
    Serial.println("SetupOLED");
    qrCodeHandle.init();
    vTaskDelay(100/portTICK_PERIOD_MS);
    display.init();
    vTaskDelay(100/portTICK_PERIOD_MS);
    display.clear();
    vTaskDelay(100/portTICK_PERIOD_MS);
    display.display();
    vTaskDelay(100/portTICK_PERIOD_MS);
    Serial.println("SetupOLED OK");
}


void updateQRCode(String message) {
    qrCodeHandle.create(message);
}


void clearDisplay(void) {
    display.clear();
    display.display();
}


void updateShowDisplay(bool b) {
    showDisplay = b;
}

bool getShowDisplayV(void) {
    return showDisplay;
}