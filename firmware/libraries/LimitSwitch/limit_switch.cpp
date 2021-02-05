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
    if (output_pin_ == 2U) {
      EIFR |= (1 << INTF4);
    } else if (output_pin_ == 3U) {
      EIFR |= (1 << INTF5);
    }
    attachInterrupt(digitalPinToInterrupt(output_pin_), isr_, RISING);
    debounce_active_ = false;
}

void LimitSwitch::isr(void) {
    // debug print
    Serial.print("limit switch isr on digital pin "); Serial.println(output_pin_);
    detachInterrupt(digitalPinToInterrupt(output_pin_));

    debounce_active_ = true;
    reattach_interrupt_time_ = millis() + debounce_duration_ms_;
}

