# Mira Light

**(Mira Light is pre-release — many features haven't been implemented yet)**

Mira Light is an opinionated open source control interface for an arbitrary number of individually addressable strip-mounted LEDs, using the FastLED library running on an ESP32 microcontroller.

Mira Light is designed for marine cockpits, but it works great anywhere you need primary lighting controls to be obvious and reliable, while still allowing complete flexibility for visual ridiculousness.

It accomplishes this by providing two interfaces to dozens of lighting configurations, or Patterns:

- **Panel**, a physical control with three easily understood knobs
- **Web**, a web interface

## Panel

The Panel consists of a five position switch and a pair of rotary encoders with push buttons.

An SVG of the panel useful for fabrication is included in the repository's `/hardware` directory.

### Dimmer

The first rotary encoder, or Dimmer, allows the default brightness of the selected Pattern to be changed. It always works the same way.

### Adjuster

The second rotary encoder, or Adjuster, changes behavior depending on the selected Pattern, allowing each pattern to have one axis of adjustment.

### Mode Switch

The five position switch, or Mode Switch, chooses between five modes:

- **Off**: software off, turning all lights off
- **Stargaze**: starts a Pattern that preserves night vision, suitable for stargazing or making a log entry on a moonless night.
- **Relax**: starts a Pattern that is just right for relaxing in the cockpit as the sun goes down.
- **Party**: starts a Pattern that does something visually ridiculous. It also enables the rotary encoder push buttons as the Pattern Explorer — pushing the left one starts the previous Pattern; pushing the right one starts the next Pattern. The Pattern Explorer skips the Mode Switch Patterns, and the buttons are disabled in all but Party mode.
- **Reveal**: starts a Pattern that acts as the blinding light of reason: all white, full brightness. For emergencies.

Mode Switch changes start each Pattern with its default Dimmer and Adjuster settings, even if they’ve been changed previously in the session.

## Web

The Web interface mirrors the functionality of the Mode Switch, with additional interactive elements to make color selection and Pattern exploration easier.

## Hardware installation

There's an example schematic in the `/hardware` directory. Note that the hardware is designed to work with a 12V marine power system, and addresses several power management issues specific to that use case. Adapt to your own use case accordingly.

## Software installation

You'll want [PlatformIO](https://platformio.org/) and a copy of this repository.

You'll need to specify your wifi credentials by creating an `include/secrets.h` containing:

```cpp
#pragma once
const char *wifi_ssid = "YOUR_WIFI_SSID";
const char *wifi_password = "YOUR_WIFI_PASSWORD";

```
