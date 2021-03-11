#ifndef _STEPPER_H_
#define _STEPPER_H_

#include "Arduino.h"
#include "packet.h"

class Stepper {
    public:
        Stepper(PacketOut* packet_out, unsigned int pin1, unsigned int pin2,
                unsigned int pin3, unsigned int pin4)
            : packet_out_(packet_out), pin1_(pin1), pin2_(pin2), pin3_(pin3),
              pin4_(pin4) {}

        // Initialize the stepper's control pins
        void init(void);

        // Do a single step of the motor
        void tick(void);

        // Send zero current to the motor
        void idle(void);

        // Getters
        volatile unsigned int get_current_position() {
            return current_position_;
        }
        void get_current_position(uint8_t* hi, uint8_t* lo) {
            *hi = (current_position_ & 0xFF00) >> 8;
            *lo = (current_position_ & 0x00FF);
        }

        // Set the stepper's target position
        void set_target_position(uint8_t hi, uint8_t lo) {
            target_position_ = ((hi << 8) | lo);
        }
        void set_target_position(uint16_t pos) {
            target_position_ = pos;
        }

        // Set the stepper's current position. Should only be called by the
        // limit switches for open loop control.
        void set_current_pos(uint16_t pos_ticks) {
            current_position_ = pos_ticks;
        }

    private:
        // Current stepper direction: -1 backwards, 1 forwards
        volatile int direction = 1;

        // Current position
        volatile unsigned int current_position_ = 0;

        // Set point position
        volatile unsigned int target_position_ = 0;

        // Control pins
        unsigned int pin1_, pin2_, pin3_, pin4_;

        // Output duty cycle per phase
        uint8_t duty_cycle_ = 255;

        // Stepper sets error flags in outgoing packet
        PacketOut* packet_out_ = nullptr;

        // Half stepping lookup
        static constexpr unsigned int NUM_STEPPER_INSTR = 4;
        static constexpr unsigned int NUM_STEPPER_LEADS = 4;
        const int half_step_[NUM_STEPPER_INSTR][NUM_STEPPER_LEADS] = {
            {1, 0, 0, 0},
            {0, 0, 0, 1},
            {0, 1, 0, 0},
            {0, 0, 1, 0}
        };

        // Azimuth ROM
        const unsigned int range_of_motion_deg_ = 380;

        // Azimuth rotation per bit of command byte
        const double resolution_deg_per_bit_ = 0.04;

        // Max valid target position
        const unsigned int max_target_position_ = range_of_motion_deg_ / resolution_deg_per_bit_;
};

#endif  // _STEPPER_H_
