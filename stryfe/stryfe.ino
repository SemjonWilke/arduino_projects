#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>

static const unsigned char PROGMEM nums[][144] = {{
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B00000011, B11110000, B00000000, B00000000, B00000000, B00111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
}, {
B11111111, B11111111, B11111111, B11111111, B00000000, B00000000,
B11111111, B11111111, B11111111, B11111111, B00000000, B00000000,
B11111111, B11111111, B11111111, B11111111, B00000000, B00000000,
B11111111, B11111111, B11111111, B11111111, B11111100, B00000000,
B11111111, B11111111, B11111111, B11111111, B11111100, B00000000,
B11111111, B11000000, B00000000, B00000000, B11111100, B00000000,
B11111111, B11000000, B00000000, B00000000, B11111100, B00000000,
B11111111, B11000000, B00000000, B00000000, B11111100, B00000000,
B00000011, B11111111, B11110000, B00000000, B11111100, B00000000,
B00000011, B11111111, B11110000, B00000000, B11111100, B00000000,
B00000000, B00111111, B11110000, B00000000, B11111100, B00000000,
B00000000, B00001111, B11110000, B00000000, B11111100, B00000000,
B00000000, B00001111, B11110000, B00000000, B11111100, B00000000,
B00000000, B00111111, B11110000, B00000000, B11111100, B00000000,
B11111111, B11111111, B11110000, B00000000, B11111111, B11000000,
B11111111, B11111111, B11110000, B00000000, B11111111, B11000000,
B11111111, B11111111, B11110000, B00000000, B11111111, B11111111,
B11111111, B11111111, B11110000, B00000000, B11111111, B11111111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00111111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B00000011, B11110000, B00000000, B00000000, B00000000, B00111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
},{
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11111111, B11111111, B11111111, B00000000, B00001111,
B11111111, B11111111, B11111111, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00001111, B11111111, B11111111,
B11111111, B11000000, B00000000, B00001111, B11111111, B11111111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00111111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B00000011, B11110000, B00000000, B00000000, B00000000, B00111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
},{
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11111111, B11111111, B11111111, B00000000, B00001111,
B11111111, B11111111, B11111111, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11111111, B11111111, B11111111, B00000000, B00001111,
B11111111, B11111111, B11111111, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B00000011, B11110000, B00000000, B00000000, B00000000, B00111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
},{
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B00000011, B11110000, B00000000, B00000000, B00000000, B00001111,
B00000011, B11111111, B11111111, B11111111, B00000000, B00001111,
B00000011, B11111111, B11111111, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000000, B00001111, B00000000, B00111111,
B00000000, B00000000, B00000000, B00001111, B11111111, B11111111,
B00000000, B00000000, B00000000, B00001111, B11111111, B11111111,
},{
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00001111, B11111111, B11111111,
B11111111, B11000000, B00000000, B00001111, B11111111, B11111111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11111111, B11111111, B11111111, B00000000, B00001111,
B11111111, B11111111, B11111111, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B00000011, B11110000, B00000000, B00000000, B00000000, B00111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
},{
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B11111111, B11111111, B11111111,
B11111111, B11000000, B00000000, B11111111, B11111111, B11111111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B00000011, B11110000, B00000000, B00000000, B00000000, B00111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
},{
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B00000011, B11111111, B11111111, B11111111, B00000000, B00001111,
B00000011, B11111111, B11111111, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000011, B11111111, B00000000, B00001111,
B00000000, B00000000, B00000000, B00001111, B00000000, B00111111,
B00000000, B00000000, B00000000, B00001111, B11111111, B11111111,
B00000000, B00000000, B00000000, B00001111, B11111111, B11111111,
},{
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B00000011, B11110000, B00000000, B00000000, B00000000, B00111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
},{
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000011, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11111111, B11111111, B11111111, B00000000, B00001111,
B11111111, B11111111, B11111111, B11111111, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B11111111, B11000000, B00000000, B00000000, B00000000, B00001111,
B00000011, B11110000, B00000000, B00000000, B00000000, B00111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111,
}};

#define OLED_ADDR 0x3C // OLED display TWI address
Adafruit_SSD1306 display(-1);  // -1 = no reset pin

#define SHOTPIN     (2U)
#define RELDPIN     (3U)
#define CNTTOP      (12U)
#define CNTWID      (128/CNTTOP)
#define DEBOUNCE    (20U)

#define LED_PIN     6
#define LED_COUNT   15
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int cntr = CNTTOP;
volatile unsigned long oldTSshot = 0;
volatile unsigned long oldTSreload = 0;
volatile uint8_t colors[LED_COUNT+1];
volatile bool blink = true;

static void _print() {
    display.clearDisplay();
    display.drawBitmap(10, 8, nums[(int)(cntr / 10) % 10], 48, 24, 1);
    display.drawBitmap(68, 8, nums[(uint8_t)cntr % 10], 48, 24, 1);
    display.fillRect((128 - cntr*CNTWID) / 2, 0, cntr*CNTWID, 7, 1);
    display.display();
}

static void _pulse() {
    if (blink) {
        for (int led = 1; led <= LED_COUNT; led++) { // led = target information
            int k = led; // k = source information
            switch (led) {
                11:         k -= 2; // offset -4 (7,8,9,10) = (11,12,13,14)
                5:          k -= 2; // offset -2 (3,4) = (5,6)
                default:    break;  // offset  0 (0,1,2)
            }
            strip.setPixelColor(led-1, strip.Color(colors[k-1], colors[k-1], colors[k-1]));
            strip.setPixelColor(led, strip.Color(colors[k], colors[k], colors[k]));
            colors[led] = colors[k-1];
            colors[led-1] = max(colors[k-1] - ceil(colors[k-1] / 2), 0);
            strip.show();
        }
    }
}

void shotisr() {
    if((millis() - oldTSshot) > DEBOUNCE) {
        cntr--;
        if (cntr > 0) {
            blink = true;
            colors[0] = 250;
            oldTSshot = millis();
        } else {
            blink = false;
            strip.fill(strip.Color(255, 0, 0));
            strip.show();
        }
    }
}

void reloadisr() {
    if((millis() - oldTSreload) > DEBOUNCE) {
        blink = false;
        if (digitalRead(RELDPIN)) { // ammo clip pulled
            cntr = 0;
            strip.fill(strip.Color(0, 0, 255));
            strip.show();
        }
        else { // ammo clip inserted
            cntr = CNTTOP;
            strip.fill(strip.Color(0, 255, 0));
            strip.show();
        }
    }
}

void setup() {
    // initialize switches
    pinMode(SHOTPIN, INPUT_PULLUP);
    attachInterrupt(0, shotisr, LOW);
    pinMode(RELDPIN, INPUT_PULLUP);
    attachInterrupt(0, reloadisr, CHANGE );

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
}

void loop() {
    _print();
    _pulse();
}
