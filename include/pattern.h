class Pattern
{
public:
  const char *name;
  uint8_t brightness;
  uint8_t hue;
  uint8_t saturation;
  uint8_t speed;
  Pattern(
      char const *patternName,
      uint8_t patternBrightness,
      uint8_t patternHue,
      uint8_t patternSaturation,
      uint8_t patternSpeed);
};

Pattern::Pattern(
    char const *patternName,
    uint8_t patternBrightness = BRIGHTNESS_INIT,
    uint8_t patternHue = 0,
    uint8_t patternSaturation = 255,
    uint8_t patternSpeed = 0)
{
  name = patternName;
  brightness = patternBrightness;
  hue = patternHue;
  saturation = patternSaturation;
  speed = patternSpeed;
}

// name, brightness, hue, saturation, speed
Pattern patterns[] = {
    Pattern("Off", 0),
    Pattern("Stargaze", 20, 0),
    Pattern("Relax", 60, 18, 255),
    Pattern("Reveal", 150, 255, 0),
    Pattern("Pacifica", 100),
    Pattern("Rainbow", 20, 0, 0, 8),
    Pattern("Sinelon", 100),
    Pattern("Fire 2012", 50),
    Pattern("Shifting Hue", 40)};

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
