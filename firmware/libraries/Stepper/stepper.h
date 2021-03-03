#ifndef _STEPPER_H_
#define _STEPPER_H_

class Stepper {
    public:
        Stepper(unsigned int pin1, unsigned int pin2, unsigned int pin3, unsigned int pin4)
            : pin1_(pin1), pin2_(pin2), pin3_(pin3), pin4_(pin4) {}

        // Initialize the stepper's control pins
        void init(void);

        // Do a single step of the motor
        void tick(void);

        // Send zero current to the motor
        void idle(void);

        // Control pins
        unsigned int pin1_, pin2_, pin3_, pin4_;

        // Half stepping lookup
        static constexpr unsigned int NUM_STEPPER_INSTR = 8;
        static constexpr unsigned int NUM_STEPPER_LEADS = 4;
        const int half_step[NUM_STEPPER_INSTR][NUM_STEPPER_LEADS] = {
            {1, 0, 1, 0},
            {1, 0, 0, 0},
            {1, 0, 0, 1},
            {0, 0, 0, 1},
            {0, 1, 0, 1},
            {0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 1, 0}
        };

        // Current stepper direction: -1 backwards, 1 forwards
        volatile int stepper_direction = 1;

        // Setpoint position in rotations
        volatile int stepper_target_position_rot = 0;

        // Current position in degrees
        volatile float stepper_position_deg = 0;

        // Step angle from datasheet
        const float step_angle_deg = 0.25;

        // Half stepping reduces angular travel by this factor
        const unsigned int half_step_factor = 2;

        // Azimuth axis gear to ring gear conversion
        const double speed_reduction = 1 / 6.25;
};

#endif  // _STEPPER_H_
