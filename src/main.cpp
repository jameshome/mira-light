#include "system.h"
#include "patterns.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("mira is starting...");
  setupOTA("mira", wifi_ssid, wifi_password);
  TelnetPrint.begin();

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(dimmerInit);

  preset_button.begin();
  preset_button.onPressed(preset_press);

  mode_button.begin();
  mode_button.onPressed(mode_press);

  ESP32Encoder::useInternalWeakPullResistors = UP;
  dimmer.attachSingleEdge(DIMMER_ENCODER_PIN_A, DIMMER_ENCODER_PIN_B);
  dimmer.setCount(dimmerInit);

  adjust.attachHalfQuad(ADJUST_ENCODER_PIN_A, ADJUST_ENCODER_PIN_B);
  adjust.setCount(0);
}

void loop()
{
  switch (activeMode)
  {
  case 0:
    TelnetPrint.println("Off");
    startOff();
    break;
  case 1:
    TelnetPrint.println("Stargaze");
    startStargaze();
    break;
  case 2:
    TelnetPrint.println("Relax");
    startRelax();
    break;
  case 3:
    TelnetPrint.println("Party");
    startParty();
    break;
  }
  ArduinoOTA.handle();
}
