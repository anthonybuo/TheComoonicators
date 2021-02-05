#include "LIS3DHTR.h"
#include "limit_switch.h"
#include "stepper.h"

// Pinout
#define STEPPER_LEAD_0     4U
#define STEPPER_LEAD_1     5U
#define STEPPER_LEAD_2     6U
#define STEPPER_LEAD_3     7U
#define LIMIT_SWITCH_PIN   2U
#define LIMIT_SWITCH_2_PIN 3U

// Timer1 Constants
#define TIMER1_ISR_PERIOD_MS           1.0
#define TIMER1_PRESCALER               256
#define SYS_CLOCK_HZ                   16000000
#define TIMER1_INTERRUPT_PERIOD_TICKS  (TIMER1_ISR_PERIOD_MS / 1000) * (SYS_CLOCK_HZ / TIMER1_PRESCALER);

LIS3DHTR<TwoWire> LIS;
LimitSwitch switch1, switch2;
Stepper stepper(STEPPER_LEAD_0, STEPPER_LEAD_1, STEPPER_LEAD_2, STEPPER_LEAD_3);

void ISR_limit_switch1(void) {
  switch1.isr();
}

void ISR_limit_switch2(void) {
  switch2.isr();
}

// Initializes timer1 interrupt period
void TIMER1_init(void) {
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = TIMER1_INTERRUPT_PERIOD_TICKS; // compare match register
  TCCR1B |= (1 << WGM12);                // CTC mode (compare then clear)
  TCCR1B |= (1 << CS12);                 // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);               // enable timer compare interrupt
}

ISR(TIMER1_COMPA_vect) {
  stepper.tick();
}

// Arduino setup
void setup() {
  // Accelerometer
  LIS.begin(Wire, LIS3DHTR_ADDRESS_UPDATED);
  delay(100);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
  LIS.setFullScaleRange(LIS3DHTR_RANGE_2G);
  if (!LIS) {
    Serial.println("LIS3DHTR didn't connect.");
    while (1);
  }

  // Serial i/o
  Serial.begin(115200);

  // Limit switches
  switch1.init(LIMIT_SWITCH_PIN, ISR_limit_switch1);
  switch2.init(LIMIT_SWITCH_2_PIN, ISR_limit_switch2);

  // Stepper motor
  stepper.init();

  // Timer 1
  TIMER1_init();
}

// Arduino main loop
void loop() {
  // Sample accelerometer
  Serial.print("accel_x: "); Serial.print(LIS.getAccelerationX());
  Serial.print(", accel_y: "); Serial.print(LIS.getAccelerationY());
  Serial.print(", accel_z: "); Serial.print(LIS.getAccelerationZ());
  Serial.println("");

  // Limit switch debounce if necessary
  if (switch1.debounce_active_ && millis() > switch1.reattach_interrupt_time_) {
    switch1.reenable_interrupt();
  }
  if (switch2.debounce_active_ && millis() > switch2.reattach_interrupt_time_) {
    switch2.reenable_interrupt();
  }

  // Get stepper command from user over serial
  if (Serial.available()) {
    unsigned char serial_data = Serial.read();
    if (serial_data == '-') {
      stepper.stepper_target_position_rot = (Serial.read() - '0') * -1;
    } else {
      stepper.stepper_target_position_rot = (serial_data - '0');
    }
    Serial.print("New stepper command: ");
    Serial.println(stepper.stepper_target_position_rot, DEC);
    stepper.stepper_direction = stepper.stepper_position_deg < (stepper.stepper_target_position_rot * 360) ? 1 : -1;
  }

  delay(250);
}
