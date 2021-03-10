#ifndef _DCMOTOR_H_
#define _DCMOTOR_H_

#include "Arduino.h"
#include "packet.h"

class DCMotor {
  public:
      DCMotor(PacketOut* packet_out, unsigned int pin1, unsigned int pin2)
          : packet_out_(packet_out), pin1_{pin1}, pin2_{pin2} {}

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
        unsigned int speed = abs(target_pos_ - curr_pos_) / 10;
        set_vel(speed, target_pos_ > curr_pos_);
      }

  private:
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

