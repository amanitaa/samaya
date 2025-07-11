#ifndef RECEIVER_H
#define RECEIVER_H

#include <RF24.h>

#define RF_CHANNEL 0x60

struct StatusPackage {
    bool isUpsideDown;
    uint8_t liionPercent;
    uint8_t lipoPercent;
};

struct ControlPackage {
  int16_t left;
  int16_t right;
};

void setupRadio(RF24& radio);
bool sendMessage(RF24& radio, const void* data, uint8_t size, StatusPackage* response);
bool receiveMessage(RF24& radio, void* data, uint8_t size);

#endif