class Mode
{
public:
  int id;
  const char *name;
  Mode(int modeId, char const *modeName);
};

Mode::Mode(int modeId, char const *modeName)
{
  id = modeId;
  name = modeName;
}

Mode modes[] = {
    Mode(0, "Off"),
    Mode(1, "Stargaze"),
    Mode(2, "Relax"),
    Mode(3, "Party"),
    Mode(4, "Reveal")};
