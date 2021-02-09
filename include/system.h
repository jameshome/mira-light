#ifndef SYSTEM_H
#define SYSTEM_H

#include "secrets.h"
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <TelnetPrint.h>
#include <EasyButton.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <ESP32Encoder.h>

// uC setup
#define LED_PIN 22

#define OFF_BUTTON_PIN 15
#define STARGAZE_BUTTON_PIN 16
#define RELAX_BUTTON_PIN 27
#define PARTY_BUTTON_PIN 28
#define REVEAL_BUTTON_PIN 29

#define PRESET_BUTTON_PIN 33
#define ADJUST_BUTTON_PIN 25

#define DIMMER_ENCODER_PIN_A 13
#define DIMMER_ENCODER_PIN_B 14
#define ADJUST_ENCODER_PIN_A 26
#define ADJUST_ENCODER_PIN_B 27

// Modes and presets setup
int activeMode = 0;
bool activePattern = false;

void selectMode(int mode)
{
  activePattern = false;
  activeMode = mode;
}

void nextMode()
{
  activePattern = false;
  activeMode = (activeMode + 1) % 4;
}

void nextPartyPreset()
{
  activePattern = false;
  activeMode = (activeMode + 1) % 4;
}

EasyButton preset_button(PRESET_BUTTON_PIN);

void preset_press()
{
  nextPartyPreset();
  TelnetPrint.print("Party preset changed: ");
}

EasyButton mode_button(ADJUST_BUTTON_PIN);

void mode_press()
{
  nextMode();
  TelnetPrint.print("Mode changed: ");
}

// LED setup
#define NUM_LEDS 30
CRGB leds[NUM_LEDS];

// Adjuster setup
ESP32Encoder adjust;
int16_t lastAdjustState = 0;

int getAdjustment()
{
  uint8_t adjustState = adjust.getCount();

  if (adjustState != lastAdjustState)
  {
    lastAdjustState = adjustState;
    TelnetPrint.print("Adjustment set to ");
    TelnetPrint.println(adjustState);
  }
  return adjustState;
}

// Dimmer setup
ESP32Encoder dimmer;
uint8_t dimmerInit = 40;
int16_t lastDimmerState = 0;
uint8_t brightness = dimmerInit;
uint8_t brightnessMax = 200;

void changeBrightness()
{
  int16_t dimmerState = dimmer.getCount();
  if (dimmerState != lastDimmerState)
  {
    if (dimmerState < 5)
    {
      dimmer.setCount(5);
      brightness = 5;
    }
    else if (dimmerState > brightnessMax)
    {
      dimmer.setCount(brightnessMax);
      brightness = brightnessMax;
    }
    else
    {
      if (dimmerState > dimmerInit)
      {
        if (dimmerState < lastDimmerState)
          dimmerState = (dimmerState - 10);
        else
          dimmerState = (dimmerState + 10);
      }
      dimmer.setCount(dimmerState);
      brightness = dimmerState;
    }
    TelnetPrint.print("Brightness set to ");
    TelnetPrint.println(brightness);
    FastLED.setBrightness(brightness);
    lastDimmerState = dimmerState;
  }
}

// OTA setup
void setupOTA(const char *nameprefix, const char *ssid, const char *password)
{
  uint16_t maxlen = strlen(nameprefix) + 7;
  char *fullhostname = new char[maxlen];
  uint8_t mac[6];
  WiFi.macAddress(mac);
  snprintf(fullhostname, maxlen, "%s-%02x%02x%02x", nameprefix, mac[3], mac[4], mac[5]);
  ArduinoOTA.setHostname(fullhostname);
  delete[] fullhostname;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Failed to connect. Restarting");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else
      type = "filesystem";

    Serial.println("Updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println(" Success");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("\nAuth failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("\nBegin failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("\nConnection failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("\nReceive failed");
    else if (error == OTA_END_ERROR)
      Serial.println("\nEnd failed");
  });

  ArduinoOTA.begin();
  Serial.println("OTA @: ");
  Serial.println(WiFi.localIP());
}

// Event handling setup
void loopEvents()
{
  changeBrightness();
  preset_button.read();
  mode_button.read();
  ArduinoOTA.handle();
}

#endif