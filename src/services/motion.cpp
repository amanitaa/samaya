#include "motion.h"
#include <Arduino.h>

const int LEFT_MOTOR_FWD  = 16;
const int LEFT_MOTOR_BWD  = 17;
const int RIGHT_MOTOR_FWD = 18;
const int RIGHT_MOTOR_BWD = 19;

const int PWM_FREQ          = 5000;
const int PWM_RESOLUTION    = 8;
const int LEFT_PWM_CHANNEL  = 0;
const int RIGHT_PWM_CHANNEL = 1;

static int lastLeftSpeed = 0;
static int lastRightSpeed = 0;

void motionSetup() {
  ledcSetup(LEFT_PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(RIGHT_PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);

  ledcAttachPin(LEFT_MOTOR_FWD, LEFT_PWM_CHANNEL);
  ledcAttachPin(RIGHT_MOTOR_FWD, RIGHT_PWM_CHANNEL);

  pinMode(LEFT_MOTOR_BWD, OUTPUT);
  pinMode(RIGHT_MOTOR_BWD, OUTPUT);

  stopMotors();
}

void setMotor(int pwmChannel, int dirPin, int speed) {
  bool reverse = (speed < 0);
  digitalWrite(dirPin, reverse ? HIGH : LOW);
  ledcWrite(pwmChannel, abs(speed));
}

void arcMove(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  if ((leftSpeed < 0) != (lastLeftSpeed < 0)) {
    ledcWrite(LEFT_PWM_CHANNEL, 0);
    delay(5);
  }
  if ((rightSpeed < 0) != (lastRightSpeed < 0)) {
    ledcWrite(RIGHT_PWM_CHANNEL, 0);
    delay(5);
  }

  setMotor(LEFT_PWM_CHANNEL, LEFT_MOTOR_BWD, leftSpeed);
  setMotor(RIGHT_PWM_CHANNEL, RIGHT_MOTOR_BWD, rightSpeed);

  lastLeftSpeed = leftSpeed;
  lastRightSpeed = rightSpeed;

  Serial.printf("Left=%d, Right=%d\n", leftSpeed, rightSpeed);
}

void stopMotors() {
  ledcWrite(LEFT_PWM_CHANNEL, 0);
  digitalWrite(LEFT_MOTOR_BWD, LOW);
  ledcWrite(RIGHT_PWM_CHANNEL, 0);
  digitalWrite(RIGHT_MOTOR_BWD, LOW);
}
