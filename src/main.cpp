#include <Arduino.h>
#include "core/network.h"
#include "services/motion.h"
#include "sensors/sensors.h"


void processClientCommand(WiFiClient& client) {
    while (!client.available()) delay(1);

    String line = client.readStringUntil('\n');
    line.trim();
    client.stop();

    int sp = line.indexOf(' ');
    String cmd = (sp > 0 ? line.substring(0, sp) : line);
    int speed = (sp > 0 ? line.substring(sp + 1).toInt() : 255);

    bool upsideDown = isUpsideDown();

    if (upsideDown) {
        Serial.println("INFO: Robot is upside down — applying inverted control mapping.");
    }

    if (cmd == "FORWARD") {
        if (upsideDown) drive(-speed, -speed);  
        else            drive(+speed, +speed);
    }
    else if (cmd == "BACK") {
        if (upsideDown) drive(+speed, +speed);
        else            drive(-speed, -speed);
    }
    else if (cmd == "LEFT") {
        if (upsideDown) drive(+speed, -speed);
        else            drive(-speed, +speed);
    }
    else if (cmd == "RIGHT") {
        if (upsideDown) drive(-speed, +speed);  
        else            drive(+speed, -speed);
    }
    else {
        drive(0, 0);
    }

    Serial.printf("Got: %s %d\n", cmd.c_str(), speed);
}


void setup() {
    Serial.begin(115200);
    motionSetup();
    networkSetup();
    sensorsSetup();
}

void loop() {
    sensorsUpdate();

    WiFiClient client = checkForClient();
    if (client) {
        processClientCommand(client);
    }

    delay(100);
}
