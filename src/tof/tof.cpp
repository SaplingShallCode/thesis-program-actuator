#include "tof.hpp"

static Adafruit_VL53L0X tof1 = Adafruit_VL53L0X();
static Adafruit_VL53L0X tof2 = Adafruit_VL53L0X();

static const uint16_t unlockedThresholdAmm = 80;
static const uint16_t lockedThresholdAmm = 30;

static const uint16_t unlockedThresholdBmm = 115;
static const uint16_t lockedThresholdBmm = 35;

/**Rack positions:
 * if value is 1 : Fully retracted (Unlocked)
 * if value is 2 : Middle (should only happen when locking/unlocking state)
 * if value is 3 : Fully extended (Locked) 
 */
uint8_t rackAPosTracker = 1;
uint8_t rackBPosTracker = 1;


#define TOF1_XSHUT 13
#define TOF2_XSHUT 14

void setupTof(void) {

    Serial.println("Setup TOF"); 
    pinMode(TOF1_XSHUT, OUTPUT);
    pinMode(TOF2_XSHUT, OUTPUT);
    
    // Reset
    digitalWrite(TOF1_XSHUT, LOW);
    digitalWrite(TOF2_XSHUT, LOW);
    delay(10);

    // Unreset
    digitalWrite(TOF1_XSHUT, HIGH);
    digitalWrite(TOF2_XSHUT, HIGH);
    delay(10);
    
    // TOF1 ACTIVE
    digitalWrite(TOF1_XSHUT, HIGH);
    digitalWrite(TOF2_XSHUT, LOW);
    
    delay(10);
    
    if (!tof1.begin(0x29, true)) {
        Serial.println(F("Failed to boot TOF1"));
        while (1);
    }
    
    tof1.setAddress(0x30);
    delay(10);
    
    // TOF2 ACTIVE
    digitalWrite(TOF2_XSHUT, HIGH);
    
    delay(10);
    
    if (!tof2.begin(0x29, true)) {
        Serial.println(F("Failed to boot TOF2"));
        while (1);
    }

    tof2.setAddress(0x31);
    Serial.println("Setup TOF OK"); 
}

static void updateTrackerA(uint16_t distanceA_mm = 0) {
    
    // Rack A fully retracted
    if (distanceA_mm >= unlockedThresholdAmm) {
        rackAPosTracker = 1;
    }
    
    // Rack A in middle
    else if (distanceA_mm >= lockedThresholdAmm && distanceA_mm < unlockedThresholdAmm) {
        rackAPosTracker = 2;
    }
    
    // Rack A fully extended
    else if (distanceA_mm < lockedThresholdAmm) {
        rackAPosTracker = 3;
    }
    Serial.print("Rack A: "); Serial.println(rackAPosTracker);
}


static void updateTrackerB(uint16_t distanceB_mm = 0) {

    // Rack B fully retracted
    if (distanceB_mm >= unlockedThresholdBmm) {
        rackBPosTracker = 1;
    }
    
    // Rack B in middle
    else if (distanceB_mm >= lockedThresholdBmm && distanceB_mm < unlockedThresholdBmm) {
        rackBPosTracker = 2;
    }
    
    // Rack B fully extended
    else if (distanceB_mm < lockedThresholdBmm) {
        rackBPosTracker = 3;
    }

    Serial.print("Rack B: "); Serial.println(rackBPosTracker);
}


void tofLoop(void *param) {
    uint16_t distanceA_mm;
    uint16_t distanceB_mm;
    VL53L0X_RangingMeasurementData_t measure1;
    VL53L0X_RangingMeasurementData_t measure2;

    for (;;) {

        tof1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!

        if (measure1.RangeStatus != 4) {  // phase failures have incorrect data
            distanceA_mm = measure1.RangeMilliMeter;
            Serial.print("Distance A (mm): "); Serial.println(distanceA_mm);
            updateTrackerA(distanceA_mm);
        } else {
            Serial.println(" out of range ");
        }

        tof2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!

        if (measure2.RangeStatus != 4) {  // phase failures have incorrect data
            distanceB_mm = measure2.RangeMilliMeter;
            Serial.print("Distance B (mm): "); Serial.println(distanceB_mm);
            updateTrackerB(distanceB_mm);
        } else {
            Serial.println(" out of range ");
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}




