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
  server.send(200, "text/html", "<html> <head> <script src='https://www.chartjs.org/dist/2.7.3/Chart.bundle.js'></script> </head> <body> <canvas id='canvas'></canvas> <script>var d=[ "
              + printHistory() +
              "]; var len=d.length; var timeFormat='DD-MM-YYYY HH:mm'; function newDate(minutes){return new Date(new Date().getTime() - minutes * 60000)}var l=[]; for(var i=0;i<len;i++){l.push(newDate(len-i));}var color=Chart.helpers.color; var config={type: 'line', data:{labels: l, datasets: [{label: 'Temperatura Pieca', backgroundColor: 'rgb(255, 99, 132)', borderColor: 'rgb(255, 99, 132)', fill: false, data: d}]}, options:{scales:{xAxes: [{type: 'time', time:{format: timeFormat, tooltipFormat: 'll HH:mm'}}]},}}; window.onload=function(){var ctx=document.getElementById('canvas').getContext('2d'); window.myLine=new Chart(ctx, config);}; </script> </body></html>");
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
    float tempValue = 0;
    if (history[index] != NULL) {
      output += String(history[index]);
      if (i < (HISTORY - 1)) {
        output += ",";
      }
    }
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
