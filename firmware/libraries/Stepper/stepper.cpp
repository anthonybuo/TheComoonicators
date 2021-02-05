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
  digitalWrite(pin1_, half_step[i][0]);
  digitalWrite(pin2_, half_step[i][1]);
  digitalWrite(pin3_, half_step[i][2]);
  digitalWrite(pin4_, half_step[i][3]);

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
    digitalWrite(pin1_, HIGH);
    digitalWrite(pin2_, HIGH);
    digitalWrite(pin3_, HIGH);
    digitalWrite(pin4_, HIGH);
}

