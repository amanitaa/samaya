#include "motion.h"
#include <Arduino.h>

// Motor pins
const int IN1 = 16;
const int IN2 = 17;
const int IN3 = 18;
const int IN4 = 19;

void motionSetup() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

void drive(int leftSpeed, int rightSpeed) {
    if (leftSpeed >= 0) {
        analogWrite(IN1, leftSpeed);
        analogWrite(IN2, 0);
    } else {
        analogWrite(IN1, 0);
        analogWrite(IN2, -leftSpeed);
    }

    if (rightSpeed >= 0) {
        analogWrite(IN3, rightSpeed);
        analogWrite(IN4, 0);
    } else {
        analogWrite(IN3, 0);
        analogWrite(IN4, -rightSpeed);
    }
}
