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
  setBrightness(10);
  setAdjuster(0);
  activePattern = true;
  PatternHue patternHue = PatternHue();
  while (activePattern)
  {
    patternHue.variable_hue();
    loopEvents();
  }
}

void startRelax()
{
  setBrightness(20);
  setAdjuster(50);
  activePattern = true;
  PatternHue patternHue = PatternHue();
  while (activePattern)
  {
    patternHue.shifting_hue();
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

void startReveal()
{
  setBrightness(250);
  setAdjuster(0);
  activePattern = true;
  PatternHue patternHue = PatternHue();
  while (activePattern)
  {
    patternHue.white();
    loopEvents();
  }
}
