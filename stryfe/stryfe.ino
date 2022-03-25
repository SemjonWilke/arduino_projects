#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include "numbers.h"

#define OLED_ADDR 0x3C // OLED display TWI address
Adafruit_SSD1306 display(-1);  // -1 = no reset pin

#define SHOTPIN     (2U)
#define RELDPIN     (3U)
#define CNTTOP      (12U)
#define CNTWID      (128/CNTTOP)
#define DEBOUNCE    (20U)

#define LED_PIN     6
#define LED_COUNT   15
#define LED_STEPS   15
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int magCntr;
bool redraw;

volatile unsigned long oldTSshot = 0;
volatile unsigned long oldTSreload = 0;
volatile bool blink = true;
uint8_t colors[LED_COUNT+1];

static void _redrawLcd() {
    if (redraw) {
        redraw = false;
        display.clearDisplay();
        display.drawBitmap(10, 8, nums[(int)(magCntr / 10) % 10], 48, 24, 1);
        display.drawBitmap(68, 8, nums[(uint8_t)magCntr % 10], 48, 24, 1);
        display.fillRect((128 - magCntr*CNTWID) / 2, 0, magCntr*CNTWID, 7, 1);
        display.display();
    }
}

//[5,15,17] * 15 = [75,225,255]
int colorSteps = LED_STEPS * LED_COUNT;
int colorBase[] = {5,15,17};

static void _pulseLed() {
    if (not blink) {
        return;
    }

    int fac = 0;
    
    for (int step = 0; step <= colorSteps*2; step++) {

        for (int led = LED_COUNT - 1; led == 0; led--){
            if (led == 0) {
                if (step > LED_STEPS) colors[led] = max(colors[led] - 1, 0);
                else                   colors[led] = max(colors[led] + 1, LED_STEPS);
            } else {
                                       colors[led] = colors[led - 1];
            }
            
            if (step == (colorSteps*2)) fac = 0;
            else                        fac = colors[led];
            
            strip.setPixelColor(led, strip.Color(colorBase[0]*fac, colorBase[1]*fac, colorBase[2]*fac));
        }
        strip.show();
        delay(15);
        
    }
    blink = false;
}

/*
Farbübergang: Startfarbe, Zielfarbe, Schritte
    0 -> 256 in 12 Schritten = 256-0 /12 = 21,333
    44 -> 130 in 12 Schritten = 130-40 /12 = 7,5
    58 -> 14 in 9 Schritten = 58-14 /9 = 4,88

Anzahl Schritte: Schrittdauer, Dauer für gesamten Farbwechsel
    Farbwechseldauer / Schrittdauer
*/

void shotisr() {
    Serial.println("TEST");
    if((millis() - oldTSshot) > DEBOUNCE) {
        blink = true;
        colors[0] = 1;
        oldTSshot = millis();
    }
}

/*
void reloadisr() {
    if((millis() - oldTSreload) > DEBOUNCE) {
        blink = false;
        if (digitalRead(RELDPIN)) { // ammo clip pulled
            magCntr = 0;
            strip.fill(strip.Color(0, 0, 255));
            strip.show();
        }
        else { // ammo clip inserted
            magCntr = CNTTOP;
            strip.fill(strip.Color(0, 255, 0));
            strip.show();
        }
    }
}
*/
void setup() {
    // initialize switches
    pinMode(SHOTPIN, INPUT_PULLUP);
    attachInterrupt(0, shotisr, LOW);
    /*pinMode(RELDPIN, INPUT_PULLUP);
    attachInterrupt(0, reloadisr, CHANGE );*/

    // initialize and clear display
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    display.display();

    // initialize NeoPixel strip object
    strip.begin();
    strip.show();
    strip.setBrightness(50);

    // initialize variables
    memset(colors,0,LED_COUNT);
    magCntr = CNTTOP;
    //redraw = true;
    redraw = false;
}

void loop() {
    _redrawLcd();
    _pulseLed();
}

