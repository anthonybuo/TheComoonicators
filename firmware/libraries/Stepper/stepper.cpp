#include "Arduino.h"
#include "stepper.h"

void Stepper::init(void) {
    pinMode(pin1_, OUTPUT);
    pinMode(pin2_, OUTPUT);
    pinMode(pin3_, OUTPUT);
    pinMode(pin4_, OUTPUT);
}

void Stepper::tick(void) {
  static int i = 0;

  // Quit if at setpoint
  if (stepper_position_deg == (stepper_target_position_rot * 360)) {
    idle();
    return;
  }

  // Stepper command
  analogWrite(pin1_, half_step[i][0] * 255);
  analogWrite(pin2_, half_step[i][1] * 255);
  analogWrite(pin3_, half_step[i][2] * 255);
  analogWrite(pin4_, half_step[i][3] * 255);

  // Update position
  stepper_position_deg += (stepper_direction * step_angle_deg / 2);

  // Update counter
  i += stepper_direction;
  if (i < 0) {
    i = Stepper::NUM_STEPPER_INSTR - 1;
  }
  i %= Stepper::NUM_STEPPER_INSTR;
}

void Stepper::idle(void) {
    // Specific to unipolar configuration
    analogWrite(pin1_, 255);
    analogWrite(pin2_, 255);
    analogWrite(pin3_, 255);
    analogWrite(pin4_, 255);
}

