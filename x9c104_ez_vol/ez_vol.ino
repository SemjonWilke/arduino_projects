#define CHIP_SELECT (3U)
#define UP_DOWN (1U)
#define INCREMENT (2U)
#define MIN_WAIT (16383U)

#define test true

void setup() {
  pinMode(CHIP_SELECT, OUTPUT);
  pinMode(UP_DOWN, OUTPUT);
  pinMode(INCREMENT, OUTPUT);
  
  digitalWrite(CHIP_SELECT, HIGH);
  digitalWrite(UP_DOWN, LOW);
  digitalWrite(INCREMENT, HIGH);

  //IRremote: register some key with wipe(true)
  //IRremote: register some key with wipe(false)
}

void wipe(bool up) {
  digitalWrite(CHIP_SELECT, LOW);
  if (up) digitalWrite(UP_DOWN, HIGH);
  else    digitalWrite(UP_DOWN, LOW);
  digitalWrite(INCREMENT, HIGH);
  delayMicroseconds(MIN_WAIT); //tcl
  digitalWrite(INCREMENT, LOW);
  delayMicroseconds(MIN_WAIT); //til
}

void store(void) {
  digitalWrite(INCREMENT, HIGH);
  delayMicroseconds(MIN_WAIT);
  digitalWrite(CHIP_SELECT, HIGH);
}

void loop() {
  if (test) {
    for (int i = 0;; i++) {
      if (i > 199) i = 0;
      else if (i > 99) wipe(true);
      else wipe(false);
    }
  }

  /* if (no input for some time) {
   *   store()
   * }
   */
  
  
}
