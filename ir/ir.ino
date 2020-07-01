/*
 * IRremote: IRrecvDump - dump details of IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 * LG added by Darryl Smith (based on the JVC protocol)
 */

#include <IRremote.h>

/* 
*  Default is Arduino pin D11. 
*  You can change this to another available Arduino Pin.
*  Your IR receiver should be connected to the pin defined here
*/
int RECV_PIN = 6;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  static unsigned long last = 0;
  if (irrecv.decode(&results)) {
    unsigned long now = results.value;

    if (now == 0xFFFFFFFF and last) {
      Serial.println("go on");
      // do something with last
    }
    else if (now == 0x20DF906F /*mute*/ || now == 0x20DFC03F /*vol-*/ || now == 0x20DF40BF /*vol+*/) {
      Serial.println(results.value, HEX);
      last = now;
    }
    else {
      last = 0;
      Serial.println("Pech");      
    }
    
    irrecv.resume(); // Receive the next value
    delayMicroseconds(16383);
  }
}
