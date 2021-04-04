#ifndef WEBSERVER_H
#define WEBSERVER_H
#include "../app/cpp/StaticFiles.h"

WiFiServer server(80);
Application app;

void readPatterns(Request &req, Response &res)
{
  const int capacity = JSON_ARRAY_SIZE(patternCount) + patternCount * JSON_OBJECT_SIZE(2);
  DynamicJsonDocument json(capacity);
  for (int i = 0; i < patternCount; i++)
  {
    json[i]["id"] = i;
    json[i]["name"] = patterns[i].name;
  }
  res.set("Content-Type", "application/json");
  serializeJson(json, res);
}

void readAnimation(Request &req, Response &res)
{
  DynamicJsonDocument json(256);
  json["id"] = activePattern;
  json["name"] = patterns[activePattern].name;
  json["brightness"] = brightness;
  json["hue"] = current.hue;
  json["saturation"] = current.saturation;
  json["speed"] = current.speed;
  res.set("Content-Type", "application/json");
  serializeJson(json, res);
}

void updateAnimation(Request &req, Response &res)
{
  DynamicJsonDocument json(256);
  deserializeJson(json, req);
  if (json["id"] || json["id"] == 0)
  {
    uint8_t id = json["id"];
    selectPattern(id);
  }
  if (json["brightness"])
  {
    setBrightness(json["brightness"]);
  }
  if (json["hue"] && json["saturation"])
  {
    current.hue = json["hue"];
    current.saturation = json["saturation"];
  }
  return readAnimation(req, res);
}

#endif
