#define FASTLED_INTERNAL
#include "FastLED.h"
#define LED_PIN 7
#define NUM_LEDS 60
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB


CRGB leds[NUM_LEDS];

void setup() {
    LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}

uint8_t gHue = 0;

void loop() {
    gHue = gHue % 360; // Faster

    rgbRunningFade();
    //sparkle();
}

void sparkle() {
    FastLED.clear();
    //TODO
    for(int i = 0; i < NUM_LEDS / 8; i++) {
        leds[random(NUM_LEDS)] = CRGB(230, 50, 100);
    }
    FastLED.show();
    delay(1000);
}

void rgbRunningFade() {
    fill_rainbow(leds, NUM_LEDS, gHue, 255 / NUM_LEDS);
    FastLED.show();
    gHue++;
    delay(10);
}

void rgbFade() {
    int pos = beatsin16(5,50,255); // generating the sinwave
    fill_solid(leds, NUM_LEDS, CHSV( gHue, 255, pos)); // CHSV (hue, saturation, value);
    FastLED.show();
    EVERY_N_MILLISECONDS(100) {gHue++;}
}