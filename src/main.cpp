#include "system.h"
#include "network.h"
#include "pattern.h"
#include "animation.h"
#include "webserver.h"

void update()
{
  updateBrightness();
  off_button.read();
  if (off_button.wasPressed())
  {
    selectPattern(0);
  }
  stargaze_button.read();
  if (stargaze_button.wasPressed())
  {
    selectPattern(1);
  }
  relax_button.read();
  if (relax_button.wasPressed())
  {
    selectPattern(2);
  }
  party_button.read();
  if (party_button.wasPressed())
  {
    selectPattern(defaultParty);
  }
  reveal_button.read();
  if (reveal_button.wasPressed())
  {
    selectPattern(3);
  }
  if (activePattern > 3)
  {
    previous_pattern_button.read();
    next_pattern_button.read();
  }
  ArduinoOTA.handle();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("MIRA LIVES");
  setupNetwork("mira", wifi_ssid, wifi_password);
  TelnetPrint.begin();

  FastLED.addLeds<NEOPIXEL, LED_A_PIN>(leds, 0, LED_A_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_B_PIN>(leds, LED_A_NUM_LEDS, LED_B_NUM_LEDS);
  FastLED.setBrightness(dimmerInitState);

  previous_pattern_button.begin();
  previous_pattern_button.onPressed(previousPattern);

  next_pattern_button.begin();
  next_pattern_button.onPressed(nextPattern);

  off_button.begin();
  if (digitalRead(OFF_BUTTON_PIN) == LOW)
  {
    selectPattern(0);
  }

  stargaze_button.begin();
  if (digitalRead(STARGAZE_BUTTON_PIN) == LOW)
  {
    selectPattern(1);
  }

  relax_button.begin();
  if (digitalRead(RELAX_BUTTON_PIN) == LOW)
  {
    selectPattern(2);
  }

  party_button.begin();
  if (digitalRead(PARTY_BUTTON_PIN) == LOW)
  {
    selectPattern(defaultParty);
  }

  reveal_button.begin();
  if (digitalRead(REVEAL_BUTTON_PIN) == LOW)
  {
    selectPattern(3);
  }

  dimmer.attachSingleEdge(DIMMER_ENCODER_PIN_A, DIMMER_ENCODER_PIN_B);

  adjust.attachHalfQuad(ADJUST_ENCODER_PIN_A, ADJUST_ENCODER_PIN_B);
  startWebserver();
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
      update();
    }
    break;
  case 1: // Stargaze
    a.settings(20);
    while (patternRunning)
    {
      a.animationSolid();
      update();
    }
    break;
  case 2: // Relax
    a.settings(20, 10);
    while (patternRunning)
    {
      a.animationShiftingHue();
      update();
    }
    break;
  case 3: // Reveal
    a.settings(150);
    while (patternRunning)
    {
      a.animationSolid(0);
      update();
    }
    break;
  case 4: // Rainbow
    a.settings(20, 10);
    while (patternRunning)
    {
      a.animationRainbow();
      update();
    }
    break;
  case 5: // Sinelon
    a.settings(100);
    while (patternRunning)
    {
      a.animationSinelon();
      update();
    }
    break;
  case 6: // Pacifica
    a.settings(100);
    while (patternRunning)
    {
      a.animationPacifica();
      update();
    }
    break;
  }
  ArduinoOTA.handle();
}
