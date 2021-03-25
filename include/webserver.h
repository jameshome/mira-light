#ifndef WEBSERVER_H
#define WEBSERVER_H
#include "../app/webserver/StaticFiles.h"

WiFiServer server(80);
Application app;

void readPatterns(Request &req, Response &res)
{
  const int capacity = JSON_ARRAY_SIZE(patternCount) + patternCount * JSON_OBJECT_SIZE(2);
  StaticJsonDocument<capacity> doc;

  for (int i = 0; i < patternCount; i++)
  {
    doc[i]["id"] = i;
    doc[i]["name"] = patterns[i].name;
  }
  res.set("Content-Type", "application/json");
  serializeJson(doc, res);
}

void readAnimation(Request &req, Response &res)
{
  DynamicJsonDocument doc(256);
  doc["id"] = activePattern;
  doc["name"] = patterns[activePattern].name;
  doc["brightness"] = dimmer.getCount();
  res.set("Content-Type", "application/json");
  serializeJson(doc, res);
}

void updateAnimation(Request &req, Response &res)
{
  DynamicJsonDocument doc(256);
  deserializeJson(doc, req);
  selectPattern(doc["id"]);
  setBrightness(doc["brightness"]);
  return readAnimation(req, res);
}

#endif
