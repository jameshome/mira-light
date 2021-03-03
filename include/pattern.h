class Pattern
{
public:
  int id;
  const char *name;
  Pattern(int patternId, char const *patternName);
};

Pattern::Pattern(int patternId, char const *patternName)
{
  id = patternId;
  name = patternName;
}

Pattern patterns[] = {
    Pattern(0, "Off"),
    Pattern(1, "Stargaze"),
    Pattern(2, "Relax"),
    Pattern(3, "Reveal"),
    Pattern(4, "Rainbow"),
    Pattern(5, "Sinelon")};

int activePattern = 0;
int patternCount = (sizeof(patterns) / sizeof(*patterns));

bool patternRunning = false;

void selectPattern(int pattern)
{
  patternRunning = false;
  activePattern = pattern;
}

void readPatternButtons()
{
  previous_pattern_button.read();
  next_pattern_button.read();
}

void browsePatterns(int delta)
{
  int min = 0;
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
