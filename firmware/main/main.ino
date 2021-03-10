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

// Stepper Constants
#define STEPPER_DEG_PER_TICK   0.04
#define STEPPER_ROM_DEG        380
#define STEPPER_HOME_POS_TICKS (STEPPER_ROM_DEG / 2 / STEPPER_DEG_PER_TICK)

// Timer1 Constants
#define TIMER1_ISR_PERIOD_MS           10.0
#define TIMER1_PRESCALER               256
#define SYS_CLOCK_HZ                   16000000
#define TIMER1_INTERRUPT_PERIOD_TICKS  (TIMER1_ISR_PERIOD_MS / 1000) * (SYS_CLOCK_HZ / TIMER1_PRESCALER)

PacketOut packet_out;
PacketIn packet_in;
LIS3DHTR<TwoWire> LIS;
Stepper stepper(&packet_out, STEPPER_LEAD_0, STEPPER_LEAD_1, STEPPER_LEAD_2, STEPPER_LEAD_3);
LimitSwitch switch1(&stepper, /*stepper_pos_ticks*/STEPPER_ROM_DEG / STEPPER_DEG_PER_TICK);
LimitSwitch switch2(&stepper, /*stepper_pos_ticks*/0);
DCMotor dcmotor(&packet_out, DCMOTOR_LEAD_0, DCMOTOR_LEAD_1,
                /*Kp*/0.1, /*Ki*/0, /*Kd*/0, /*period*/TIMER1_ISR_PERIOD_MS/1000, /*bias*/0);

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

// Initializes timer2 interrupt period
void TIMER2_init(void) {
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;
  OCR2A = TIMER1_INTERRUPT_PERIOD_TICKS; // compare match register
  TCCR2B |= (1 << WGM12);                // CTC mode (compare then clear)
  TCCR2B |= (1 << CS12);                 // 256 prescaler
  TIMSK2 |= (1 << OCIE1A);               // enable timer compare interrupt
}

ISR(TIMER1_COMPA_vect) {
  stepper.tick();
}

ISR(TIMER2_COMPA_vect) {
  // For testing allow dc motor to be turned off
  if (enable_dcmotor) {
    dcmotor.tick();
  } else {
    dcmotor.idle();
  }
}

// Take the data read over serial and update the antenna set points
void update_antenna_settings() {
  switch(packet_in.command) {
    case PacketIn::GOTO_POS_SPEED:
      stepper.set_target_position(packet_in.azimuth_hi, packet_in.azimuth_lo);
      dcmotor.set_target_position(packet_in.elevation_hi, packet_in.elevation_lo);
      enable_dcmotor = true;
      break;
    case PacketIn::GOTO_HOME:
      stepper.set_target_position(STEPPER_HOME_POS_TICKS);
      break;
    case PacketIn::PARK:
      // TODO
      break;
    case PacketIn::UNPARK:
      // TODO
      break;
    case PacketIn::MUSIC:
      // TODO
      break;
    case PacketIn::GOTO_AZIMUTH:
      stepper.set_target_position(packet_in.azimuth_hi, packet_in.azimuth_lo);
      break;
    case PacketIn::GOTO_ELEVATION:
      dcmotor.set_target_position(packet_in.elevation_hi, packet_in.elevation_lo);
      enable_dcmotor = true;
      break;
    default:
      break;
  }
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

    // Done reading packet
    if (state == PacketIn::PACKET_SIZE) {
      update_antenna_settings();
      state = 0;
    }
  }
}

// Arduino setup
void setup() {
  // Serial i/o
  Serial.begin(115200);
  
  // Accelerometer
  LIS.begin(Wire, LIS3DHTR_ADDRESS_UPDATED);
  delay(100);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
  LIS.setFullScaleRange(LIS3DHTR_RANGE_2G);
  if (!LIS) {
    packet_out.set_error(PacketOut::ACCELEROMETER_NO_COMM);
  }

  // Initialize all other components
  switch1.init(LIMIT_SWITCH_1_PIN, ISR_limit_switch1);
  switch2.init(LIMIT_SWITCH_2_PIN, ISR_limit_switch2);
  stepper.init();
  dcmotor.init();
  TIMER1_init();
  TIMER2_init();
}

// Arduino main loop
void loop() {
  // Sample accelerometer
  double inclination_milli_rad = atan2(LIS.getAccelerationX(), LIS.getAccelerationY()) * 1000;
  dcmotor.update_current_position(inclination_milli_rad);

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
