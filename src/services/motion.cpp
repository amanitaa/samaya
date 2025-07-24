#include "motion.h"
#include <Arduino.h>

const int LEFT_IN1  = 5;
const int LEFT_IN2  = 3; 
const int RIGHT_IN1 = 9;
const int RIGHT_IN2 = 6;

void motionSetup() {
    pinMode(LEFT_IN1, OUTPUT);
    pinMode(LEFT_IN2, OUTPUT);
    pinMode(RIGHT_IN1, OUTPUT);
    pinMode(RIGHT_IN2, OUTPUT);

    stopMotors();
}

void arcMove(int leftSpeed, int rightSpeed) {
    int tmp = leftSpeed; leftSpeed = rightSpeed; rightSpeed = tmp;

    leftSpeed  = constrain(leftSpeed,  -255, 255);
    rightSpeed = constrain(rightSpeed, -255, 255);

    if (leftSpeed >= 0) {
        digitalWrite(LEFT_IN2, LOW);
        analogWrite(LEFT_IN1,  leftSpeed);
    } else {
        digitalWrite(LEFT_IN1, LOW);
        analogWrite(LEFT_IN2, -leftSpeed);
    }

    if (rightSpeed >= 0) {
        digitalWrite(RIGHT_IN2, LOW);
        analogWrite(RIGHT_IN1,  rightSpeed);
    } else {
        digitalWrite(RIGHT_IN1, LOW);
        analogWrite(RIGHT_IN2, -rightSpeed);
    }
}

void stopMotors() {
    analogWrite(LEFT_IN1, 0);
    digitalWrite(LEFT_IN2, LOW);
    analogWrite(RIGHT_IN1, 0);
    digitalWrite(RIGHT_IN2, LOW);
}