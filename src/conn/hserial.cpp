#include "hserial.hpp"
#include "oled/oled.hpp"
#include "motor/motor.hpp"
#include "alarm/alarm.hpp"

HardwareSerial mainBoard(1);
static bool isRunning = false;

void sendMsgToMain(String msg) {
    mainBoard.println(msg);
    mainBoard.flush();
}

/**Setup the serial for communicating with the actuator 
 * board. the actuator board will handle most of the 
 * required peripherals for the project. */
void setupHardwareSerial(void) {

    pinMode(SSERIAL_RX, INPUT);
    pinMode(SSERIAL_TX, OUTPUT);
    
    mainBoard.begin(38400,
                    SERIAL_8N1,
                    SSERIAL_RX,
                    SSERIAL_TX,
                    false
                    );

    while (!mainBoard) {
        Serial.println("Software Serial Error");
        delay(1000);
    }
}



void uartLoop(void *param) {
    Serial.println("START UART LOOP TASK");
    for (;;) {
        if (mainBoard.available()) {
            String msg = mainBoard.readStringUntil('\n');
            
            /**NOTES:
             * When using terminator character \n ->
             * Use \r for checking.
            **/
    
            /**Wait for main board if for some reason,
             * the actuator board initializes first. */ 
            if (msg == String("MAIN_OK\r")) {
                isRunning = true;
                sendMsgToMain(ACTUATOR_OK);
            }
    
            if (isRunning) {
    
                if (msg == String("DISPLAY_QR\r")) {
                    Serial.println("DISPLAY_QR");
                    String qrCode = mainBoard.readStringUntil('\n');
                    Serial.println(qrCode);
                    updateQRCode(qrCode);
                }
                
                if (msg == String("LOCK\r")) {
                    Serial.println("LOCK");
                    setTargetAReached(false);
                    setTargetBReached(false);
                    setTargetPos(3);
                }
                
                if (msg == String("UNLOCK\r")) {
                    Serial.println("UNLOCK");
                    setTargetAReached(false);
                    setTargetBReached(false);
                    setTargetPos(1);                    
                }
                
                if (msg == String("SHOW_DISPLAY_TRUE\r")) {
                    Serial.println("SHOW_DISPLAY_TRUE");
                    updateShowDisplay(true);
                }
                
                if (msg == String("SHOW_DISPLAY_FALSE\r")) {
                    Serial.println("SHOW_DISPLAY_FALSE");
                    updateShowDisplay(false);
                }
                
                if (msg == String("CLEAR_DISPLAY\r")) {
                    Serial.println("CLEAR_DISPLAY");
                    clearDisplay();
                }
                
                if (msg == String("ALARM_ON\r")) {
                    Serial.println("ALARM_ON");
                    setOffAlarm();
                }
                if (msg == String("ALARM_OFF\r")) {
                    Serial.println("ALARM_OFF");
                    turnOffAlarm();
                }
            }
        }
    }
    vTaskDelete(NULL);
}