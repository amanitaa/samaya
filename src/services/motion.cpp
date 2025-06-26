#include "motion.h"
#include <Arduino.h>

const int LEFT_IN1  = 16;
const int LEFT_IN2  = 17;
const int RIGHT_IN1 = 18;
const int RIGHT_IN2 = 19;

const int PWM_FREQ          = 5000;
const int PWM_RESOLUTION    = 8;
const int LEFT_PWM_CHANNEL  = 0;
const int RIGHT_PWM_CHANNEL = 1;


void motionSetup() {
    ledcSetup(LEFT_PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcSetup(RIGHT_PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);

    ledcAttachPin(LEFT_IN1, LEFT_PWM_CHANNEL);
    ledcAttachPin(RIGHT_IN1, RIGHT_PWM_CHANNEL);

    pinMode(LEFT_IN2, OUTPUT);
    pinMode(RIGHT_IN2, OUTPUT);

    stopMotors();
}

void arcMove(int leftSpeed, int rightSpeed) {
    leftSpeed  = constrain(leftSpeed, -255, 255);
    rightSpeed = constrain(rightSpeed, -255, 255);

    if (leftSpeed >= 0) {
        digitalWrite(LEFT_IN2, LOW);                            
        ledcWrite(LEFT_PWM_CHANNEL, leftSpeed);
    } else {
        digitalWrite(LEFT_IN2, HIGH);                           
        ledcWrite(LEFT_PWM_CHANNEL, 255 - abs(leftSpeed));  
    }

    if (rightSpeed >= 0) {
        digitalWrite(RIGHT_IN2, LOW);
        ledcWrite(RIGHT_PWM_CHANNEL, rightSpeed);
    } else {
        digitalWrite(RIGHT_IN2, HIGH);
        ledcWrite(RIGHT_PWM_CHANNEL, 255 - abs(rightSpeed));
    }

    Serial.printf("Left=%d, Right=%d\n", leftSpeed, rightSpeed);
}

void stopMotors() {
  ledcWrite(LEFT_PWM_CHANNEL, 0);
  digitalWrite(LEFT_IN2, LOW);
  ledcWrite(RIGHT_PWM_CHANNEL, 0);
  digitalWrite(RIGHT_IN2, LOW);
}
