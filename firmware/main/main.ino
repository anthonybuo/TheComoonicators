#define NUM_STEPPER_INSTR 4U
#define NUM_STEPPER_LEADS 4U

#define STEPPER_LEAD_0 4U
#define STEPPER_LEAD_1 5U
#define STEPPER_LEAD_2 6U
#define STEPPER_LEAD_3 7U

const int step[NUM_STEPPER_INSTR][NUM_STEPPER_LEADS] = {
  {1, 0, 1, 0},
  {1, 0, 0, 1},
  {0, 1, 0, 1},
  {0, 1, 1, 0}
};

void setup() {
  pinMode(STEPPER_LEAD_0, OUTPUT);
  pinMode(STEPPER_LEAD_1, OUTPUT);
  pinMode(STEPPER_LEAD_2, OUTPUT);
  pinMode(STEPPER_LEAD_3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  static unsigned int i = 0;
  digitalWrite(STEPPER_LEAD_0, step[i][0]);
  digitalWrite(STEPPER_LEAD_1, step[i][1]);
  digitalWrite(STEPPER_LEAD_2, step[i][2]);
  digitalWrite(STEPPER_LEAD_3, step[i][3]);
  Serial.print(i);
  Serial.print(": ");
  Serial.print(step[i][0]);
  Serial.print(step[i][1]);
  Serial.print(step[i][2]);
  Serial.print(step[i][3]);
  Serial.println("");
  delay(25);
  i++;
  i %= NUM_STEPPER_INSTR;
}
