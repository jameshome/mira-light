# mira

mira is an open source control interface for an arbitrary number of individually addressable strip LEDs. It’s designed to make primary interactions obvious and reliable, while allowing for secondary interactions that are flexible and ridiculous. It does this by providing two primary control surfaces:

- mira panel, a physical control with three easily understood knobs
- mira app, a web interface (in development)

mira runs on the ESP32 wifi-enabled microcontroller, connected to LEDs like the WS8212B.

### Install

You'll want [PlatformIO](https://platformio.org/), an ESP32, and some strip LEDs.

You will also want to specify your wifi credentials by creating an `include/secrets.h` containing:

```cpp
#pragma once
const char *wifi_ssid = "YOUR_WIFI_SSID";
const char *wifi_password = "YOUR_WIFI_PASSWORD";

```
