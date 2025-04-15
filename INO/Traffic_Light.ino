#include <FastLED.h>

#define NUM_LEDS 3
#define PIN 14

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds <WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(50);
}

void loop() {
  leds[2] = CRGB::Red;
  FastLED.show();
  delay(15000);
  leds[2] = CRGB( 0, 0, 0);
  FastLED.show();

  leds[1] = CRGB::Yellow;
  FastLED.show();
  delay(2000);
  leds[1] = CRGB( 0, 0, 0);
  FastLED.show();

  leds[0] = CRGB::Green;
  FastLED.show();
  delay(15000);
  leds[0] = CRGB( 0, 0, 0);
  FastLED.show();

  leds[1] = CRGB::Yellow;
  FastLED.show();
  delay(2000);
  leds[1] = CRGB( 0, 0, 0);
  FastLED.show();
}