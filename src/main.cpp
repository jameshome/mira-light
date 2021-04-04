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
  reveal_button.read();
  if (reveal_button.wasPressed())
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

  setupNetwork("mira", wifi_ssid, wifi_password);

  TelnetPrint.begin();

  FastLED.addLeds<NEOPIXEL, LED_A_PIN>(leds, 0, LED_A_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_B_PIN>(leds, LED_A_NUM_LEDS, LED_B_NUM_LEDS);

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

  reveal_button.begin();
  if (digitalRead(REVEAL_BUTTON_PIN) == LOW)
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
  current.hue = patterns[activePattern].hue;
  current.saturation = patterns[activePattern].saturation;
  current.speed = patterns[activePattern].speed;

  FastLED.clear();

  while (patternRunning)
  {
    a.select(patterns[activePattern].name);
    update();
  }
}
