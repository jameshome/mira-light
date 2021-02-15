#include "system.h"
#include "patterns.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("mira is starting...");
  setupOTA("mira", wifi_ssid, wifi_password);
  TelnetPrint.begin();

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(dimmerInitState);

  preset_button.begin();
  preset_button.onPressed(preset_press);

  mode_button.begin();
  mode_button.onPressed(mode_press);

  ESP32Encoder::useInternalWeakPullResistors = UP;
  dimmer.attachSingleEdge(DIMMER_ENCODER_PIN_A, DIMMER_ENCODER_PIN_B);
  setBrightness(dimmerInitState);

  adjust.attachHalfQuad(ADJUST_ENCODER_PIN_A, ADJUST_ENCODER_PIN_B);
  setAdjuster(0);
}

void loop()
{
  switch (activeMode)
  {
  case 0:
    debugln("Off");
    startOff();
    break;
  case 1:
    debugln("Stargaze");
    startStargaze();
    break;
  case 2:
    debugln("Relax");
    startRelax();
    break;
  case 3:
    debugln("Party");
    startParty();
    break;
  case 4:
    debugln("Reveal");
    startReveal();
    break;
  }
  ArduinoOTA.handle();
}
