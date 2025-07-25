#include <Arduino.h>
#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>
#include "receiver.h"

byte controller_node[6] = "0768";
byte samaya_node[6] = "3163";

const uint8_t channel = 88;

#define RF_PA_LEVEL RF24_PA_MAX
#define RF_DATA_RATE RF24_250KBPS
#define RF_AUTO_ACK true
#define RF_RETRIES_DELAY 0
#define RF_RETRIES_COUNT 15
#define RF_PAYLOAD_SIZE 32

void setupRadio(RF24& radio) {
  radio.begin();
  radio.setPayloadSize(32);
  radio.setPALevel(RF_PA_LEVEL);
  radio.setDataRate(RF_DATA_RATE);
  radio.setChannel(channel);
  radio.openWritingPipe(controller_node);
  radio.openReadingPipe(1, samaya_node);
  radio.startListening();
}
