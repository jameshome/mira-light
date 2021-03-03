class Animation
{
public:
  Animation();
  ~Animation();
  void settings(uint8_t brightness = dimmerInitState, uint8_t adjustment = 0);
  void update();
  void mirror();
  void animationSolid(uint8_t saturation = 255);
  void animationShiftingHue();
  void animationRainbow();
  void animationSinelon();

private:
  uint8_t hue = 0;
};

Animation::Animation()
{
  debugln("Animation created");
  FastLED.clear();
  patternRunning = true;
}

Animation::~Animation(void)
{
  debugln("Animation destroyed");
  debugln("—————————————————————");
}

void Animation::settings(uint8_t brightness, uint8_t adjustment)
{
  setBrightness(brightness);
  setAdjuster(adjustment);
}

void Animation::update()
{
  changeBrightness();
  readPatternButtons();
  ArduinoOTA.handle();
}

void Animation::mirror()
{
  for (uint8_t i = 0; i < HALF_NUM_LEDS; i++)
  {
    leds[NUM_LEDS - 1 - i] = leds[i];
  }
}

void Animation::animationSolid(uint8_t saturation)
{
  fill_solid(leds, NUM_LEDS, CHSV(getAdjustment(), saturation, 255));
  FastLED.show();
}

void Animation::animationShiftingHue()
{
  int speed = getAdjustment();
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(hue, 200, 255);
  }
  EVERY_N_MILLIS_I(hueShift, speed)
  {
    hueShift.setPeriod(speed);
    FastLED.show();
    hue++;
  }
}

void Animation::animationRainbow()
{
  uint8_t delta = 200;
  uint8_t density = 1;
  fill_rainbow(leds, NUM_LEDS, hue += density, delta);
  FastLED.show();
}

void Animation::animationSinelon()
{
  fadeToBlackBy(leds, HALF_NUM_LEDS, 25);
  int pos = beatsin16(10, 0, HALF_NUM_LEDS);
  leds[pos] += CHSV(getAdjustment(), 255, 192);
  mirror();
  FastLED.show();
}
