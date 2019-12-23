#include "FastLED.h"
#define LED_PIN 7
#define NUM_LEDS 60
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB


CRGB leds[NUM_LEDS];

void setup() {
    LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}

uint8_t gHue = 0;

void loop() {
    gHue = gHue % 360; // Faster

    sparkle(60, true);
    rgb_running_fade(60);
    rgb_running_fill(3, false);
    sparkle(60, false);
    rgb_fade(60);
    rgb_running_fill(3, true);
}

void rgb_running_fill(uint8_t cycle_count, boolean colorful) {
    gHue = 0;
    for (uint8_t j = 0; j < cycle_count; j++) {
        for (uint8_t n = 0; n < NUM_LEDS; n++) {
            for (uint8_t i = 0; i < NUM_LEDS - n; i++) {
                // Clear tail
                if (i > 0) leds[i - 1] = CHSV(0, 0, 0);

                leds[i] = CHSV(gHue, 255, 200);

                if (i / 2 == 0 || colorful)
                    gHue = (gHue + 1) % 360;

                FastLED.show();
                delay(100);
            }
            FastLED.show();
            delay(500);
        }
    }
}

void sparkle(uint8_t cycle_count, boolean rgb) {
    for (uint8_t j = 0; j < cycle_count; j++) {
        FastLED.clear();
        // Every n-th led
        for(uint8_t i = 0; i < NUM_LEDS / 6; i++) {
            if (rgb) {
                leds[random(NUM_LEDS)] = CHSV(random(360), 255, 200);
            } else {
                leds[random(NUM_LEDS)] = CRGB(230, 50, 100);
            }
        }
        FastLED.show();
        delay(1000);
    }
}

void rgb_running_fade(uint16_t cycle_time) {
    gHue = 0;
    cycle_time = cycle_time * 50;  // convert to seconds
    for (uint16_t i = 0; i < cycle_time; i++) {
        fill_rainbow(leds, NUM_LEDS, gHue, 255 / NUM_LEDS);
        FastLED.show();
        gHue = (gHue + 1) % 360;
        delay(20);
    }
}

void rgb_fade(uint16_t cycle_time) {
    gHue = 0;
    cycle_time = cycle_time * 50;  // convert to seconds
    for (uint16_t i = 0; i < cycle_time; i++) {
        uint8_t pos = beatsin16(5,50,150); // generating the sinwave
        fill_solid(leds, NUM_LEDS, CHSV( gHue, 255, pos));
        FastLED.show();
        gHue = (gHue + 1) % 360;
        delay(100);
    }
}