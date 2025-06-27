#include <Arduino.h>
#include "services/motion.h"
#include "core/receiver.h"
#include "sensors/sensors.h"


int currentLeft = 0;
int currentRight = 0;
const unsigned long TIMEOUT_MS = 500;
unsigned long lastCommandTime = 0;


void processCommand(ControlPackage& command) {
    int left = command.left;
    int right = command.left;

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

    Serial.printf("Processed: L=%d, R=%d\n", left, right);
}

void setup() {
    Serial.begin(115200);
    motionSetup();
    radioSetup();
    sensorsSetup();
}

void loop() {
    sensorsUpdate();

    bool radioUpdate = checkForRadio();
    if (radioUpdate) {
        ControlPackage command = checkForCurrentCommand();
        processCommand(command);
    }

    unsigned long now = millis();

    if (now - lastCommandTime > TIMEOUT_MS) {
        arcMove(0, 0);
    } else {
        arcMove(currentLeft, currentRight);
    }

    delay(20);
}