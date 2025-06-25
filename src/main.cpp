#include <Arduino.h>
#include "core/network.h"
#include "services/motion.h"
#include "sensors/sensors.h"


#include <Arduino.h>
#include "core/network.h"
#include "services/motion.h"
#include "sensors/sensors.h"

unsigned long lastCommandTime = 0;
int prevLeft = 999;
int prevRight = 999;

void processClientCommand(WiFiClient& client) {
    while (client.available()) {
        String line = client.readStringUntil('\n');
        line.trim();

        int sp = line.indexOf(' ');
        if (sp <= 0) {
            Serial.println("Invalid command. Skipped.");
            continue;
        }

        String leftStr = line.substring(0, sp);
        String rightStr = line.substring(sp + 1);
        leftStr.trim(); rightStr.trim();

        int left = leftStr.toInt();
        int right = rightStr.toInt();

        if (left < -255 || left > 255 || right < -255 || right > 255) {
            Serial.println("Out-of-range speeds. Skipped.");
            continue;
        }

        if (left == prevLeft && right == prevRight) continue;
        prevLeft = left;
        prevRight = right;

        arcMove(left, right);
        lastCommandTime = millis();
        Serial.printf("Got: L=%d, R=%d\n", left, right);
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

    if (millis() - lastCommandTime > 500) {
        arcMove(0, 0);
    }

    delay(20);
}
