#ifndef OLED_HPP
#define OLED_HPP

#include <Arduino.h>

#define OLED_SDA                33
#define OLED_SDL                35
#define OLED_I2C_ADDRESS      0x3c

void setupOLED(void);
void updateQRCode(String);
void clearDisplay(void);
void updateShowDisplay(bool);
bool getShowDisplayV(void);

#endif