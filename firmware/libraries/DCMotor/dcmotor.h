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

      // Update the set point if command is valid, otherwise idle
      void set_target_position(uint8_t hi, const uint8_t lo) {
        double command;

        // Most significant bit of hi byte is a sign bit
        if ((hi & (1 << 7)) != 0) {
          hi &= ~(1 << 7);
          command = -1 * ((hi << 8) | lo);
        } else {
          command = ((hi << 8) | lo);
        }

        if (in_desired_rom(command)) {
          packet_out_->clear_error(PacketOut::ELEVATION_COMMAND_OOB);
          target_pos_ = command;
        } else {
          packet_out_->set_error(PacketOut::ELEVATION_COMMAND_OOB);
          idle();
        }
      }

      // Update current position
      void update_current_position(const double pos) {
        curr_pos_ = pos;
      }

      // Check if command is within bounds
      bool in_desired_rom(const double command) {
        if ((command > max_rom_mrad_) || (command < min_rom_mrad_)) {
          return false;
        }
        return true;
      }

      // Control command
      void tick();

      // Allowable range of motion just beyond 0deg to 90deg
      const double max_rom_mrad_ = (HALF_PI * 1000) + (3 * DEG_TO_RAD * 1000);
      const double min_rom_mrad_ = -5 * DEG_TO_RAD * 1000;

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

