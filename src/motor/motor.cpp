#include <Arduino.h>
#include "motor.hpp"
#include "prox/prox.hpp"
// #include "tof/tof.hpp"

#define MOTOR_TIME_MS 1000

static bool targetAReached = false;
static bool targetBReached = false;
static bool stateLockA  = false;
static bool stateLockB  = false;

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

[[deprecated("Implementation not used.")]]
void setTargetAReached(bool b) {
    targetAReached = b;
}

[[deprecated("Implementation not used.")]]
void setTargetBReached(bool b) {
    targetBReached = b;
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
            }
            else {
                doPause('A');
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
                doExtend('B');
            }
            else {
                doPause('B');
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