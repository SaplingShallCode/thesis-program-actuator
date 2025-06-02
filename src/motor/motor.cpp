#include <Arduino.h>
#include "motor.hpp"
#include "prox/prox.hpp"
#include "alarm/alarm.hpp"

#define MOTOR_TIME_MS 1000

static bool targetAReached = false;
static bool targetBReached = false;
static bool rodAtA = false;
static bool rodAtB = false;
static bool stateLockA  = false;
static bool stateLockB  = false;
static bool motorRunning = false;

void setupMotorPins(void) {
    Serial.println("setupMotorPins");
    
    pinMode(PWM_A, OUTPUT);
    pinMode(PWM_B, OUTPUT);

    pinMode(A_IN1, OUTPUT);
    pinMode(A_IN2, OUTPUT);

    pinMode(B_IN1, OUTPUT);
    pinMode(B_IN2, OUTPUT);

    Serial.println("setupMotorPins OK");
}


[[deprecated("Reimplemented as doExtend()")]]
void moveForward(void *param) {
    Serial.println("    MOTORS MOVING FORWARD");
    analogWrite(PWM_A, 255);
    digitalWrite(A_IN1, HIGH);
    digitalWrite(A_IN2, LOW);
    vTaskDelay(MOTOR_TIME_MS/portTICK_PERIOD_MS);

    stopMotor();
    
    digitalWrite(PWM_B, 255);
    digitalWrite(B_IN1, HIGH);
    digitalWrite(B_IN2, LOW);
    // analogWrite(B_EN_B, 255);
    vTaskDelay(1250/portTICK_PERIOD_MS);
    
    stopMotor();
}


[[deprecated("Reimplemented as doRetract()")]]
void moveBackward(void *param) {
    Serial.println("    MOTORS MOVING BACKWARD");
    analogWrite(PWM_A, 255);
    digitalWrite(A_IN1, LOW);
    digitalWrite(A_IN2, HIGH);

    vTaskDelay(MOTOR_TIME_MS/portTICK_PERIOD_MS);
    stopMotor();
    
    analogWrite(PWM_B, 255);
    digitalWrite(B_IN1, LOW);
    digitalWrite(B_IN2, HIGH);
    // analogWrite(B_EN_B, 255);
    vTaskDelay(1250/portTICK_PERIOD_MS);
    stopMotor();
}


[[deprecated("Reimplemented as doPause()")]]
void stopMotor(void) {
    analogWrite(PWM_A, 255);
    digitalWrite(A_IN1, LOW);
    digitalWrite(A_IN2, LOW);
    
    analogWrite(PWM_B, 255);
    digitalWrite(B_IN1, LOW);
    digitalWrite(B_IN2, LOW);

    Serial.println("    MOTORS STOPPED MOVING");
}


static void doRetract(char rack) {

    switch(rack) {
        case 'a':
        case 'A':
        analogWrite(PWM_A, 255);
        digitalWrite(A_IN1, HIGH);
        digitalWrite(A_IN2, LOW);
        break;
        
        case 'b':
        case 'B':
        digitalWrite(PWM_B, 255);
        digitalWrite(B_IN1, HIGH);
        digitalWrite(B_IN2, LOW); 
        break;
    }
}


static void doExtend(char rack) {
    switch(rack) {
        case 'a':
        case 'A':
        analogWrite(PWM_A, 255);
        digitalWrite(A_IN1, LOW);
        digitalWrite(A_IN2, HIGH);
        break;
        
        case 'b':
        case 'B':
        digitalWrite(PWM_B, 255);
        digitalWrite(B_IN1, LOW);
        digitalWrite(B_IN2, HIGH);
        break;
    }
}


static void doPause(char rack) {
    switch(rack) {
        case 'a':
        case 'A':
        analogWrite(PWM_A, 255);
        digitalWrite(A_IN1, LOW);
        digitalWrite(A_IN2, LOW);
        break;
        
        case 'b':
        case 'B':
        digitalWrite(PWM_B, 255);
        digitalWrite(B_IN1, LOW);
        digitalWrite(B_IN2, LOW);
        break;
    }
}

void setTargetAReached(bool b) {
    targetAReached = b;
}

void setTargetBReached(bool b) {
    targetBReached = b;
}

void setRodAtA(bool b) {
    rodAtA = b;
}

bool getRodAtA(void) {
    return rodAtA;
}

void setRodAtB(bool b) {
    rodAtB = b;
}

bool getRodAtB(void) {
    return rodAtB;
}

void setStateLockA(bool b) {
    stateLockA = b;
}

bool getStateLockA() {
    return stateLockA;
}

void setStateLockB(bool b) {
    stateLockB = b;
}

bool getStateLockB() {
    return stateLockB;
}


/**The motor event loop handles how the motor behaves at different events.
 * 
 */
void motorEvent(void * param) {

    for (;;) {

        /// Motor A
        if (stateLockA) {
            if (getProxAState()) {
                doExtend('A');
                setTargetAReached(false);

                if (getRodAtA()) {
                    forceAlarm();
                }
                else {
                    turnOffAlarm();
                }

            }
            else {
                doPause('A');
                setTargetAReached(true);
                setRodAtA(true);
            }
        }
        else {
            if (!getProxAState()) {
                doRetract('A');
                vTaskDelay(3000 / portTICK_PERIOD_MS);
            }
        }
        
        
        /// Motor B
        if (stateLockB) {
            if (getProxBState()) {
                if (targetAReached) {
                    doExtend('B');
                    
                    if (getRodAtB()) {
                        forceAlarm();
                    }
                    else {
                        turnOffAlarm();
                    }
                }
                else {
                    doPause('B');
                }
                setTargetBReached(false);
            }
            else {
                doPause('B');
                setTargetBReached(true);
                setRodAtB(true);
            }
        }
        else {
            if (!getProxBState()) {
                doRetract('B');
                vTaskDelay(3000 / portTICK_PERIOD_MS);
            }
        }
    }
}