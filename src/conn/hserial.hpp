#ifndef HSERIAL_H
#define HSERIAL_H

#include <Arduino.h>
#include "HardwareSerial.h"

// PCB Config: 
// MainBoard    : (tx:25 | rx:33)
// ActuatorBoard: (tx:11 | rx: 12)
#define SSERIAL_TX 11
#define SSERIAL_RX 12

void sendMsgToMain(String);
void setupHardwareSerial(void);
void uartLoop(void *param);


// UART COMMANDS SENT:
#define ACTUATOR_OK "ACTUATOR_OK"

// UART COMMANDS RECV:

#endif