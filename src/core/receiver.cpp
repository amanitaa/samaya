#include <Arduino.h>
#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>
#include "receiver.h"

byte controller_node[6] = "NodeA";
byte samaya_node[6] = "NodeB";

#define RF_PA_LEVEL RF24_PA_MAX
#define RF_DATA_RATE RF24_250KBPS
#define RF_AUTO_ACK true
#define RF_RETRIES_DELAY 0
#define RF_RETRIES_COUNT 15
#define RF_PAYLOAD_SIZE 32

void setupRadio(RF24& radio) {
  radio.begin();
  size_t maxPayloadSize = max(sizeof(ControlPackage), sizeof(StatusPackage));
  radio.setPayloadSize(maxPayloadSize);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(controller_node);
  radio.openReadingPipe(1, samaya_node);
  radio.startListening();
}
