#include "receiver.h"

static RF24 radio(4, 5);
const byte address[6] = "00001";

ControlPackage currentPackage = {0, 0};

static unsigned long lastReceiveTime = 0;
static const unsigned long TIMEOUT_MS = 500;

void radioSetup() {
    if (!radio.begin()) {
        Serial.println("[ERROR] RF24 failed to initialize");
        while (true);
    }

    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_1MBPS);
    radio.setChannel(108);
    radio.openReadingPipe(0, address);
    radio.startListening();

    Serial.println("[INFO] RF24 receiver initialized");
}

bool checkForRadio() {
    if (radio.available()) {
        ControlPackage incoming;
        radio.read(&incoming, sizeof(incoming));

        incoming.left = constrain(incoming.left, -255, 255);
        incoming.right = constrain(incoming.right, -255, 255);

        currentPackage = incoming;
        lastReceiveTime = millis();

        Serial.printf("[RF24] Received: L=%d, R=%d\n", incoming.left, incoming.right);
        return true;
    }
    return false;
}

ControlPackage checkForCurrentCommand() {
    if (millis() - lastReceiveTime > TIMEOUT_MS) {
        return ControlPackage{0, 0};
    }
    return currentPackage;
}