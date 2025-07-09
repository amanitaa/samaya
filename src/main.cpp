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

struct ControlPackage {
  int16_t left;
  int16_t right;
};


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

    // Serial.printf("Processed: L=%d, R=%d\n", left, right);
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

    ControlPackage receivedData;
    if (receiveMessage(radio, &receivedData, sizeof(ControlPackage))) {
        processCommand(receivedData);

        byte ackData = (byte)(receivedData.left + 1);
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