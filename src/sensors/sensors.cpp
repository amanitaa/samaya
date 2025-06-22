#include "sensors.h"
#include "gyro.h"
// #include "heat_sensor.h" // future

void sensorsSetup() {
    gyroSetup();
    // heatSensorSetup(); // future
}

void sensorsUpdate() {
    gyroUpdate();
    // heatSensorUpdate(); // future
}

bool isUpsideDown() {
    return gyroIsUpsideDown();
}
