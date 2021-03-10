#include "Arduino.h"
#include "limit_switch.h"

void LimitSwitch::init(unsigned int output_pin, void (*isr)(void)) {
  output_pin_ = output_pin;
  isr_ = isr;
  pinMode(output_pin_, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(output_pin_), isr_, RISING);
}

void LimitSwitch::reenable_interrupt(void) {
    // Clear all pending interrupts on this pin, otherwise they will be triggered
    // immediate after re-attaching the interrupt
    if (output_pin_ == 18U) {
      EIFR |= (1 << INTF3);
    } else if (output_pin_ == 19U) {
      EIFR |= (1 << INTF2);
    }
    attachInterrupt(digitalPinToInterrupt(output_pin_), isr_, RISING);
    debounce_active_ = false;
}

void LimitSwitch::isr(void) {
    stepper_->set_current_pos(stepper_pos_ticks_);

    detachInterrupt(digitalPinToInterrupt(output_pin_));

    debounce_active_ = true;
    reattach_interrupt_time_ = millis() + debounce_duration_ms_;
}

