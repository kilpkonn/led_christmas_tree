#include "FastLED.h"

#define LED_PIN 7
#define NUM_LEDS 60
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB


CRGB leds[NUM_LEDS];
uint8_t layers[5] = {5, 8, 12, 16, 19};

void setup() {
    LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}

uint8_t gHue = 0;

void rgb_running_fill(uint8_t cycle_count, bool colorful, uint8_t tail_length);

void sparkle(uint16_t cycle_count, const CHSV &color, bool continuous);

void rgb_spiral_fill(uint8_t cycle_count, uint8_t colors_count, uint8_t splits_count);

void rgb_running_fade(uint16_t cycle_time);

void rgb_fade(uint16_t cycle_time);

void spiral_tail(uint16_t cycle_time, const CHSV &color);

void spiral_falloff(uint16_t cycle_time);

void shiny_peak(uint8_t cycle_count);

void reset();

void loop() {
    //spiral_tail(30, CHSV(300, 255, 200));
    rgb_running_fade(60);
    shiny_peak(20);
    spiral_falloff(60);
    rgb_running_fill(3, false, 2);
    sparkle(60, CHSV(0, 0, 0), true);
    rgb_spiral_fill(60, 6, 6);
    rgb_running_fade(60);
    sparkle(60, CHSV(random(255), 100 + random(155), 200), false);
    rgb_spiral_fill(60, 6, 2);
    rgb_fade(60);
    rgb_running_fill(3, true, 0);
    rgb_spiral_fill(60, 6, 1);
}


void reset() {
    gHue = 0;
    for (uint8_t i = NUM_LEDS; i > 0 ; --i) {
        leds[i] = CHSV(0, 0, 0);
        FastLED.show();
        delay(40);
    }
}

void shiny_peak(uint8_t cycle_count) {
    reset();
    for (uint8_t i = 0; i < cycle_count; i++) {
        for (uint8_t j = 0; j < NUM_LEDS; j++) {
            leds[j] = CHSV(gHue, 255, 20 + j * (235 / NUM_LEDS));
            FastLED.show();
            delay(20);
        }
        delay(100);
        for (uint8_t j = NUM_LEDS; j > 0; --j) {
            leds[j] = CHSV(0, 0, 0);
            FastLED.show();
            delay(20);
        }
        gHue += 50;
    }
}

void spiral_falloff(uint16_t cycle_time) {
    reset();
    cycle_time = cycle_time * 5; // seconds
    for (uint16_t i = 0; i < cycle_time; i++) {
        if (i % 15 < 3 + random(3)) {
            leds[NUM_LEDS - 1] = CHSV(gHue, 255, 200);
        } else {
            leds[NUM_LEDS - 1] = CHSV(0, 0, 0);
        }

        for (uint8_t j = 0; j < NUM_LEDS - 1; j++) {
            leds[j] = leds[j + 1];
        }
        FastLED.show();
        delay(200);
        gHue += 10;
    }
}

void spiral_tail(uint16_t cycle_time, const CHSV &color) {
    reset();
    cycle_time = cycle_time * 10; // seconds
    for (uint16_t i = 0; i < cycle_time; i++) {
        if (random(4) < 2) {
            leds[NUM_LEDS - random(layers[0]) - 1] = color;
            FastLED.show();
            delay(100);
        }
        uint8_t leds_before = 1;
        for (uint8_t layer : layers) {
            for (uint8_t k = 0; k < layer; k++) {
                if (NUM_LEDS - leds_before - k < 0) continue;

                /*CRGB led = leds[NUM_LEDS - leds_before - k];
                if (led.red != 0 && led.green != 0 && led.blue != 0) {
                    uint8_t random_number = NUM_LEDS - leds_before - k - layer; // - random(3);
                    if (random_number >= 0)
                        leds[random_number] = led;
                } else {
                    for (uint8_t l = 0; l < 3; l++) {
                        if (NUM_LEDS - leds_before - k - layer - l >= 0)
                            leds[NUM_LEDS - leds_before - k - layer - l] = CHSV(0, 0, 0);
                    }
                }*/
                FastLED.show();
                delay(100);
            }
            leds_before += layer;
        }
    }
}

void rgb_spiral_fill(uint8_t cycle_count, uint8_t colors_count, uint8_t splits_count) {
    reset();
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
            gHue += 255 / colors_count;
        }
    }
}

void rgb_running_fill(uint8_t cycle_count, bool colorful, uint8_t tail_length) {
    reset();
    for (uint8_t j = 0; j < cycle_count; j++) {
        for (uint8_t n = 0; n < NUM_LEDS; n++) {
            for (uint8_t i = 0; i < (uint8_t) (NUM_LEDS - n + tail_length); i++) {
                // Clear tail

                if (i < NUM_LEDS - n)
                    leds[i] = CHSV(gHue, 255, 200);

                for (uint8_t k = 0; k < tail_length; k++) {
                    if (i - k >= 0 && i < NUM_LEDS - n)
                        leds[i - k] = CHSV((gHue + k * 30) % 360, 255, 50 + (tail_length - k) * (150 / tail_length));
                }
                if (i - tail_length > 0) leds[i - tail_length - 1] = CHSV(0, 0, 0);

                if (i / 2 == 0 || colorful)
                    gHue++;

                FastLED.show();
                delay(50);
            }
            FastLED.show();
            delay(500);
        }
    }
}

void sparkle(uint16_t cycle_count, const CHSV &color, bool continuous) {
    reset();
    uint8_t every_nth = 6;
    cycle_count = cycle_count * every_nth;

    for (uint16_t j = 0; j < cycle_count; j++) {
        if (!continuous) {
            FastLED.clear();
        } else {
            for (uint8_t i = 0; i < (uint8_t) (NUM_LEDS / every_nth) * 2; i++) {
                leds[random(NUM_LEDS)] = CHSV(0, 0, 0);
            }
        }
        // Every n-th led
        for (uint8_t i = 0; i < NUM_LEDS / every_nth; i++) {
            if (color.hue == 0 && color.saturation == 0 && color.value == 0) {
                leds[random(NUM_LEDS)] = CHSV(random(255), 255, 200);
            } else {
                leds[random(NUM_LEDS)] = color;
            }
        }
        FastLED.show();
        if (continuous) {
            delay(1000 * 5 / every_nth);
        } else {
            delay(1000);
        }
    }
}

void rgb_running_fade(uint16_t cycle_time) {
    reset();
    cycle_time = cycle_time * 50;  // convert to seconds
    for (uint16_t i = 0; i < cycle_time; i++) {
        fill_rainbow(leds, NUM_LEDS, gHue, 255 / NUM_LEDS);
        FastLED.show();
        gHue++;
        delay(20);
    }
}

void rgb_fade(uint16_t cycle_time) {
    reset();
    cycle_time = cycle_time * 10;  // convert to seconds
    for (uint16_t i = 0; i < cycle_time; i++) {
        uint8_t pos = beatsin16(5, 50, 150); // generating the sinwave
        fill_solid(leds, NUM_LEDS, CHSV(gHue, 255, pos));
        FastLED.show();
        gHue++;
        delay(100);
    }
}