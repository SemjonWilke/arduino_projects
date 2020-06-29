#define AHID

#ifdef AHID
  #include "HID-Project.h"
#endif

int16_t buttonarray[17] = {0};
int16_t analogbutton[2] = {0};
int16_t joystick[2] = {0};

#define PER 0.7

void setup() {
  /*  set pins to high if button is not pressed
   *  if its pressed the pin is grounded (low) */
  for (int i = 0; i < 17; i++) {
    if (i == 1 || i == 11 || i == 12 || i == 13) {continue;}
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);

#ifdef AHID
    Gamepad.begin();
    #warning AHID
#else
    Serial.begin(9600);
    #warning NO AHID
#endif
}

void loop() {
  /* if digitalread(button) == 1 then it's not pressed hence the array should to be zero */
  /* read buttons */
  for (int i = 0; i < 17; i++){
    if (i == 1 || i == 11 || i == 12 || i == 13){ continue; }
    buttonarray[i] = (digitalRead(i)) ? 0 :  1;
  }
  analogbutton[0] = analogRead(A0);
  analogbutton[1] = analogRead(A1);
  
  /* read joystick */
  joystick[0] = analogRead(A2);
  joystick[1] = analogRead(A3);
  /* debounce joystick */
  joystick[0] = (analogRead(A2) * PER + joystick[0] * (1-PER));
  joystick[1] = (analogRead(A3) * PER + joystick[1] * (1-PER));
  
  /* debounce buttons */
  for (int i = 1; i <= 16; i++){
    if (i == 1 || i == 11 || i == 12 || i == 13){ continue; }
    buttonarray[i] = ((!buttonarray[i]) & digitalRead(i)) ? 0 :  1;
  }
  analogbutton[0] = (analogRead(A0) * PER + analogbutton[0] * (1-PER));
  analogbutton[1] = (analogRead(A1) * PER + analogbutton[1] * (1-PER));

/* write buttons */
  for (int i = 0; i <= 16; i++) {
/* nc */    
    if (i == 1 || i == 13) { continue; }
/* analog buttons */
    if (i == 11 || i == 12) {
#ifdef AHID
      if (analogbutton[i - 11] < 30) {
        Gamepad.press(i);
      } else {
        Gamepad.release(i);
      }
#else
      Serial.print("A");
      Serial.print(i - 11);
      Serial.print(": ");
      Serial.print(analogbutton[i - 11]);
      Serial.print(", ");
#endif
      continue;
    }
    
/* digital buttons */
    if (buttonarray[i]) {
#ifdef AHID
      Gamepad.press(i);
    } else {
      Gamepad.release(i);
#else
      Serial.print("D");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(buttonarray[i]);
      Serial.print(", ");
#endif
    }
  }

/* write joystick */
  joystick[0] <<= 6;
  joystick[1] <<= 6;

#ifdef AHID
  Gamepad.xAxis(INT16_MAX - joystick[0]);
  Gamepad.yAxis(INT16_MAX - joystick[1]);

/* send to host */
  Gamepad.write();
#else
  Serial.print("X: ");
  Serial.print(joystick[0], HEX);
  Serial.print(", Y: ");
  Serial.print(joystick[1], HEX);
  Serial.println();
#endif
  
/* write dpad clockwise 9=up->10=right->11=down->12=left 
  int dpad = dbounsarray[9] << 3 && dbounsarray[10] << 2 && dbounsarray[11] << 1 && dbounsarray[12];
  switch (dpad) {
#ifdef HID
    case (0xC): Gamepad.dPad1(GAMEPAD_DPAD_UP_RIGHT); break;
    case (0x6): Gamepad.dPad1(GAMEPAD_DPAD_DOWN_RIGHT); break;
    case (0x3): Gamepad.dPad1(GAMEPAD_DPAD_DOWN_LEFT); break;
    case (0x9): Gamepad.dPad1(GAMEPAD_DPAD_UP_LEFT); break;
    case (0x8): Gamepad.dPad1(GAMEPAD_DPAD_UP); break;
    case (0x4): Gamepad.dPad1(GAMEPAD_DPAD_RIGHT); break;
    case (0x2): Gamepad.dPad1(GAMEPAD_DPAD_DOWN); break;
    case (0x1): Gamepad.dPad1(GAMEPAD_DPAD_LEFT); break;
    default: Gamepad.dPad1(GAMEPAD_DPAD_CENTERED); break;
#endif
  }*/
}
