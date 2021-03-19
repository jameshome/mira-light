class Animation
{
public:
  Animation();
  ~Animation();
  void selector(char const *pattern);
  void brightness(uint8_t brightness = dimmerInitState);
  void adjustment(uint8_t adjustment = 0);

private:
  uint8_t hue = 0;
  void mirror();
  void animationSolid(uint8_t saturation = 255);
  void animationShiftingHue();
  void animationRainbow();
  void animationSinelon();
  void animationPacifica();
  void animationFire2012();
  void pacifica_one_layer(CRGBPalette16 &p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff);
  void pacifica_add_whitecaps();
  void pacifica_deepen_colors();
};

Animation::Animation()
{
  debugln("Animation created");
  FastLED.clear();
  patternRunning = true;
}

Animation::~Animation(void)
{
  debugln("Animation destroyed");
  debugln("—————————————————————");
}

void Animation::selector(char const *pattern)
{
  if (strcmp(pattern, "Stargaze") == 0)
  {
    animationSolid();
  }
  else if (strcmp(pattern, "Relax") == 0)
  {
    animationShiftingHue();
  }
  else if (strcmp(pattern, "Reveal") == 0)
  {
    animationSolid(0);
  }
  else if (strcmp(pattern, "Pacifica") == 0)
  {
    animationPacifica();
  }
  else if (strcmp(pattern, "Rainbow") == 0)
  {
    animationRainbow();
  }
  else if (strcmp(pattern, "Sinelon") == 0)
  {
    animationSinelon();
  }
  else if (strcmp(pattern, "Fire 2012") == 0)
  {
    animationFire2012();
  }
}

void Animation::brightness(uint8_t brightness)
{
  setBrightness(brightness);
}

void Animation::adjustment(uint8_t adjustment)
{
  setAdjuster(adjustment);
}

void Animation::mirror()
{
  for (uint8_t i = 0; i < HALF_NUM_LEDS; i++)
  {
    leds[NUM_LEDS - 1 - i] = leds[i];
  }
}

// ——————————————————————————————————————————————
// Solid
void Animation::animationSolid(uint8_t saturation)
{
  fill_solid(leds, NUM_LEDS, CHSV(getAdjustment(), saturation, 255));
  FastLED.show();
}

// ——————————————————————————————————————————————
// Shifting Hue
void Animation::animationShiftingHue()
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

// ——————————————————————————————————————————————
// Rainbow
void Animation::animationRainbow()
{
  uint8_t delta = 200;
  uint8_t density = 1;
  fill_rainbow(leds, NUM_LEDS, hue += density, delta);
  FastLED.show();
}

// ——————————————————————————————————————————————
// Sinelon
void Animation::animationSinelon()
{
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS);
  static int prevpos = 0;
  if (pos < prevpos)
  {
    fill_solid(leds + pos, (prevpos - pos) + 1, CHSV(getAdjustment(), 255, 255));
  }
  else
  {
    fill_solid(leds + prevpos, (pos - prevpos) + 1, CHSV(getAdjustment(), 255, 255));
  }
  prevpos = pos;
  FastLED.show();
}

// ——————————————————————————————————————————————
// Pacifica
// by Mark Kriegsman and Mary Corey March
// for Dan Garcia
void Animation::animationPacifica()
{
  // Increment the four "color index start" counters,one for each wave layer. Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011, 10, 13));
  sCIStart2 -= (deltams21 * beatsin88(777, 8, 11));
  sCIStart3 -= (deltams1 * beatsin88(501, 5, 7));
  sCIStart4 -= (deltams2 * beatsin88(257, 4, 6));

  // Clear out the LED array to a dim background blue-green
  fill_solid(leds, NUM_LEDS, CRGB(2, 6, 10));

  // Render each of four 'waves', with different scales and speeds that vary over time
  Animation::pacifica_one_layer(palette_pacifica_1, sCIStart1, beatsin16(3, 11 * 256, 14 * 256), beatsin8(10, 70, 130), 0 - beat16(301));
  Animation::pacifica_one_layer(palette_pacifica_2, sCIStart2, beatsin16(4, 6 * 256, 9 * 256), beatsin8(17, 40, 80), beat16(401));
  Animation::pacifica_one_layer(palette_pacifica_3, sCIStart3, 6 * 256, beatsin8(9, 10, 38), 0 - beat16(503));
  Animation::pacifica_one_layer(palette_pacifica_3, sCIStart4, 5 * 256, beatsin8(8, 10, 28), beat16(601));

  // Add brighter 'whitecaps' where the four layers of 'waves' have lined up brightly
  Animation::pacifica_add_whitecaps();

  // Deepen the blues and greens a bit
  Animation::pacifica_deepen_colors();

  FastLED.show();
}

// Add one layer of waves into the led array
void Animation::pacifica_one_layer(CRGBPalette16 &p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    waveangle += 250;
    uint16_t s16 = sin16(waveangle) + 32768;
    uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16(ci) + 32768;
    uint8_t sindex8 = scale16(sindex16, 240);
    CRGB c = ColorFromPalette(p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}

void Animation::pacifica_add_whitecaps()
{
  uint8_t basethreshold = beatsin8(9, 55, 65);
  uint8_t wave = beat8(7);

  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    uint8_t threshold = scale8(sin8(wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if (l > threshold)
    {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8(overage, overage);
      leds[i] += CRGB(overage, overage2, qadd8(overage2, overage2));
    }
  }
}

void Animation::pacifica_deepen_colors()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i].blue = scale8(leds[i].blue, 145);
    leds[i].green = scale8(leds[i].green, 200);
    leds[i] |= CRGB(2, 5, 7);
  }
}

// ——————————————————————————————————————————————
// Fire 2012
// by Mark Kriegsman
void Animation::animationFire2012()
{
  bool reverse = false;
  uint8_t cooling = 55;
  uint8_t sparking = 120;
  CRGBPalette16 palette = HeatColors_p;

  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Cool down every cell a little
  for (int i = 0; i < NUM_LEDS; i++)
  {
    heat[i] = qsub8(heat[i], random8(0, ((cooling * 10) / NUM_LEDS) + 2));
  }

  // Heat from each cell drifts 'up' and diffuses a little
  for (int k = NUM_LEDS - 1; k >= 2; k--)
  {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Randomly ignite new 'sparks' of heat near the bottom
  if (random8() < sparking)
  {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255));
  }

  // Map from heat cells to LED colors
  for (int j = 0; j < NUM_LEDS; j++)
  {
    byte colorindex = scale8(heat[j], 240);
    CRGB color = ColorFromPalette(palette, colorindex);
    int pixelnumber;
    if (reverse)
    {
      pixelnumber = (NUM_LEDS - 1) - j;
    }
    else
    {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }

  FastLED.show();
  FastLED.delay(1000 / 60);
}