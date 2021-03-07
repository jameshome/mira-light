#ifndef NETWORK_H
#define NETWORK_H

// Setup network
void setupNetwork(const char *hostname, const char *ssid, const char *password)
{

  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("No wireless connection. Restarting MIRA...");
    delay(5000);
    ESP.restart();
  }
  uint16_t maxlen = strlen(hostname) + 7;
  char *fullhostname = new char[maxlen];
  uint8_t mac[6];
  WiFi.macAddress(mac);
  snprintf(fullhostname, maxlen, "%s-%02x%02x%02x", hostname, mac[3], mac[4], mac[5]);

  // Enable OTA updates
  ArduinoOTA.setHostname(fullhostname);
  delete[] fullhostname;
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
  Serial.println("Running on ");
  Serial.println(WiFi.localIP());
}

// Setup HTTP server

const char *PARAM_INPUT_1 = "output";
const char *PARAM_INPUT_2 = "state";

AsyncWebServer server(80);

const char *PARAM_MESSAGE = "message";

void serve404(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

#endif
