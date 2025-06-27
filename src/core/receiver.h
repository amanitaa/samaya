#pragma once
#include <RF24.h>
#include <SPI.h>

struct ControlPackage {
    byte left;
    byte right;
};

void radioSetup();
bool checkForRadio();
ControlPackage checkForCurrentCommand();
