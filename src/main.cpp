#include <Arduino.h>
#include "services/motion.h"
#include "core/receiver.h"
#include "sensors/sensors.h"
#include "sensors/battery.h"

#define CE_PIN A2
#define CSN_PIN A3

RF24 radio(CE_PIN, CSN_PIN);

int currentLeft = 0;
int currentRight = 0;
const unsigned long TIMEOUT_MS = 500;
unsigned long lastCommandTime = 0;


void processCommand(ControlPackage& command) {
    int left = command.left;
    int right = command.right;

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

    ControlPackage receivedData;
    if (radio.available()) {
        while (radio.available()) {                                 
            radio.read(&receivedData, sizeof(ControlPackage) );          
            processCommand(receivedData);
        }
        radio.stopListening();                                        
        StatusPackage ackData = {
            .isUpsideDown = isUpsideDown(),
            .liionPercent = readLiIonPercentage(),
            .lipoPercent = readLiPoPercentage()
        };
        radio.write(&ackData, sizeof(StatusPackage) );              
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