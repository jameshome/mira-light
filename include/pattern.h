class Pattern
{
public:
  const char *name;
  uint8_t brightness;
  uint8_t adjustment;
  uint8_t hue;
  uint8_t saturation;
  Pattern(
      char const *patternName,
      uint8_t patternBrightness,
      uint8_t patternAdjustment,
      uint8_t patternHue,
      uint8_t patternSaturation);
};

Pattern::Pattern(
    char const *patternName,
    uint8_t patternBrightness = BRIGHTNESS_INIT,
    uint8_t patternAdjustment = 0,
    uint8_t patternHue = 0,
    uint8_t patternSaturation = 255)
{
  name = patternName;
  brightness = patternBrightness;
  adjustment = patternAdjustment;
  hue = patternHue;
  saturation = patternSaturation;
}

// name, brightness, adjustment, hue, saturation
Pattern patterns[] = {
    Pattern("Off", 0),
    Pattern("Stargaze", 30, 200, 200, 180),
    Pattern("Relax", 60, 18, 18),
    Pattern("Daze", 200, 255, 255, 0),
    Pattern("Pacifica", 100),
    Pattern("Rainbow", 20, 0),
    Pattern("Sinelon", 100, 0),
    Pattern("Fire 2012", 50),
    Pattern("Shifting Hue", 40),
    Pattern("VU Meter", 40)};

const uint16_t patternCount = (sizeof(patterns) / sizeof(*patterns));
int activePattern = 0;
bool patternRunning = false;

void selectPattern(int pattern)
{
  patternRunning = false;
  activePattern = pattern;
}

void browsePatterns(int delta)
{
  uint16_t min = 4;
  uint16_t max = patternCount - 1;
  const uint16_t mod = max + 1 - min;
  activePattern += delta - min;
  activePattern += (1 - activePattern / mod) * mod;
  selectPattern(activePattern % mod + min);
}

void previousPattern()
{
  browsePatterns(-1);
}

void nextPattern()
{
  browsePatterns(1);
}
