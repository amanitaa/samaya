#include "network.h"

const char* AP_SSID = "Samaya";
const char* AP_PASS = "12345678";
WiFiServer server(8266);

void networkSetup() {
    WiFi.softAP(AP_SSID, AP_PASS);
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
    server.begin();
    Serial.println("WiFi server started");
}

WiFiClient checkForClient() {
    return server.available();
}
