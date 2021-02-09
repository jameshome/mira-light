class PatternRainbow
{
public:
  PatternRainbow(){};
  void pattern_rainbow();
  void pattern_rainbow_shift();

private:
  uint8_t hue = 0;
  uint8_t delta = 200;
  uint8_t density = 1;
};

void PatternRainbow::pattern_rainbow()
{
  fill_rainbow(leds, NUM_LEDS, hue += density, delta);
  FastLED.show();
}
