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

// Setup Panel inputs
#define OFF_BUTTON_PIN 15
#define STARGAZE_BUTTON_PIN 16
#define RELAX_BUTTON_PIN 17
#define PARTY_BUTTON_PIN 18
#define REVEAL_BUTTON_PIN 19

#define PREVIOUS_PATTERN_BUTTON_PIN 33
#define NEXT_PATTERN_BUTTON_PIN 25

#define DIMMER_ENCODER_PIN_A 13
#define DIMMER_ENCODER_PIN_B 14
#define ADJUST_ENCODER_PIN_A 26
#define ADJUST_ENCODER_PIN_B 27

// Setup LEDs
#define LED_A_PIN 21
#define LED_A_NUM_LEDS 2
#define LED_B_PIN 22
#define LED_B_NUM_LEDS 30
#define NUM_LEDS (LED_A_NUM_LEDS + LED_B_NUM_LEDS)
#define HALF_NUM_LEDS (NUM_LEDS / 2)

#define BRIGHTNESS_INIT 50
#define BRIGHTNESS_MAX 225

CRGB leds[NUM_LEDS];

// Setup logging
void debug(int i)
{
  Serial.print(i);
  TelnetPrint.print(i);
}
void debug(char const *c)
{
  Serial.print(F(c));
  TelnetPrint.print(F(c));
}
void debugln(int i)
{
  Serial.println(i);
  TelnetPrint.println(i);
}
void debugln(char const *c)
{
  Serial.println(F(c));
  TelnetPrint.println(F(c));
}

// Setup dimmer
ESP32Encoder dimmer;
uint8_t brightness = BRIGHTNESS_INIT;
uint8_t lastDimmerState = 0;

void setBrightness(uint8_t level)
{
  if (level < 5)
  {
    level = 5;
  }
  else if (level > BRIGHTNESS_MAX)
  {
    level = (BRIGHTNESS_MAX - 5);
  }
  brightness = level;
  dimmer.setCount(level);
  FastLED.setBrightness(level);
  debug("Brightness set to ");
  debugln(level);
}

void readDimmer()
{
  uint8_t dimmerState = dimmer.getCount();

  if (dimmerState != lastDimmerState)
  {
    // increase dimmer increment when above initial brightness
    if (dimmerState > BRIGHTNESS_INIT)
    {
      if (dimmerState < lastDimmerState)
      {
        dimmerState = (dimmerState - 10);
      }
      else
      {
        dimmerState = (dimmerState + 10);
      }
    }
    setBrightness(dimmerState);
    lastDimmerState = dimmerState;
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

  if (adjustState != lastAdjustState)
  {
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
EasyButton reveal_button(REVEAL_BUTTON_PIN);
EasyButton previous_pattern_button(PREVIOUS_PATTERN_BUTTON_PIN);
EasyButton next_pattern_button(NEXT_PATTERN_BUTTON_PIN);

#endif
