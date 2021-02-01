#include "LIS3DHTR.h"
#include <Wire.h>
#define WIRE Wire

// Stepper Constants
#define NUM_STEPPER_INSTR 8U
#define NUM_STEPPER_LEADS 4U
#define STEPPER_LEAD_0    4U
#define STEPPER_LEAD_1    5U
#define STEPPER_LEAD_2    6U
#define STEPPER_LEAD_3    7U
#define STEP_ANGLE_DEG    0.25

// Limit switch constants
#define LIMIT_SWITCH_PIN 2U

// Timer1 Constants
#define TIMER1_ISR_PERIOD_MS           1.0
#define TIMER1_PRESCALER               256
#define SYS_CLOCK_HZ                   16000000
#define TIMER1_INTERRUPT_PERIOD_TICKS  (TIMER1_ISR_PERIOD_MS / 1000) * (SYS_CLOCK_HZ / TIMER1_PRESCALER);

// Accelerometer
LIS3DHTR<TwoWire> LIS;
float accel_x_g, accel_y_g, accel_z_g;

// Half stepping lookup
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

// Direction of rotation of stepper: -1 or 1
volatile int stepper_direction = 1;
// Flag for debouncing limit switch
volatile bool limit_switch_interrupt_active = true;
// When to reattach the limit switch interrupt in ms since program start
volatile long reattach_interrupt_time_ms = 0;

// Initializes stepper control outputs
void STEPPER_init(void) {
  pinMode(STEPPER_LEAD_0, OUTPUT);
  pinMode(STEPPER_LEAD_1, OUTPUT);
  pinMode(STEPPER_LEAD_2, OUTPUT);
  pinMode(STEPPER_LEAD_3, OUTPUT);
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

// Initializes serial debugging
void SERIAL_init(void) {
  Serial.begin(115200);
}

// Initializes accelerometer
void ACCEL_init(void) {
  LIS.begin(WIRE, LIS3DHTR_ADDRESS_UPDATED);
  delay(100);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
  LIS.setFullScaleRange(LIS3DHTR_RANGE_2G);
  if (!LIS) {
    Serial.println("LIS3DHTR didn't connect.");
    while (1);
  }
}

// Initializes limit switch
void LIMIT_SWITCH_init(void) {
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(LIMIT_SWITCH_PIN), ISR_limit_switch, RISING);
  pinMode(LED_BUILTIN, OUTPUT);
}

// Limit switch ISR triggered on switch press
void ISR_limit_switch(void) {
  detachInterrupt(digitalPinToInterrupt(LIMIT_SWITCH_PIN));  // debounce
  limit_switch_interrupt_active = false;
  reattach_interrupt_time_ms = millis() + 1000;
  // Serial.println("LIMIT SWITCH ISR!!");
  stepper_direction *= -1;
}

// Timer1 ISR
ISR(TIMER1_COMPA_vect) {
  static int i = 0;
  static float stepper_position_deg = 0;

  // Stepper command
  digitalWrite(STEPPER_LEAD_0, half_step[i][0]);
  digitalWrite(STEPPER_LEAD_1, half_step[i][1]);
  digitalWrite(STEPPER_LEAD_2, half_step[i][2]);
  digitalWrite(STEPPER_LEAD_3, half_step[i][3]);

  // Update position and stepper_direction
  stepper_position_deg += stepper_direction * STEP_ANGLE_DEG / 2;

  // Update counter
  i += stepper_direction;
  if (i < 0) {
    i = NUM_STEPPER_INSTR - 1;
  }
  i %= NUM_STEPPER_INSTR;
}

// Arduino setup
void setup() {
  ACCEL_init();  // needs interrupts on for I2C init
  noInterrupts();
  SERIAL_init();
  LIMIT_SWITCH_init();
  STEPPER_init();
  TIMER1_init();
  interrupts();
}

// Arduino main loop
void loop() {
  // Display accelerometer data
  LIS.getAcceleration(&accel_x_g, &accel_y_g, &accel_z_g);
  Serial.print(accel_x_g); Serial.print(" ");
  Serial.print(accel_y_g); Serial.print(" ");
  Serial.print(accel_z_g); Serial.print(" ");
  Serial.println("");

  // Limit switch debounce if necessary
  if (millis() > reattach_interrupt_time_ms && !limit_switch_interrupt_active) {
    EIFR |= (1 << INTF4);  // cancel all pending interrupts on limit switch digital input
    attachInterrupt(digitalPinToInterrupt(LIMIT_SWITCH_PIN), ISR_limit_switch, RISING);
    limit_switch_interrupt_active = true;
  }

  delay(1000);
}
