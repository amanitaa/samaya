#include "gyro.h"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>

Adafruit_MPU6050 mpu;
static bool upsideDown = false;

void gyroSetup() {
    Serial.println("Initializing MPU6050...");

    Wire.begin();

    if (!mpu.begin()) {
        Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
        while (1) delay(10);
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial.println("MPU6050 initialized.\n");
}

void gyroUpdate() {
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    float az = accel.acceleration.z / 9.80665;
    upsideDown = (az < 0.0f);

    Serial.print("Z-accel: ");
    Serial.print(az, 2);
    Serial.print(" g   → ");
    if (upsideDown) {
        Serial.println("Upside down");
    } else {
        Serial.println("Normal");
    }
}

bool gyroIsUpsideDown() {
    return upsideDown;
}
