#ifndef BATTERY_H
#define BATTERY_H

void batterySetup();
float readLiIonVoltage();
float readLiPoVoltage();
uint8_t readLiIonPercentage();
uint8_t readLiPoPercentage();

#endif
