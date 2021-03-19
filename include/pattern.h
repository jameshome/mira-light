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
    Pattern(4, "Pacifica"),
    Pattern(5, "Rainbow"),
    Pattern(6, "Sinelon"),
    Pattern(7, "Fire 2012")};

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
