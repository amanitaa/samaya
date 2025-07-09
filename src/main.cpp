#include <Arduino.h>
#include "services/motion.h"
#include "core/receiver.h"
#include "sensors/sensors.h"

#define CE_PIN A2
#define CSN_PIN A3

RF24 radio(CE_PIN, CSN_PIN);

int currentLeft = 0;
int currentRight = 0;
const unsigned long TIMEOUT_MS = 500;
unsigned long lastCommandTime = 0;


void processCommand(byte receivedData) {
    // TODO REFACTOR TO STRUCT MAYBE?
    int left = map(receivedData, 0, 255, -255, 255);
    int right = left;

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

    // Serial.println("Processed: L=%d, R=%d\n", left, right);
}

void setup() {
    Serial.begin(115200);
    motionSetup();
    sensorsSetup();
    if (!radio.begin()) {
        Serial.println("Radio initialization failed!");
        while (1);
    }
    radioSetup(radio);
    radio.startListening();
    Serial.println("Receiver initialized");

    byte ackData = 0xAA;
    radio.writeAckPayload(1, &ackData, 1);
}


void loop() {
    sensorsUpdate();

    byte receivedData;
    if (receiveMessage(radio, &receivedData, 1)) {
        processCommand(receivedData);

        byte ackData = receivedData + 1;
        radio.writeAckPayload(1, &ackData, 1);
    }

    unsigned long now = millis();

    if (now - lastCommandTime > TIMEOUT_MS) {
        arcMove(0, 0);
    } else {
        arcMove(currentLeft, currentRight);
    }

    delay(20);
}