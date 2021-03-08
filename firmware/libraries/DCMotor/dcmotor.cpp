#include "dcmotor.h"
#include "Arduino.h"

void DCMotor::init() {
  pinMode(pin1_, OUTPUT);
  pinMode(pin2_, OUTPUT);
}

