#ifndef RECEIVER_H
#define RECEIVER_H

#include <RF24.h>

void radioSetup(RF24& radio);
bool sendMessage(RF24& radio, const void* data, uint8_t size, byte* response);
bool receiveMessage(RF24& radio, void* data, uint8_t size);

#endif