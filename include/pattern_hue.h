class PatternHue
{
public:
  PatternHue(){};
  void variable_hue();
  void shifting_hue();
  void white();

private:
  uint8_t hue = 0;
};

void PatternHue::variable_hue()
{
  fill_solid(leds, NUM_LEDS, CHSV(getAdjustment(), 255, 255));
  FastLED.show();
}

void PatternHue::shifting_hue()
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

void PatternHue::white()
{
  fill_solid(leds, NUM_LEDS, CHSV(getAdjustment(), 0, 255));
  FastLED.show();
}
