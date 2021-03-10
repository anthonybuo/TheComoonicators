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
      void init();

      // Set speed and direction
      void set_velocity(uint8_t speed, bool dir);

      // Turn off
      void idle();

      // Setters
      void set_target_position(uint8_t hi, uint8_t lo) {
        target_pos_ = ((hi << 8) | lo);
      }
      void update_current_position(double pos) {
        curr_pos_ = pos;
      }

      // Control command
      void tick();

  private:
    // PID parameters
    double Kp_;
    double Ki_;
    double Kd_;
    double bias_;

    // DC Motor computation period
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

