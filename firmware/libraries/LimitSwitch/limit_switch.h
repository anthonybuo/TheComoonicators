#ifndef _LIMIT_SWITCH_H_
#define _LIMIT_SWITCH_H_

#include <Arduino.h>

class LimitSwitch {
  public:
    // Associate a limit switch with an interrupt pin and enable its ISR
    void init(unsigned int output_pin, void (*isr)(void));

    // Re-enable the interrupt, typically called after debounce period
    // has ended which is checked outside of this class
    void reenable_interrupt(void);

    // Interrupt service routine. Meaningful work goes here
    void isr(void);

    // MCU pin to which the limit switch triggers an interrupt
    unsigned int output_pin_;

    // True if the switch is being debounced, false otherwise
    bool debounce_active_ = false;

    // Interrupt callback
    void (*isr_)(void);

    // Time at which the debounce period will end
    unsigned int reattach_interrupt_time_;

  private:
    // Debounce duration
    const unsigned int debounce_duration_ms_ = 1000;
};

#endif  // _LIMIT_SWITCH_H_

