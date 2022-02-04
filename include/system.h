#ifndef SYSTEM_H
#define SYSTEM_H

#include <EasyButton.h>
#include <ESP32Encoder.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <secrets.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>
#include <aWOT.h>
#include <TelnetPrint.h>
#include <Filter.h>

// Setup Panel inputs
#define OFF_BUTTON_PIN 19
#define STARGAZE_BUTTON_PIN 18
#define RELAX_BUTTON_PIN 5
#define PARTY_BUTTON_PIN 17
#define DAZE_BUTTON_PIN 16
#define PREVIOUS_PATTERN_BUTTON_PIN 26
#define NEXT_PATTERN_BUTTON_PIN 32
#define ADJUST_ENCODER_PIN_A 25
#define ADJUST_ENCODER_PIN_B 33
#define DIMMER_ENCODER_PIN_A 14
#define DIMMER_ENCODER_PIN_B 27
#define BRIGHTNESS_INIT 50
#define BRIGHTNESS_MIN 10
#define BRIGHTNESS_MAX 240

// Setup LEDs
#define CHANNEL_1A_PIN 21
#define CHANNEL_1B_PIN 22
#define CHANNEL_2_PIN 23
#define CHANNEL_1A_NUM_LEDS 91
#define CHANNEL_1B_NUM_LEDS 51
#define CHANNEL_1_NUM_LEDS (CHANNEL_1A_NUM_LEDS + CHANNEL_1B_NUM_LEDS)
#define CHANNEL_2_NUM_LEDS 58
#define NUM_LEDS (CHANNEL_1A_NUM_LEDS + CHANNEL_1B_NUM_LEDS + CHANNEL_2_NUM_LEDS)
#define HALF_NUM_LEDS (NUM_LEDS / 2)
CRGB leds[NUM_LEDS];

// Setup audio detection
#define MIC_PIN 34
#define NOISE 200
ExponentialFilter<long> ADCFilter(5,0);

// Setup logging
void debug(int i) {
  Serial.print(i);
  TelnetPrint.print(i);
}
void debug(char const *c) {
  Serial.print(F(c));
  TelnetPrint.print(F(c));
}
void debugln(int i) {
  Serial.println(i);
  TelnetPrint.println(i);
}
void debugln(char const *c) {
  Serial.println(F(c));
  TelnetPrint.println(F(c));
}

// Setup dimmer
ESP32Encoder dimmer;
uint8_t brightness = 0;
uint8_t targetBrightness = 0;
int16_t lastDimmerValue = 0;

void readDimmer()
{
  int16_t dimmerValue = dimmer.getCount();
  uint8_t dimmerStep = 2;
  if (brightness > 50) {
    dimmerStep = 5;
  } else if (brightness > 100) {
    dimmerStep = 10;
  } else if (brightness > 180) {
    dimmerStep = 20;
  }

  if (dimmerValue != lastDimmerValue) {
    if (dimmerValue > lastDimmerValue) {
      targetBrightness = constrain((targetBrightness + dimmerStep), BRIGHTNESS_MIN, BRIGHTNESS_MAX);
    } else {
      targetBrightness = constrain((targetBrightness - dimmerStep), BRIGHTNESS_MIN, BRIGHTNESS_MAX);
    }
    lastDimmerValue = dimmerValue;
  }
}

void setBrightness(uint8_t level)
{
  level = constrain(level, BRIGHTNESS_MIN, BRIGHTNESS_MAX);
  brightness = level;
  targetBrightness = level;
  FastLED.setBrightness(level);
}

void adjustBrightness()
{
  if (targetBrightness > brightness) {
    brightness++;
    FastLED.setBrightness(brightness);
  } else if (targetBrightness < brightness) {
    brightness--;
    FastLED.setBrightness(brightness);
  }
}

// Setup adjuster
ESP32Encoder adjust;
uint8_t lastAdjustState = 0;
void setAdjuster(uint8_t level)
{
  adjust.setCount(level);
}
uint8_t readAdjuster()
{
  uint8_t adjustState = adjust.getCount();

  if (adjustState != lastAdjustState) {
    lastAdjustState = adjustState;
    debug("Adjuster set to ");
    debugln(adjustState);
  }
  return adjustState;
}

// Setup buttons
EasyButton off_button(OFF_BUTTON_PIN);
EasyButton stargaze_button(STARGAZE_BUTTON_PIN);
EasyButton relax_button(RELAX_BUTTON_PIN);
EasyButton party_button(PARTY_BUTTON_PIN);
EasyButton daze_button(DAZE_BUTTON_PIN);
EasyButton previous_pattern_button(PREVIOUS_PATTERN_BUTTON_PIN);
EasyButton next_pattern_button(NEXT_PATTERN_BUTTON_PIN);

#endif
