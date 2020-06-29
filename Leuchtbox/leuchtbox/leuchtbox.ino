#include <avr/sleep.h>

#define silent
#define shutdowntime (15000UL)

unsigned int  btn[5]; //Button pressed (t+0)? 0 = pressed, 1 = not pressed
unsigned int  btnlast[5]; //Button last pressed (t+1)? 0 = pressed, 1 = not pressed
unsigned int  led[5]; //LED light? 0 = off, 1 = on, 2 = toggle, 3 = no change
unsigned int  ledstate[5]; //LED light! 0 = off, 1 = on
unsigned int  game; //keep track of current game for looping
unsigned int  i; //debug
unsigned long last; //last pressed
unsigned int  ledinvert; //invert led lightning
int           game_four = -1; //game four state var! -1 = init, else array position

//############################################################################################
//WAKEUP AFTER SHUTDOWN
void wakeup(void) {
  detachInterrupt(digitalPinToInterrupt(3));
  last = millis();
}

//check if any button was pressed or released since last loop
bool changedbutton(void) {
  int code = false;
  for (int j = 0; j < 5; j++) {
    if (btnlast[j] != btn[j]) {
      code = true;
    }
    btnlast[j] = btn[j];
  }
  return code;
}

// returns the pressed button+1 or 0 if more than one is pressed
int exactlyone(void) {
    if ((btn[0] + btn[1] + btn[2] + btn[3] + btn[4]) != 4) {
        /*Serial.print("Not exactly one button pressed: ");
        Serial.print(btn[0]); Serial.print(", ");
        Serial.print(btn[1]); Serial.print(", ");
        Serial.print(btn[2]); Serial.print(", ");
        Serial.print(btn[3]); Serial.print(", ");
        Serial.println(btn[4]);*/
        return 0;
    }
    for (int i = 0; i < 5; i++) {
        if(btn[i] == 0){
            return i+1;
        }
    }
}

//############################################################################################
void ledWrite(unsigned int led, unsigned int mode) {
  digitalWrite((led*2)+4, (mode == ledinvert) ? 0 : 1);
}

/*enum color { yellow = 4, red = 6, white = 8, blue = 10, green = 12, };*/
void changeallleds(int val) {
  for (int j = 0; j < 5; j++) {
    switch(val) {
      case 0: ledstate[j] = 0; break;
      case 1: ledstate[j] = 1; break;
      case 2: ledstate[j] = (ledstate[j] + 1) % 2; break;
    }
    ledWrite(j, ledstate[j]);
  }
}

void changeleds(void) {
  for (int j = 0; j < 5; j++) {
    switch (led[j]) {
      case 0: ledstate[j] = 0; break;
      case 1: ledstate[j] = 1; break;
      case 2: ledstate[j] = (ledstate[j] + 1) % 2; break;
      default: break;
    }
    ledWrite(j, ledstate[j]);
    led[j] = 3;
  }
}

//make any LED blink
void ledblink(int led, int runs = 3, int leave = 0) {
  for (int j = 0; j < runs; j++) {
    ledWrite(led, 0);
    delay(100);
    ledWrite(led, 1);
    delay(100);
  }
  if (leave) {
    ledWrite(led, 0);
    ledstate[led] = 0;
  } else {
    ledstate[led] = 1;
  }
}

//led effect for shutdown
void ledexit(int leave = 0) {
    changeallleds(1);
    delay(220);
    for (int j = 5; j > 0; j -= 2) {
      delay(180);
      ledWrite(j, 0);
    }
    changeallleds(leave);
}

//############################################################################################
// PLAY GAME 1 !!!!!!!!!!!!!!!!!!!!!
// PRESSED btn LIGHT UP
void gameone() {
  for (int j = 0; j < 5; j++) {
    led[j] = btn[j] ? 0 : 1;
  }
  changeleds();
}

// PLAY GAME 2 !!!!!!!!!!!!!!!!!!!!!
// ALADINS LAMPS
void gametwo() {
  static unsigned int tgltwo[5] = {0};
  if (!exactlyone()) { return; }
  Serial.println("ALADIN");
  unsigned int pressed = exactlyone()-1;
  Serial.print("Pressed: ");
  Serial.println(pressed);
  switch (pressed) {
    case 0: led[0] = 2; led[1] = 2; led[2] = 2; led[3] = 2; led[4] = 2; break;
    case 1: led[0] = 2; led[1] = 2; led[2] = 2; led[3] = 3; led[4] = 2; break;
    case 2: led[0] = 2; led[1] = 2; led[2] = 2; led[3] = 2; led[4] = 3; break;
    case 3: led[0] = 2; led[1] = 3; led[2] = 2; led[3] = 2; led[4] = 2; break;
    case 4: led[0] = 2; led[1] = 2; led[2] = 3; led[3] = 2; led[4] = 2; break;
    default: break;
  }
  changeleds();
  if(ledstate[0]+ledstate[1]+ledstate[2]+ledstate[3]+ledstate[4] == 0 ||
     ledstate[0]+ledstate[1]+ledstate[2]+ledstate[3]+ledstate[4] == 5) {
     ledexit(ledstate[0]);
  }
  delay(200);
}

