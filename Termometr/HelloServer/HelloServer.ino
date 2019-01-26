#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef STASSID
#define STASSID "ASUS"
#define STAPSK  "rejestracja"
#endif

#define ONE_WIRE_BUS 2
#define HISTORY 200
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

const char* ssid = STASSID;
const char* password = STAPSK;
const long interval = 1000;
float currentTemp;
unsigned long previousMillis = 0;
float history[HISTORY];
int historyIndex = 0;
ESP8266WebServer server(80);


void handleRoot() {
  server.send(200, "text/plain",  printHistory());
}

void reconnectToWifi() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

String printHistory() {
  String output = "";
  for (int i = 0; i < HISTORY; i++) {
    int index = i + historyIndex;
    if (index >= HISTORY) {
      index -= HISTORY;
    }
    //output += (history[index])


    float tempValue = 0;
    if (history[index] != NULL) {
      tempValue = history[index];
    }
    output += (String(index) +":"+  String(tempValue) + "\n");
  }
  return output;
}

void setup(void) {
  Serial.begin(115200);
  DS18B20.begin();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.begin();
}

void loop(void) {
  reconnectToWifi();
  server.handleClient();

  //1s loop
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    DS18B20.requestTemperatures();
    currentTemp = DS18B20.getTempCByIndex(0);
    saveToHistory(currentTemp);
    //   Serial.println(String(currentTemp).c_str());
  }
}

void saveToHistory(float tempToSave) {
  history[historyIndex] = tempToSave;
  historyIndex++;
  if (historyIndex >= HISTORY) {
    historyIndex = 0;
  }
}
