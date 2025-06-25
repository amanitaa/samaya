#include "network.h"

const char* WIFI_SSID = "HUAWEI-Dbu3";
const char* WIFI_PASS = "2cKtC998";
WiFiServer server(8266, 4);

void networkSetup() {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");
    Serial.print("ESP32 IP address: ");
    Serial.println(WiFi.localIP());

    server.begin();
    Serial.println("Server started");
}

WiFiClient checkForClient() {
    return server.available();
}
