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

    Serial.print("Processed: L=");
    Serial.print(left);
    Serial.print(", R=");
    Serial.println(right);
}

void setup() {
    Serial.begin(9600);
    motionSetup();
    // sensorsSetup();
    if (!radio.begin()) {
        Serial.println("Radio initialization failed!");
        while (1);
    }
    setupRadio(radio);
    radio.startListening();
    Serial.println("Receiver initialized");

    StatusPackage ackData = {isUpsideDown()};
    radio.writeAckPayload(1, &ackData, sizeof(StatusPackage));
}

void loop() {
    // sensorsUpdate();

    ControlPackage receivedData;
    if (receiveMessage(radio, &receivedData, sizeof(ControlPackage))) {
        processCommand(receivedData);

        StatusPackage ackData = {isUpsideDown()};
        radio.writeAckPayload(1, &ackData, sizeof(StatusPackage));
    }

    unsigned long now = millis();

    if (now - lastCommandTime > TIMEOUT_MS) {
        arcMove(0, 0);
    } else {
        arcMove(currentLeft, currentRight);
    }

    delay(20);
}