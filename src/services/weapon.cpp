#include <Arduino.h>
#include <Servo.h>
#include "weapon.h"

Servo esc;
const int escPin = 10;

void armESC() {
  esc.writeMicroseconds(2000);
  delay(2000);
  esc.writeMicroseconds(1000);
  delay(2000);
}

void weaponSetup() {
  esc.attach(escPin, 1000, 2000);
  armESC();
}

void bldcSpeedControl(int percentage) {
  int pulse;

  switch (percentage) {
    case 0: pulse = 1000; break;
    case 25: pulse = 1250; break;
    case 50: pulse = 1500; break;
    case 75: pulse = 1750; break;
    case 100: pulse = 2000; break;
    default: pulse = 1000; Serial.println("Invalid BLDC percentage, stopping motor"); break;
  }

  esc.writeMicroseconds(pulse);
  Serial.print("ESC set to ");
  Serial.print(percentage);
  Serial.print("% (Pulse: ");
  Serial.print(pulse);
  Serial.println("µs)");
  delay(20);
}

void recoverESC() {
  esc.writeMicroseconds(1000);
  delay(500);
  armESC();
}