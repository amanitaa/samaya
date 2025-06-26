#include <Arduino.h>
#include "core/network.h"
#include "services/motion.h"
#include "sensors/sensors.h"


int currentLeft = 0;
int currentRight = 0;
const unsigned long TIMEOUT_MS = 500;
unsigned long lastCommandTime = 0;


void processClientCommand(WiFiClient& client) {
    while (client.available()) {
        String line = client.readStringUntil('\n');
        line.trim();

        int sp = line.indexOf(' ');
        if (sp <= 0) return;

        int left = line.substring(0, sp).toInt();
        int right = line.substring(sp + 1).toInt();

        if (left < -255 || left > 255 || right < -255 || right > 255) return;

        if (isUpsideDown()) {
            Serial.println("INFO: Robot is upside down — inverting motion.");
            left = -left;
            right = -right;
        }

        currentLeft = left;
        currentRight = right;
        lastCommandTime = millis();

        Serial.printf("Processed: L=%d, R=%d\n", left, right);
    }
}



void setup() {
    Serial.begin(115200);
    motionSetup();
    networkSetup();
    sensorsSetup();
}

void loop() {
    sensorsUpdate();

    static WiFiClient activeClient;
    WiFiClient newClient = checkForClient();

    if (newClient) {
        if (!activeClient || !activeClient.connected()) {
            activeClient = newClient;
            Serial.println("Client connected");
        }
    }

    if (activeClient && activeClient.connected()) {
        processClientCommand(activeClient);
    } else {
        activeClient.stop();
    }

    unsigned long now = millis();

    if (now - lastCommandTime > TIMEOUT_MS) {
        arcMove(0, 0);
    } else {
        arcMove(currentLeft, currentRight);
    }

    delay(20);
}