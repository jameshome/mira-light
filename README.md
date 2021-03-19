# Mira Light

**Mira Light is pre-release — here's the task list for 1.0.0:**

- [x] **OTA updates** enabled
- [x] Basic **Pattern object** selection logic
- [x] **Animation objects** unloaded from memory on Pattern change
- [x] **Panel controls** work as intended
- [ ] **App controls** work as intended
- [ ] Improved **Pattern object** selection logic
- [ ] **Built-in Patterns** chosen/designed

Mira Light is an opinionated open source control interface for an arbitrary number of individually addressable strip-mounted LEDs, using the FastLED library running on an ESP32 microcontroller. It provides a set of consistent physical and web-based controls, pattern and animation object separation for convenience, stability, and performance.

Mira Light is designed for marine cockpits, but it works great anywhere you need primary lighting controls to be obvious and reliable, while still allowing complete flexibility for visual ridiculousness.

It accomplishes this by providing two interfaces to dozens of lighting configurations, or Patterns:

- **Panel**, a physical control with three easily understood knobs
- **App**, a React-based web interface

## Panel

The Panel consists of a five position switch and a pair of rotary encoders with push buttons.

An SVG of the panel useful for fabrication is included in the repository's `/hardware` directory.

### Dimmer

The first rotary encoder, or Dimmer, allows the default brightness of the selected Pattern to be changed. It always works the same way.

### Adjuster

The second rotary encoder, or Adjuster, changes behavior depending on the selected Pattern, allowing each Pattern to have one axis of physical adjustment on the Panel.

### Mode Switch

The five position switch, or Mode Switch, chooses between five modes:

- **Off**: software off, turning all lights off
- **Stargaze**: starts a Pattern that preserves night vision, suitable for stargazing or making a log entry on a moonless night.
- **Relax**: starts a Pattern that is just right for relaxing in the cockpit as the sun goes down.
- **Party**: starts a Pattern that does something visually ridiculous. It also enables the rotary encoder push buttons as the Pattern Explorer — pushing the left one starts the previous Pattern; pushing the right one starts the next Pattern. The Pattern Explorer skips the Mode Switch Patterns, and the buttons are disabled in all but Party mode.
- **Reveal**: starts a Pattern that acts as the blinding light of reason: all white, full brightness. For emergencies.

Mode Switch changes start each Pattern with its default Dimmer and Adjuster settings, discarding any changes made previously in the session.

## App

The App is a React-based web interface that mirrors the functionality of the Mode Switch, with additional interactive elements to make color selection and Pattern exploration easier. The web app interacts with the microcontroller over a JSON-based API.

### API

- api/animation - pattern
  - id
  - name
  - brightness
  - speed
  - color
    - [ ]
  - palette
    - [ ]
- api/patterns
  - id
  - name
- api/palettes

  - id
  - name

  ```json
  {
    "pattern": { "name": "Pacifica", "id": 6 },
    "brightness": 35,
    "speed": 0,
    "color": [],
    "palette": ["palette_pacifica_1", "palette_pacifica_2", "palette_pacifica_3"]
  }
  ```

## Hardware installation

There's an example schematic in the `/hardware` directory. Note that the hardware is designed to work with a 12V marine power system, and addresses several power management issues specific to that use case. Adapt to your own use case accordingly.

## Software installation

Get [PlatformIO](https://platformio.org/) and a copy of this repository.

Specify your wifi credentials by creating an `include/secrets.h` containing:

```cpp
#pragma once
const char *wifi_ssid = "YOUR_WIFI_SSID";
const char *wifi_password = "YOUR_WIFI_PASSWORD";
```

Edit `platformio.ini` to specify your microcontroller's IP address as `upload_port` to enable OTA updates. Comment out `upload_port` and `upload_protocol` to update over USB.

Get [Node.js](https://nodejs.org/) and [Yarn](https://yarnpkg.com/) to build the React-based App. To install dependencies:

```sh
$ cd app
$ yarn install
```

To run a local development instance of the app, configure the `proxy` setting in `package.json`, pointing it to the IP address of your microcontroller. Then start the development server:

```sh
$ yarn start
```

The App uses [awot-scripts](https://github.com/lasselukkari/awot-scripts) to generate `app/webserver/StaticFiles.h`, a gzip compressed payload header file that is included by `include/webserver.h`. To update `StaticFiles.h`:

```sh
$ yarn build
$ yarn dist
```

A more detailed explanation of how this process works can be found in the excellent [aWOT webserver tutorial](https://github.com/lasselukkari/aWOT).
