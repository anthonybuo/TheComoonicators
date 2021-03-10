#ifndef _DCMOTOR_H_
#define _DCMOTOR_H_

#include "Arduino.h"
#include "packet.h"

class DCMotor {
  public:
      DCMotor(PacketOut* packet_out, unsigned int pin1, unsigned int pin2,
              double Kp, double Ki, double Kd, double period, double bias)
          : packet_out_(packet_out), pin1_{pin1}, pin2_{pin2}, Kp_{Kp}, Ki_{Ki},
          Kd_{Kd}, period_{period}, bias_{bias} {}

      // Initialize the DC motor's control pins
      void init() {
        pinMode(pin1_, OUTPUT);
        pinMode(pin2_, OUTPUT);
      }

      // Test set velocity
      void set_vel(unsigned char speed, bool dir) {
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

      // Turn off
      void idle() {
        analogWrite(pin1_, 0);
        analogWrite(pin2_, 0);
      }

      void set_target_position(uint8_t hi, uint8_t lo) {
        target_pos_ = ((hi << 8) | lo);
      }

      void update_current_position(double pos) {
        curr_pos_ = pos;
      }

      // Control command
      void tick() {
        // Compute error, integral, derivative terms
        static unsigned int error_prev = 0;
        static unsigned int integral_prev = 0;
        double error = target_pos_ - curr_pos_;
        double derivative = (error - error_prev) / period_;
        double integral = integral_prev + error * period_;

        // Compute control action and update previous values
        double speed = Kp_*error + Ki_*integral + Kd_*derivative + bias_;
        error_prev = error;
        integral_prev = integral;

        // Commit the command
        set_vel(abs(speed), target_pos_ > curr_pos_);
      }

  private:
    // PID parameters
    double Kp_;
    double Ki_;
    double Kd_;
    double bias_;
    const double period_;

    // Set point
    double target_pos_;

    // Current position
    double curr_pos_;

    // Control pins
    unsigned int pin1_, pin2_;

    // DC Motor sets error flags in outgoing packet
    PacketOut* packet_out_ = nullptr;
};

#endif  // _DCMOTOR_H_

