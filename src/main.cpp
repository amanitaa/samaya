#include <Arduino.h>
#include "services/motion.h"
#include "core/receiver.h"
#include "sensors/sensors.h"
#include "sensors/battery.h"

#define CE_PIN A2
#define CSN_PIN A3

RF24 radio(CE_PIN, CSN_PIN);

int16_t currentLeft = 0;
int16_t currentRight = 0;
const unsigned long TIMEOUT_MS = 500;
unsigned long lastCommandTime = 0;


void processCommand(int16_t left, int16_t right) {
    if (left < -255 || left > 255 || right < -255 || right > 255) return;

    if (isUpsideDown()) {
        Serial.println("INFO: Robot is upside down — inverting motion.");
        arcMove(0, 0);
        delay(100);
        left = -left;
        right = -right;
    }

    currentLeft = left;
    currentRight = right;
    lastCommandTime = millis();

    Serial.print("Processed: L=");
    Serial.print(left);
    Serial.print(", R=");
    Serial.println(right);
}

void setup() {
    Serial.begin(9600);
    motionSetup();
    batterySetup();
    // sensorsSetup();
    if (!radio.begin()) {
        Serial.println("Radio initialization failed!");
        while (1);
    }
    setupRadio(radio);
    Serial.println("Receiver initialized");
}

void loop() {
    // sensorsUpdate();

    char receivedDataString[16];

    if (radio.available()) {
        while (radio.available()) {                                   
            radio.read(&receivedDataString, sizeof(receivedDataString) );            
            int16_t receivedLeft, receivedRight;
            Serial.print(F("Got message '"));
            Serial.print(receivedDataString);
            if (sscanf(receivedDataString, "L%dR%d", &receivedLeft, &receivedRight) == 2) {
                processCommand(receivedLeft, receivedRight);
            } else {
                Serial.print("Error parsing: ");
                Serial.println(receivedDataString);
            }
        }
        radio.stopListening();                                        
        StatusPackage responseData = {
            .isUpsideDown = isUpsideDown(),
            .liionPercent = readLiIonPercentage(),
            .lipoPercent = readLiPoPercentage()
        };
        radio.write(&responseData, sizeof(StatusPackage));
        radio.startListening();

    }

    unsigned long now = millis();

    if (now - lastCommandTime > TIMEOUT_MS) {
        arcMove(0, 0);
    } else {
        arcMove(currentLeft, currentRight);
    }

    delay(20);
}