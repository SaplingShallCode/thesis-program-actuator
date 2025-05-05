#include <Arduino.h>
#include "motor.hpp"
#include "tof/tof.hpp"

#define MOTOR_TIME_MS 1000

static bool targetAReached = false;
static bool targetBReached = false;
static uint8_t targetPosition = 1;

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


void setTargetPos(uint8_t newTarget) {
    targetPosition = newTarget;
}


void setTargetAReached(bool b) {
    targetAReached = b;
}


void setTargetBReached(bool b) {
    targetBReached = b;
}


/**The motor event loop handles how the motor behaves at different events.
 * 
 */
void motorEvent(void * param) {

    for (;;) {

        if (targetAReached && rackAPosTracker != targetPosition) {
            targetAReached = false;
        }

        if (targetBReached && rackBPosTracker != targetPosition) {
            targetBReached = false;
        }



        if (rackAPosTracker < targetPosition) {
            // Extend only if target is not reached, Otherwise trigger alarm
            
            if (!targetAReached) {
                doExtend('A');
            } else {

            }

        }
        else if (rackAPosTracker > targetPosition) {
            // Retract only if target is not reached, Otherwise trigger alarm
            if (!targetAReached) {
                doRetract('A');
            } else {

            }
        }
        else if (rackAPosTracker == targetPosition) {
            // Destination reached, Pause motors
            targetAReached = true;
            doPause('A');
        }

        // ====================== RACK B
        if (rackBPosTracker < targetPosition) {
            if (!targetBReached) {
                doExtend('B');
            } else {

            }

        }
        else if (rackBPosTracker > targetPosition) {
            if (!targetBReached) {
                doRetract('B');
            } else {

            }
        }
        else if (rackBPosTracker == targetPosition) {
            targetBReached = true;
            doPause('B');
        }
    }
}