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

  // Potential set point error
  if (target_position > max_target_position) {
      packet_out_->set_error(PacketOut::AZIMUTH_COMMAND_OOB);
      idle();
      return;
  } else {
      packet_out_->clear_error(PacketOut::AZIMUTH_COMMAND_OOB);
  }

  // Do nothing if set point reached
  if (current_position == target_position) {
    idle();
    return;
  }

  // Stepper command
  analogWrite(pin1_, half_step[i][0] * 255);
  analogWrite(pin2_, half_step[i][1] * 255);
  analogWrite(pin3_, half_step[i][2] * 255);
  analogWrite(pin4_, half_step[i][3] * 255);

  // Update position
  current_position += direction;

  // Update counter
  i += direction;
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

