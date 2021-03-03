#include "system.h"
#include "pattern.h"
#include "animation.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("mira is starting...");
  setupOTA("mira", wifi_ssid, wifi_password);
  TelnetPrint.begin();

  FastLED.addLeds<NEOPIXEL, LED_A_PIN>(leds, 0, LED_A_NUM_LEDS);

  FastLED.addLeds<NEOPIXEL, LED_B_PIN>(leds, LED_A_NUM_LEDS, LED_B_NUM_LEDS);

  FastLED.setBrightness(dimmerInitState);

  previous_pattern_button.begin();
  previous_pattern_button.onPressed(previousPattern);

  next_pattern_button.begin();
  next_pattern_button.onPressed(nextPattern);

  dimmer.attachSingleEdge(DIMMER_ENCODER_PIN_A, DIMMER_ENCODER_PIN_B);

  adjust.attachHalfQuad(ADJUST_ENCODER_PIN_A, ADJUST_ENCODER_PIN_B);
}

void loop()
{

  Animation a = Animation();

  debug("Pattern set to ");
  debugln(patterns[activePattern].name);

  switch (activePattern)
  {
  case 0: // Off
    while (patternRunning)
    {
      FastLED.clear();
      FastLED.show();
      a.update();
    }
    break;
  case 1: // Stargaze
    a.settings(20);
    while (patternRunning)
    {
      a.animationSolid();
      a.update();
    }
    break;
  case 2: // Relax
    a.settings(20, 10);
    while (patternRunning)
    {
      a.animationShiftingHue();
      a.update();
    }
    break;
  case 3: // Reveal
    a.settings(150);
    while (patternRunning)
    {
      a.animationSolid(0);
      a.update();
    }
    break;
  case 4: // Rainbow
    a.settings(20, 10);
    while (patternRunning)
    {
      a.animationRainbow();
      a.update();
    }
    break;
  case 5: // Sinelon
    a.settings(100);
    while (patternRunning)
    {
      a.animationSinelon();
      a.update();
    }
    break;
  }
  ArduinoOTA.handle();
}