// PLAY GAME 3 !!!!!!!!!!!!!!!!!!!!!
// PRESS CORRECT BUTTON
void gamethree() {
  Serial.println("CORRECT BUTTON");
  static unsigned int lastledthree = random(0,5);
  if (!exactlyone()) { return; }

  if (!btn[lastledthree] && led[lastledthree]) {
    Serial.print("btn and led = ");
    Serial.println(lastledthree);
    unsigned int oldlastledthree = lastledthree;
    do {
      lastledthree = random(0,5);
    } while (lastledthree == oldlastledthree);
  }
  for (int j = 0; j < 5; j++) {
      Serial.print("for ");
      Serial.print(j);
      Serial.print(": ");
      led[j] = 0;
      led[lastledthree] = 1;
      Serial.println(led[j]);
  }
  changeleds();
}

// PLAY GAME 4 !!!!!!!!!!!!!!!!!!!!!
// PASSWORD
void gamefour() {
  Serial.println("PASSWORD");
  static unsigned int pw[5] = {0,1,2,3,4};

  if (game_four == -1) { //init (at start and after win)
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < round(20/i); j++) {
        int k = random(0,5);
        int a = pw[i];
        int b = pw[k];
        pw[i] = b;
        pw[k] = a;
      }
    }
    for (int i = 0; i < 5; i++){
      ledblink(pw[i],2,1);
      led[i] = 0;
    }
    game_four = 0;
    return;
  }

  if (!exactlyone()) { return; }

  int pressed = exactlyone()-1;
  Serial.print("Haben: ");
  Serial.print(pressed);
  Serial.print(", Soll: ");
  Serial.println(pw[game_four]);

  if (pw[game_four] == pressed) { // correct button pressed
    Serial.println("Correct");
    if (game_four == 4){ // game won
      ledexit();
      game_four = -1;
      return;
    }
    led[pressed] = 1;
    game_four++;
  }
  else if (ledstate[pressed]) { // old button pressed
    //room for fancy addons
  }
  else { // wrong button pressed
    Serial.println("wrong");
    for (int i = 0; i < 5; i++){
      if(i >= game_four){
        led[pw[i]] = 0;
      }
      else{
        led[pw[i]] = 1;
      }
    }
    game_four = 0;
  }
  changeleds();
}

// PLAY GAME 5 !!!!!!!!!!!!!!!!!!!!!
// INVERT LEDS
void gamefive(void) {
  ledinvert = ledinvert ? 0 : 1;
  game = (random(2)*2)+1;
}

//############################################################################################
//INIT FUNCTION
void setup() {
  pinMode(3, INPUT_PULLUP);
  pinMode(4, OUTPUT);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, OUTPUT);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, OUTPUT);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, OUTPUT);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, OUTPUT);

  randomSeed(654984);

  detachInterrupt(digitalPinToInterrupt(3));
#ifndef silent
  Serial.begin(9600);
#endif
  last = millis();
  game = 0;
  i = 0;
  ledinvert = 0;
}

//############################################################################################
//MAIN LOOP
void loop() {
  //READ btn ##################
  for (int j = 0; j < 5; j++) {
    btn[j] = digitalRead((j*2)+3);
  }
  //DEBOUNCE btn
  delay(50);
  for (int j = 0; j < 5; j++) {
    btn[j] = btn[j] & digitalRead((j*2)+3) ? 1 : 0;
  }
  //RESET
  if ((btn[0] + btn[1] + btn[2] + btn[3] + btn[4]) == 0) {
    for (int j = 0; j < 5; j++) {
      btn[j] = 1;
      changeallleds(1);
    }
    game = 0;
    delay(500);
  }

  //CHOOSE GAME ##################
  if (!game) {
    game_four = -1;
    ledinvert = 0;
    for (int j = 0; j < 5; j++) {
      led[j] = 1;
    }
    changeallleds(1);
    int j;
    for (j = 0; j < 5; j++) {
      if (!btn[j]) {
        game = j+1;
        break;
      }
    }
    if (game) {
      changeallleds(0);
      ledblink(j);
    }
  }

  //PLAY GAME ##################
  else {
    Serial.print("game ");
    switch (game) {
      case 1: gameone();   break;  //yellow
      case 2: gametwo();   break;  //red
      case 3: gamethree(); break;  //white
      case 4: gamefour();  break;  //blue
      case 5: gamefive();  break;  //green
      default: gameone();   break;
    }
    changeleds();
  }

  //CHECK IF PRESSED OR RELEASED IN LAST #shutdowntime
  if (changedbutton()) {
    last = millis();
    Serial.println("press");
    i = 0;
  }
  //CHECK IF SHUTDOWN TIME IS REACHED
  else if ((last + shutdowntime) < (millis())) {
    ledexit();

    attachInterrupt(digitalPinToInterrupt(3), wakeup, LOW);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
    sleep_disable();
    delay(500);
    game = 0;
    i = 0;
  } else {
    Serial.print(i++);
    Serial.println(" no press");
  }
}
