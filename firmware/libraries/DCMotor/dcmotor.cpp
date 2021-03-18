#include "dcmotor.h"
#include "Arduino.h"

void DCMotor::init() {
  pinMode(pin1_, OUTPUT);
  pinMode(pin2_, OUTPUT);
}

void DCMotor::tick() {
  // Compute error, integral, derivative terms
  static double error_prev = 0;
  static double integral_prev = 0;
  double error = target_pos_ - curr_pos_;
  double derivative = (error - error_prev) / period_;
  double integral = integral_prev + error * period_;

  // decay integral term if near setpoint
  if (abs(error) < 60) {  // ~3 deg
    integral /= 1.03;
  }

  // Compute control action and update previous values
  double speed = Kp_*error + Ki_*integral + Kd_*derivative + bias_;
  error_prev = error;
  integral_prev = integral;
  
  // Commit the command
  set_velocity(abs(speed), target_pos_ > curr_pos_);
}

void DCMotor::set_velocity(uint8_t speed, bool dir) {
  // Saturate for testing
  if (speed > 200) {
    speed = 200;
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

