#include <Arduino.h>
#include "battery.h"

const int LIION_PIN = A0;
const int LIPO_PIN = A1;

const float R1 = 10000.0;
const float R2 = 5600.0;

const float R3 = 33000.0;
const float R4 = 6800.0;

const float ADC_REFERENCE = 5.0;
const int ADC_RESOLUTION = 1023;

void batterySetup() {
    Serial.begin(9600);
    pinMode(LIION_PIN, INPUT);
    pinMode(LIPO_PIN, INPUT);
}

float readLiIonVoltage() {
    int raw = analogRead(LIION_PIN);
    float vOut = (raw / (float)ADC_RESOLUTION) * ADC_REFERENCE;
    return vOut * ((R1 + R2) / R2);
}

float readLiPoVoltage() {
    int raw = analogRead(LIPO_PIN);
    float vOut = (raw / (float)ADC_RESOLUTION) * ADC_REFERENCE;
    return vOut * ((R3 + R4) / R4);
}

uint8_t readLiIonPercentage() {
    float v = readLiIonVoltage();
    const float minVoltage = 6.0;
    const float maxVoltage = 8.4;
    float percentage = (v - minVoltage) / (maxVoltage - minVoltage) * 100.0;
    return (uint8_t)constrain(percentage, 0, 100);
}

uint8_t readLiPoPercentage() {
    float v = readLiPoVoltage();
    const float minVoltage = 13.2;
    const float maxVoltage = 16.8;
    float percentage = (v - minVoltage) / (maxVoltage - minVoltage) * 100.0;
    return (uint8_t)constrain(percentage, 0, 100);
}

void printBatteryStatus() {
    float liIonVoltage = readLiIonVoltage();
    uint8_t liIonPercent = readLiIonPercentage();
    float liPoVoltage = readLiPoVoltage();
    uint8_t liPoPercent = readLiPoPercentage();
    Serial.print("Li-ion Voltage: ");
    Serial.print(liIonVoltage, 2);
    Serial.print(" V | Li-ion Battery: ");
    Serial.print(liIonPercent);
    Serial.println("%");
    Serial.print("LiPo Voltage: ");
    Serial.print(liPoVoltage, 2);
    Serial.print(" V | LiPo Battery: ");
    Serial.print(liPoPercent);
    Serial.println("%");
}