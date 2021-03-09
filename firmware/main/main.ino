#include "LIS3DHTR.h"
#include "limit_switch.h"
#include "stepper.h"
#include "dcmotor.h"
#include "packet.h"
#include <Math.h>

// Pinout
#define STEPPER_LEAD_0     3U
#define STEPPER_LEAD_1     2U
#define STEPPER_LEAD_2     5U
#define STEPPER_LEAD_3     6U
#define LIMIT_SWITCH_1_PIN 18U
#define LIMIT_SWITCH_2_PIN 19U
#define DCMOTOR_LEAD_0     7U
#define DCMOTOR_LEAD_1     8U

// Timer1 Constants
#define TIMER1_ISR_PERIOD_MS           10.0
#define TIMER1_PRESCALER               256
#define SYS_CLOCK_HZ                   16000000
#define TIMER1_INTERRUPT_PERIOD_TICKS  (TIMER1_ISR_PERIOD_MS / 1000) * (SYS_CLOCK_HZ / TIMER1_PRESCALER)

LIS3DHTR<TwoWire> LIS;
LimitSwitch switch1, switch2;
Stepper stepper(STEPPER_LEAD_0, STEPPER_LEAD_1, STEPPER_LEAD_2, STEPPER_LEAD_3);
DCMotor dcmotor(DCMOTOR_LEAD_0, DCMOTOR_LEAD_1);
PacketOut packet_out;
PacketIn packet_in;

// For testing
bool enable_dcmotor = false;

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

// Receive a command from the serial port
void poll_command() {
  while (Serial.available()) {
    unsigned char serial_data = Serial.read();
    static unsigned int state = 0;
    if ((state == 0) && (serial_data == 255)) {
      // Read the start byte (unused)
    } else {
      // Read the payload and stop byte
      packet_in.read_byte(serial_data, state);
    }
    state++;
    if (state == PacketIn::PACKET_SIZE) {
      // Change antenna settings with new command
      stepper.set_target_position(packet_in.azimuth_hi, packet_in.azimuth_lo);
      // For testing, enable dc motor if elevation_lo is non-zero
      enable_dcmotor = (bool)packet_in.elevation_lo;
      state = 0;
    }
  }
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
  switch1.init(LIMIT_SWITCH_1_PIN, ISR_limit_switch1);
  switch2.init(LIMIT_SWITCH_2_PIN, ISR_limit_switch2);

  // Stepper motor
  stepper.init();

  // DC motor
  dcmotor.init();

  // Timer 1
  TIMER1_init();
}

// Arduino main loop
void loop() {
  // Sample accelerometer
  double inclination_milli_rad = atan2(LIS.getAccelerationX(), LIS.getAccelerationY()) * 1000;

  // Test dc motor if desired
  if (enable_dcmotor) {
    dcmotor.set_vel(/*speed*/abs(inclination_milli_rad) / 10,
                    /*direction*/inclination_milli_rad > 0);
  } else {
    dcmotor.idle();
  }

  // Limit switch debounce if necessary
  if (switch1.debounce_active_ && millis() > switch1.reattach_interrupt_time_) {
    switch1.reenable_interrupt();
  }
  if (switch2.debounce_active_ && millis() > switch2.reattach_interrupt_time_) {
    switch2.reenable_interrupt();
  }

  // Get command packet
  poll_command();

  // To computer application
  stepper.get_current_position(&packet_out.azimuth_hi, &packet_out.azimuth_lo);
  packet_out.elevation_hi = (((int)inclination_milli_rad & 0xFF00) >> 8);
  packet_out.elevation_lo = ((int)inclination_milli_rad & 0xFF);
  Serial.write(packet_out.serialize(), PacketOut::PACKET_SIZE);

  delay(250);
}
