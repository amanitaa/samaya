#include <Arduino.h>
#include "services/motion.h"
#include "services/weapon.h"
#include "sensors/sensors.h"
#include "sensors/battery.h"
#include "core/receiver.h"

#define CE_PIN A2
#define CSN_PIN A3
#define TIMEOUT_MS 500

RF24 radio(CE_PIN, CSN_PIN);

int16_t currentLeft = 0;
int16_t currentRight = 0;
int currentBLDCPercentage = 0;
unsigned long lastCommandTime = 0;

void processCommand(int16_t left, int16_t right, int bldcPercentage) {
  if (left < -255 || left > 255 || right < -255 || right > 255) {
    Serial.println("Invalid motor speeds, ignoring command");
    return;
  }
  if (bldcPercentage != 0 && bldcPercentage != 25 && bldcPercentage != 50 &&
      bldcPercentage != 75 && bldcPercentage != 100) {
    Serial.println("Invalid BLDC percentage, ignoring");
    return;
  }
  if (isUpsideDown()) {
    Serial.println("INFO: Robot is upside down — inverting motion");
    arcMove(0, 0);
    delay(100);
    left = -left;
    right = -right;
  }
  currentLeft = left;
  currentRight = right;
  currentBLDCPercentage = bldcPercentage;
  lastCommandTime = millis();
  arcMove(currentLeft, currentRight);
  bldcSpeedControl(currentBLDCPercentage);
  Serial.print("Processed: L=");
  Serial.print(left);
  Serial.print(", R=");
  Serial.print(right);
  Serial.print(", BLDC=");
  Serial.print(bldcPercentage);
  Serial.println("%");
}

void setup() {
  Serial.begin(9600);
  weaponSetup();
  motionSetup();
  batterySetup();
  sensorsSetup();
  if (!radio.begin()) {
    Serial.println("Radio initialization failed!");
    while (1);
  }
  setupRadio(radio);
  radio.startListening();
  Serial.println("Receiver initialized");
}

void loop() {
  sensorsUpdate();
  char receivedDataString[20];
  if (radio.available()) {
    while (radio.available()) {
      radio.read(&receivedDataString, sizeof(receivedDataString));
      Serial.print("Received: ");
      Serial.println(receivedDataString);
      int16_t receivedLeft, receivedRight, bldcPercentage;
      if (sscanf(receivedDataString, "L%dR%dB%d", &receivedLeft, &receivedRight, &bldcPercentage) == 3) {
        processCommand(receivedLeft, receivedRight, bldcPercentage);
      } else if (sscanf(receivedDataString, "L%dR%d", &receivedLeft, &receivedRight) == 2) {
        processCommand(receivedLeft, receivedRight, currentBLDCPercentage); // Keep last BLDC speed
        Serial.println("No BLDC percentage in message, using last value");
      } else {
        Serial.print("Error parsing: ");
        Serial.println(receivedDataString);
      }
    }
    radio.stopListening();
    StatusPackage responseData = {
      .isUpsideDown = isUpsideDown(),
      .liionPercent = readLiIonPercentage(),
      .lipoPercent = readLiPoPercentage()
    };
    radio.write(&responseData, sizeof(StatusPackage));
    Serial.println("Status sent back");
    radio.startListening();
  }
  unsigned long now = millis();
  if (now - lastCommandTime > TIMEOUT_MS) {
    arcMove(0, 0);
    bldcSpeedControl(0); // Stop BLDC motor on timeout
    currentLeft = 0;
    currentRight = 0;
    currentBLDCPercentage = 0;
    Serial.println("Timeout: Stopped motors and BLDC");
  }
  delay(20);
}