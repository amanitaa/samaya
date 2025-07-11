#include <Arduino.h>
#include "battery.h"

const int LIION_PIN = A0;
const int LIPO_PIN = A1;

const float VREF = 5.0;

void batterySetup() {
    pinMode(LIION_PIN, INPUT);
    pinMode(LIPO_PIN, INPUT);
}

float readLiIonVoltage() {
    int raw = analogRead(LIION_PIN);
    return raw * (VREF / 1023.0);
}

float readLiPoVoltage() {
    int raw = analogRead(LIPO_PIN);
    return raw * (VREF / 1023.0);
}

uint8_t readLiIonPercentage() {
    float v = readLiIonVoltage();
    if (v >= 4.20) return 100;
    else if (v >= 4.10) return 95;
    else if (v >= 4.00) return 85;
    else if (v >= 3.90) return 75;
    else if (v >= 3.80) return 60;
    else if (v >= 3.70) return 45;
    else if (v >= 3.60) return 30;
    else if (v >= 3.50) return 20;
    else if (v >= 3.40) return 10;
    else if (v >= 3.30) return 5;
    else return 0;
}

uint8_t readLiPoPercentage() {
    float v = readLiPoVoltage();
    if (v >= 4.20) return 100;
    else if (v >= 4.10) return 90;
    else if (v >= 4.00) return 80;
    else if (v >= 3.90) return 65;
    else if (v >= 3.80) return 50;
    else if (v >= 3.70) return 35;
    else if (v >= 3.60) return 25;
    else if (v >= 3.50) return 15;
    else if (v >= 3.40) return 5;
    else return 0;
}
