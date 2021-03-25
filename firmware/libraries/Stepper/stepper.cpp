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
  // Direction changed flag
  static unsigned int backlash_compensation_remaining = 0;

  // Homing sequence takes priority
  if (homing_active_) {
    step(false);
    return;
  }

  // Potential set point error
  if (target_position_ > max_target_position_) {
      packet_out_->set_error(PacketOut::AZIMUTH_COMMAND_OOB);
      idle();
      return;
  } else {
      packet_out_->clear_error(PacketOut::AZIMUTH_COMMAND_OOB);
  }

  // Set point reached
  if (current_position_ == target_position_) {
    // Complete steps required to eliminate backlash
    if (backlash_compensation_remaining > 0) {
      step(false);
      backlash_compensation_remaining--;
    } else {
      idle();
    }
    return;
  }

  // Update the direction based on target position and current position
  int prev_direction = direction_;
  direction_ = current_position_ < target_position_ ? 1 : -1;
  // Add additional steps to account for backlash on direction change
  if (prev_direction != direction_) {
    backlash_compensation_remaining = backlash_steps_;
  }

  step(true);
}

void Stepper::step(const bool update_position) {
  // Stepper command
  analogWrite(pin1_, full_step_[i_][0] * duty_cycle_);
  analogWrite(pin2_, full_step_[i_][1] * duty_cycle_);
  analogWrite(pin3_, full_step_[i_][2] * duty_cycle_);
  analogWrite(pin4_, full_step_[i_][3] * duty_cycle_);

  // Update position
  if (update_position) {
    current_position_ += direction_;
  }

  // Update counter
  i_ += direction_;
  if (i_ < 0) {
    i_ = Stepper::NUM_STEPPER_INSTR - 1;
  }
  i_ %= Stepper::NUM_STEPPER_INSTR;
}

void Stepper::idle(void) {
    // Specific to unipolar configuration
    analogWrite(pin1_, 255);
    analogWrite(pin2_, 255);
    analogWrite(pin3_, 255);
    analogWrite(pin4_, 255);
}

