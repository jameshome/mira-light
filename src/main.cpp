#include "system.h"
#include "network.h"
#include "pattern.h"
#include "palette.h"
#include "animation.h"
#include "webserver.h"


// UPDATE runs after each Animation's while loop
void update()
{
  // Read dimmer changes
  readDimmer();
  adjustBrightness();

  // Read physical button changes
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
  daze_button.read();
  if (daze_button.wasPressed())
  {
    selectPattern(3);
  }
  party_button.read();
  if (party_button.wasPressed())
  {
    selectPattern(4);
  }

  if (activePattern > 3)
  {
    previous_pattern_button.read();
    next_pattern_button.read();
  }

  // Check for an OTA update
  ArduinoOTA.handle();

  // Check for web requests
  WiFiClient client = server.available();
  if (client.connected())
  {
    app.process(&client);
    client.stop();
  }
}

// SETUP runs once at boot
void setup()
{
  Serial.begin(115200);
  Serial.println("MIRA LIVES");

  setupNetwork("mira-light", wifi_ssid, wifi_password);

  TelnetPrint.begin();

  unsigned int ledOffset = 0;

  FastLED.addLeds<NEOPIXEL, CHANNEL_1A_PIN>(leds, ledOffset, CHANNEL_1_NUM_LEDS);
  ledOffset = ledOffset + CHANNEL_1A_NUM_LEDS; 
  FastLED.addLeds<NEOPIXEL, CHANNEL_1B_PIN>(leds, ledOffset, CHANNEL_1B_NUM_LEDS);
  ledOffset = ledOffset + CHANNEL_1B_NUM_LEDS; 
  FastLED.addLeds<NEOPIXEL, CHANNEL_2_PIN>(leds, ledOffset, CHANNEL_2_NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS_INIT);

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

  daze_button.begin();
  if (digitalRead(DAZE_BUTTON_PIN) == LOW)
  {
    selectPattern(3);
  }

  party_button.begin();
  if (digitalRead(PARTY_BUTTON_PIN) == LOW)
  {
    selectPattern(4);
  }

  dimmer.attachSingleEdge(DIMMER_ENCODER_PIN_A, DIMMER_ENCODER_PIN_B);

  adjust.attachHalfQuad(ADJUST_ENCODER_PIN_A, ADJUST_ENCODER_PIN_B);

  // Start web server
  app.get("/api/patterns", &readPatterns);
  app.get("/api/animation", &readAnimation);
  app.put("/api/animation", &updateAnimation);
  app.use(staticFiles());
  server.begin();
}



// LOOP runs once whenever a new Animation begins
void loop()
{
  Animation a = Animation();
  debug("Pattern set to ");
  debugln(patterns[activePattern].name);
  setBrightness(patterns[activePattern].brightness);
  setAdjuster(patterns[activePattern].adjustment);
  current.hue = patterns[activePattern].hue;
  current.saturation = patterns[activePattern].saturation;
  FastLED.clear();
  while (patternRunning)
  {
    a.select(patterns[activePattern].name);
    update();
  }
}
