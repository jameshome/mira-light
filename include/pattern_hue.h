class PatternHue
{
public:
  PatternHue(){};
  void pattern_hue();
  void pattern_hue_shift();

private:
  uint8_t hue = 0;
};

void PatternHue::pattern_hue()
{
  fill_solid(leds, NUM_LEDS, CHSV(getAdjustment(), 255, 255));
  FastLED.show();
}

void PatternHue::pattern_hue_shift()
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