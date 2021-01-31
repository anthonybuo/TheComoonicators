#define NUM_STEPPER_INSTR 8U
#define NUM_STEPPER_LEADS 4U

#define STEPPER_LEAD_0 4U
#define STEPPER_LEAD_1 5U
#define STEPPER_LEAD_2 6U
#define STEPPER_LEAD_3 7U

#define STEP_ANGLE_DEG 0.25
#define TIMER1_ISR_PERIOD_MS 1.0
#define TIMER1_PRESCALER 256
#define SYS_CLOCK_HZ 16000000

const unsigned int timer1_interrupt_ticks = (TIMER1_ISR_PERIOD_MS / 1000) * (SYS_CLOCK_HZ / TIMER1_PRESCALER);

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

void GPIO_init(void) {
  pinMode(STEPPER_LEAD_0, OUTPUT);
  pinMode(STEPPER_LEAD_1, OUTPUT);
  pinMode(STEPPER_LEAD_2, OUTPUT);
  pinMode(STEPPER_LEAD_3, OUTPUT);
}

void TIMER1_init(void) {
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = timer1_interrupt_ticks;      // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
}

void setup() {
  noInterrupts();
  GPIO_init();
  TIMER1_init();
  interrupts();
}

ISR(TIMER1_COMPA_vect) {
  static int i = 0;
  static float stepper_position_deg = 0;
  static int direction = 1;
  digitalWrite(STEPPER_LEAD_0, half_step[i][0]);
  digitalWrite(STEPPER_LEAD_1, half_step[i][1]);
  digitalWrite(STEPPER_LEAD_2, half_step[i][2]);
  digitalWrite(STEPPER_LEAD_3, half_step[i][3]);
  stepper_position_deg += direction * STEP_ANGLE_DEG / 2;
  if (stepper_position_deg >= 360) {
    direction = -1;
  } else if (stepper_position_deg <= 0) {
    direction = 1;
  }
  i += direction;
  if (i < 0) {
    i = NUM_STEPPER_INSTR - 1;
  }
  i %= NUM_STEPPER_INSTR;
}

void loop() {}
