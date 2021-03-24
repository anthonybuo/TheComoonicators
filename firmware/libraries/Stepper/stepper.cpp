#include "Arduino.h"
#include "stepper.h"

void Stepper::init(void) {
  // Set all stepper leads as output
  pinMode(pin1_, OUTPUT);
  pinMode(pin2_, OUTPUT);
  pinMode(pin3_, OUTPUT);
  pinMode(pin4_, OUTPUT);
}

void Stepper::tick(void) {
  // Stepping index
  static int i = 0;

  // Potential set point error
  if (target_position_ > max_target_position_) {
      packet_out_->set_error(PacketOut::AZIMUTH_COMMAND_OOB);
      idle();
      return;
  } else {
      packet_out_->clear_error(PacketOut::AZIMUTH_COMMAND_OOB);
  }

  // Do nothing if set point reached
  if (current_position_ == target_position_) {
    idle();
    return;
  }

  // Update the direction based on target position and current position
  direction = current_position_ < target_position_ ? 1 : -1;

  // Stepper command
  analogWrite(pin1_, full_step_[i][0] * duty_cycle_);
  analogWrite(pin2_, full_step_[i][1] * duty_cycle_);
  analogWrite(pin3_, full_step_[i][2] * duty_cycle_);
  analogWrite(pin4_, full_step_[i][3] * duty_cycle_);

  // Update position
  current_position_ += direction;

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

