#include "LIS3DHTR.h"
#include "limit_switch.h"
#include "stepper.h"
#include "dcmotor.h"
#include "packet.h"
#include "low_pass_filter.h"
#include <Math.h>

// System Constants
#define SYS_CLOCK_HZ 16000000

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
#define STEPPER_ROM_DEG        370
#define STEPPER_HOME_POS_TICKS 0
#define STEPPER_BACKLASH_DEG   2.3  // estimate
#define STEPPER_BACKLASH_TICKS STEPPER_BACKLASH_DEG / STEPPER_DEG_PER_TICK

// Timer1 Constants
#define TIMER1_ISR_PERIOD_MS          10.0
#define TIMER1_PRESCALER              256
#define TIMER1_INTERRUPT_PERIOD_TICKS (TIMER1_ISR_PERIOD_MS / 1000) * (SYS_CLOCK_HZ / TIMER1_PRESCALER)

// Timer2 Constants
#define TIMER2_ISR_PERIOD_MS          1.0
#define TIMER2_PRESCALER              256
#define TIMER2_INTERRUPT_PERIOD_TICKS (TIMER2_ISR_PERIOD_MS / 1000) * (SYS_CLOCK_HZ / TIMER2_PRESCALER)

// Timer5 Constants
#define TIMER5_ISR_PERIOD_MS          20.0
#define TIMER5_PRESCALER              256
#define TIMER5_INTERRUPT_PERIOD_TICKS (TIMER5_ISR_PERIOD_MS / 1000) * (SYS_CLOCK_HZ / TIMER5_PRESCALER)

double inclination_milli_rad;
PacketOut packet_out;
PacketIn packet_in;
LIS3DHTR<TwoWire> LIS;
Stepper stepper(&packet_out, STEPPER_LEAD_0, STEPPER_LEAD_1, STEPPER_LEAD_2, STEPPER_LEAD_3, /*backlash_compensation_ticks*/STEPPER_BACKLASH_TICKS);
LimitSwitch switch1(/*id*/0, &packet_out, &stepper, /*stepper_pos_ticks*/STEPPER_ROM_DEG / STEPPER_DEG_PER_TICK);
LimitSwitch switch2(/*id*/1, &packet_out, &stepper, /*stepper_pos_ticks*/0);
DCMotor dcmotor(&packet_out, DCMOTOR_LEAD_0, DCMOTOR_LEAD_1,
                /*Kp*/0.6, /*Ki*/0.125, /*Kd*/0, /*period*/TIMER1_ISR_PERIOD_MS/1000, /*bias*/0);
LowPassFilter LPF(0.45);

// For testing
bool enable_dcmotor = false;
bool enable_stepper = false;

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
  OCR2A = TIMER2_INTERRUPT_PERIOD_TICKS; // compare match register
  TCCR2B |= (1 << WGM12);                // CTC mode (compare then clear)
  TCCR2B |= (1 << CS12);                 // 256 prescaler
  TIMSK2 |= (1 << OCIE1A);               // enable timer compare interrupt
}

// Initializes timer5 interrupt period
void TIMER5_init(void) {
  TCCR5A = 0;
  TCCR5B = 0;
  TCNT5  = 0;
  OCR5A = TIMER5_INTERRUPT_PERIOD_TICKS; // compare match register
  TCCR5B |= (1 << WGM12);                // CTC mode (compare then clear)
  TCCR5B |= (1 << CS12);                 // 256 prescaler
  TIMSK5 |= (1 << OCIE1A);               // enable timer compare interrupt
}

// Timer1 interrupt service routine
ISR(TIMER1_COMPA_vect) {
  if (enable_stepper) {
    stepper.tick();
  } else {
    stepper.idle();
  }
}

// Timer2 interrupt service routine
ISR(TIMER2_COMPA_vect) {
  if (enable_dcmotor) {
    dcmotor.tick();
  } else {
    dcmotor.idle();
  }
}

// Timer5 interrupt service routine
ISR(TIMER5_COMPA_vect) {
  // Gather vitals to send
  stepper.get_current_position(&packet_out.azimuth_hi, &packet_out.azimuth_lo);
  // Use MSB of elevation_hi as sign bit
  if (inclination_milli_rad < 0) {
    packet_out.elevation_hi = (1 << 7) | (((int)inclination_milli_rad*-1 & 0xFF00) >> 8);
    packet_out.elevation_lo = ((int)inclination_milli_rad*-1 & 0xFF);
  } else {
    packet_out.elevation_hi = (((int)inclination_milli_rad & 0xFF00) >> 8);
    packet_out.elevation_lo = ((int)inclination_milli_rad & 0xFF);
  }

  packet_out.sample_rate = 1000 / TIMER5_ISR_PERIOD_MS;
  Serial.write(packet_out.serialize(), PacketOut::PACKET_SIZE);
  packet_out.clear_limit_switch();
}

// Take the data read over serial and update the antenna set points
void update_antenna_settings() {
  switch(packet_in.command) {
    case PacketIn::GOTO_POS_SPEED:
      stepper.set_target_position(packet_in.azimuth_hi, packet_in.azimuth_lo);
      dcmotor.set_target_position(packet_in.elevation_hi, packet_in.elevation_lo);
      enable_dcmotor = true;
      enable_stepper = true;
      break;
    case PacketIn::GOTO_HOME:
      stepper.start_home_sequence();
      stepper.set_target_position(STEPPER_HOME_POS_TICKS);
      enable_stepper = true;
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
    case PacketIn::GOTO_ELEVATION:
      dcmotor.set_target_position(packet_in.elevation_hi, packet_in.elevation_lo);
      enable_dcmotor = true;
      break;
    case PacketIn::GOTO_AZIMUTH:
      stepper.set_target_position(packet_in.azimuth_hi, packet_in.azimuth_lo);
      stepper.set_speed(packet_in.speed_hi, packet_in.speed_lo);
      enable_stepper = true;
      break;
    case PacketIn::EMERGENCY_STOP:
      enable_dcmotor = false;
      enable_stepper = false;
      break;
    case PacketIn::SET_AZIMUTH:
      stepper.set_current_pos((packet_in.azimuth_hi << 8) | packet_in.azimuth_lo);
      stepper.set_target_position(packet_in.azimuth_hi, packet_in.azimuth_lo);
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

    // Wait for start byte
    if ((state == 0) && (serial_data != 255)) {
      continue;
    }
    
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
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_100HZ);
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
  TIMER5_init();
}

// Arduino main loop
void loop() {
  // Sample accelerometer
  double y = LIS.getAccelerationY();
  double z = LIS.getAccelerationZ();

  // Convert acceleration to antenna reference frame
  if (y < 0) {
    inclination_milli_rad = (atan2(y, z) + PI) * 1000;
  }
  else {
    inclination_milli_rad = (atan2(y, z) - PI) * 1000;
  }

  // Filter the noisy reading
  inclination_milli_rad = LPF.filter(inclination_milli_rad);

  // Update dc motor with accel reading if in bounds
  if (!dcmotor.in_desired_rom(inclination_milli_rad)) {
    packet_out.set_error(PacketOut::ACCELEROMETER_READING_OOB);
    dcmotor.idle();
  } else {
    packet_out.clear_error(PacketOut::ACCELEROMETER_READING_OOB);
    dcmotor.update_current_position(inclination_milli_rad);
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

  delay(10);
}
