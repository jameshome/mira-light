#include "pattern_rainbow.h"
#include "pattern_hue.h"

void startOff()
{
  activePattern = true;
  while (activePattern)
  {
    FastLED.clear();
    FastLED.show();
    loopEvents();
  }
}

void startStargaze()
{
  dimmer.setCount(10);
  adjust.setCount(0);
  activePattern = true;
  PatternHue patternHue = PatternHue();
  while (activePattern)
  {
    patternHue.pattern_hue();
    loopEvents();
  }
}

void startRelax()
{
  dimmer.setCount(20);
  adjust.setCount(50);
  activePattern = true;
  PatternHue patternHue = PatternHue();
  while (activePattern)
  {
    patternHue.pattern_hue_shift();
    loopEvents();
  }
}

void startParty()
{
  activePattern = true;
  PatternRainbow patternRainbow = PatternRainbow();
  while (activePattern)
  {
    patternRainbow.pattern_rainbow();
    loopEvents();
  }
}
