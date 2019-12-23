#include "FastLED.h"

#define LED_PIN 7
#define NUM_LEDS 60
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB


CRGB leds[NUM_LEDS];

void setup() {
    LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}

uint16_t gHue = 0;

void loop() {
    gHue = gHue % 360; // Faster

    rgb_running_fill(3, false, 2);
    sparkle(60, CHSV(0, 0, 0), true);
    rgb_spiral_fill(60, 6, 6);
    rgb_running_fade(60);
    sparkle(60, CHSV(343, 255, 200), false);
    rgb_spiral_fill(60, 6, 2);
    rgb_fade(60);
    rgb_running_fill(3, true, 0);
    rgb_spiral_fill(60, 6, 1);
}

void rgb_spiral_fill(uint8_t cycle_count, uint8_t colors_count, uint8_t splits_count) {
    gHue = 0;
    uint8_t split_length = NUM_LEDS / splits_count;
    for (uint8_t i = 0; i < cycle_count; i++) {
        for (uint8_t k = 0; k < colors_count; k++) {
            for (uint8_t j = 0; j < split_length; j++) {
                for (uint8_t l = 0; l < splits_count; l++) {
                    if (j + l * split_length < NUM_LEDS)
                        leds[j + l * split_length] = CHSV(gHue, 255, 200);
                }
                //leds[j] = CHSV(gHue, 255, 200);
                FastLED.show();
                delay(20 * splits_count);
            }
            gHue = (gHue + 360 / colors_count) % 360;
        }
    }
}

void rgb_running_fill(uint8_t cycle_count, boolean colorful, uint8_t tail_length) {
    gHue = 0;
    for (uint8_t j = 0; j < cycle_count; j++) {
        for (uint8_t n = 0; n < NUM_LEDS; n++) {
            for (uint8_t i = 0; i < NUM_LEDS - n + tail_length; i++) {
                // Clear tail

                if (i < NUM_LEDS - n)
                    leds[i] = CHSV(gHue, 255, 200);

                for (uint8_t k = 0; k < tail_length; k++) {
                    if (i - k >= 0 && i < NUM_LEDS - n)
                        leds[i - k] = CHSV((gHue + k * 30) % 360, 255, 50 + (tail_length - k) * (150 / tail_length));
                }
                if (i - tail_length > 0) leds[i - tail_length - 1] = CHSV(0, 0, 0);

                if (i / 2 == 0 || colorful)
                    gHue = (gHue + 1) % 360;

                FastLED.show();
                delay(50);
            }
            FastLED.show();
            delay(500);
        }
    }
}

void sparkle(uint16_t cycle_count, CHSV color, boolean continuous) {
    uint8_t every_nth = 6;
    cycle_count = cycle_count * every_nth;

    for (uint16_t j = 0; j < cycle_count; j++) {
        if (!continuous) {
            FastLED.clear();
        } else {
            for (uint8_t i = 0; i < (NUM_LEDS / every_nth) * 2; i++) {
                leds[random(NUM_LEDS)] = CHSV(0, 0, 0);
            }
        }
        // Every n-th led
        for (uint8_t i = 0; i < NUM_LEDS / every_nth; i++) {
            if (color.hue == 0 && color.saturation == 0 && color.value == 0) {
                leds[random(NUM_LEDS)] = CHSV(random(360), 255, 200);
            } else {
                leds[random(NUM_LEDS)] = color;
            }
        }
        FastLED.show();
        if (continuous) {
            delay(1000 * 3 / every_nth);
        } else {
            delay(1000);
        }
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
        uint8_t pos = beatsin16(5, 50, 150); // generating the sinwave
        fill_solid(leds, NUM_LEDS, CHSV(gHue, 255, pos));
        FastLED.show();
        gHue = (gHue + 1) % 360;
        delay(100);
    }
}