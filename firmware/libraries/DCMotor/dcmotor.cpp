#include "dcmotor.h"
#include "Arduino.h"

void DCMotor::init() {
  pinMode(pin1_, OUTPUT);
  pinMode(pin2_, OUTPUT);
}

void DCMotor::set_vel(unsigned char speed, bool dir) {
  // Saturate
  if (speed > 100) {
      speed = 100;
  }

  // Set command
  if (dir) {
    analogWrite(pin1_, speed);
    analogWrite(pin2_, 0);
  } else {
    analogWrite(pin1_, 0);
    analogWrite(pin2_, speed);
  }
}

void DCMotor::idle() {
  analogWrite(pin1_, 0);
  analogWrite(pin2_, 0);
}

