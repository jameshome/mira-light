class Pattern
{
public:
  const char *name;
  uint8_t brightness;
  uint8_t adjustment;
  Pattern(char const *patternName, uint8_t patternBrightness, uint8_t patternAdjustment);
};

Pattern::Pattern(char const *patternName, uint8_t patternBrightness = dimmerInitState, uint8_t patternAdjustment = 0)
{
  name = patternName;
  brightness = patternBrightness;
  adjustment = patternAdjustment;
}

Pattern patterns[] = {
    Pattern("Off"),
    Pattern("Stargaze", 20),
    Pattern("Relax", 20, 10),
    Pattern("Reveal", 100),
    Pattern("Pacifica", 100),
    Pattern("Rainbow", 20, 10),
    Pattern("Sinelon", 100),
    Pattern("Fire 2012", 50)};

const int patternCount = (sizeof(patterns) / sizeof(*patterns));

int activePattern = 0;
int defaultParty = 4;

bool patternRunning = false;

void selectPattern(int pattern)
{
  patternRunning = false;
  activePattern = pattern;
}

void browsePatterns(int delta)
{
  int min = 4;
  int max = patternCount - 1;
  const int mod = max + 1 - min;
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
