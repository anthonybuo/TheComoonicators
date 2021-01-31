#define NUM_STEPPER_INSTR 8U
#define NUM_STEPPER_LEADS 4U

#define STEPPER_LEAD_0 4U
#define STEPPER_LEAD_1 5U
#define STEPPER_LEAD_2 6U
#define STEPPER_LEAD_3 7U

const float timer1_interrupt_ms = 5;
const unsigned int timer1_prescaler = 256;
const unsigned long sys_clock_MHz = 16 * 1000000;
const unsigned int timer1_interrupt_ticks = timer1_interrupt_ms / 1000 * sys_clock_MHz / timer1_prescaler;

const int step[NUM_STEPPER_INSTR][NUM_STEPPER_LEADS] = {
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
  static unsigned int i = 0;
  digitalWrite(STEPPER_LEAD_0, step[i][0]);
  digitalWrite(STEPPER_LEAD_1, step[i][1]);
  digitalWrite(STEPPER_LEAD_2, step[i][2]);
  digitalWrite(STEPPER_LEAD_3, step[i][3]);
  i++;
  i %= NUM_STEPPER_INSTR;
}

void loop() {}
