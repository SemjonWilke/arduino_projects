#include <IRremote.h>

#define MUTE_RLY    (1U)
#define CHIP_SELECT (2U)
#define UP_DOWN     (3U)
#define INCREMENT   (4U)
#define RECV_PIN     (6U)

#define MIN_WAIT    (16383U)
#define DEF_TIMEOUT (15000UL)

#define KEY_REPEAT  (0xFFFFFFFF)
#define KEY_MUTE    (0x20DF906F)
#define KEY_VOLUP   (0x20DF40BF)
#define KEY_VOLDW   (0x20DFC03F)

#define DEBUG
#define TEST

IRrecv irrecv(RECV_PIN);
decode_results results;

void wipe(bool up)
{
  Serial.print("Volume ");
  digitalWrite(CHIP_SELECT, LOW);
  digitalWrite(INCREMENT, HIGH);
  if (up) {
    digitalWrite(UP_DOWN, HIGH);
    Serial.println("up");
  }
  else {
    digitalWrite(UP_DOWN, LOW);
    Serial.println("down");
  }
  delayMicroseconds(MIN_WAIT); //tcl
  digitalWrite(INCREMENT, LOW);
  delayMicroseconds(MIN_WAIT); //til
}

void store(void)
{
  Serial.println("Storing");
  digitalWrite(INCREMENT, HIGH);
  delayMicroseconds(MIN_WAIT);
  digitalWrite(CHIP_SELECT, HIGH);
}

void mute(void)
{
  Serial.println("Muteing");
  digitalWrite(MUTE_RLY, !digitalRead(MUTE_RLY));
}

void execute_key(unsigned long key)
{
  Serial.print("Execute Key: 0x");
  Serial.println(key, HEX);
  switch(key){
    case KEY_MUTE:  mute();      break;
    case KEY_VOLUP: wipe(true);  break;
    case KEY_VOLDW: wipe(false); break;
    default: /*never go here please*/ break;
  }
}

// ############################################################################
void setup(void)
{
#ifdef DEBUG
  Serial.begin(9600);
#endif
  Serial.println("Setting up");

  pinMode(CHIP_SELECT, OUTPUT);
  pinMode(UP_DOWN, OUTPUT);
  pinMode(INCREMENT, OUTPUT);
  digitalWrite(CHIP_SELECT, HIGH);
  digitalWrite(UP_DOWN, LOW);
  digitalWrite(INCREMENT, HIGH);

  pinMode(MUTE_RLY, OUTPUT);
  digitalWrite(MUTE_RLY, LOW);

  irrecv.enableIRIn(); // Start the IR receiver

  Serial.println("Setup done");
}

void loop(void)
{
//TODO: get rid of test
#ifdef TEST
  Serial.println("Testing:");
  for (int i = 0;; i++) {
    Serial.print("i: ");
    if (i > 199) i = 0;
    else if (i > 99) wipe(true);
    else wipe(false);
  }
#endif

  static long store_timeout = millis();
  static bool stored = true;
  static unsigned long last = 0;

  if (irrecv.decode(&results)) {
    unsigned long now = results.value;

    if (now == KEY_REPEAT & last > 0 & last != KEY_MUTE) {
      stored = false;
      execute_key(last);
    }
    else if (now == KEY_MUTE || now == KEY_VOLDW || now == KEY_VOLUP) {
      stored = false;
      last = now;
      execute_key(now);
    }
    else {
      last = 0;
    }

    delayMicroseconds(MIN_WAIT);
    irrecv.resume(); // Receive the next value
  }

  if (!stored & (store_timeout + DEF_TIMEOUT) < (millis())) {
    store();
    stored = true;
  }
}
