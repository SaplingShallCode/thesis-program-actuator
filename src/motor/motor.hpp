#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "Arduino.h"

/** ++++++++++++++++++++++++++++++
 * Motor Driver Pin Connections
++++++++++++++++++++++++++++++++ */ 

#define PWM_A 9
#define PWM_B 4

#define A_IN1 5
#define A_IN2 7

#define B_IN1 3
#define B_IN2 2

/** ++++++++++++++++++++++++++++++
 * Function Prototypes
++++++++++++++++++++++++++++++++ */ 

void setupMotorPins(void);
void moveForward(void *);
void moveBackward(void *);
void stopMotor(void);
void setTargetPos(uint8_t);
void setTargetAReached(bool);
void setTargetBReached(bool);
void motorEvent(void *);

#endif