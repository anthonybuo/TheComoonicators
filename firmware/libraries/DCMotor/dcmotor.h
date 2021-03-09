#ifndef _DCMOTOR_H_
#define _DCMOTOR_H_

#include "Arduino.h"

class DCMotor {
  public:
      DCMotor(unsigned int pin1, unsigned int pin2)
          : pin1_{pin1}, pin2_{pin2} {}

      // Initialize the DC motor's control pins
      void init();

      // Test
      void set_vel(unsigned char speed, bool dir);

      // Turn off
      void idle();

  private:
    // Control pins
    unsigned int pin1_, pin2_;
};

#endif  // _DCMOTOR_H_

