/**
 **************************************************
 *
 * @file        SHTC3_WebServer.ino
 * @brief       Example showing how to measure temperature and humidity with the
 *              SHTC3 sensor and display the readings on a self-hosted web page.
 *              The NULA MINI connects to a local Wi-Fi network, starts a web
 *              server, and updates the values every two seconds using
 *              JavaScript for live refresh.
 * @author      Soldered
 ***************************************************
 */

#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include "SHTC3-SOLDERED.h"

//Change the WiFi credentials
const char* ssid = "Your SSID";
const char* password = "Your password";

SHTC3 shtcSensor;
WebServer server(80);

float temperature = 0.0;
float humidity = 0.0;
unsigned long lastUpdate = 0;
const unsigned long UPDATE_MS = 2000; // Update every 2 seconds


// HTML page 
const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>ESP32-C6 SHTC3 Live Data</title>
  <style>
    body { font-family: Arial; text-align: center; background: #f4f4f4; }
    h1 { color: #333; margin-top: 40px; }
    .value { font-size: 2em; color: #0078D7; }
  </style>
</head>
<body>
  <h1>ESP32-C6 SHTC3 Sensor</h1>
  <p>Temperature: <span id="temp" class="value">--</span> °C</p>
  <p>Humidity: <span id="hum" class="value">--</span> %</p>
  <p style="font-size:0.9em;color:#777;">(Updates automatically every 2 seconds)</p>

  <script>
    // Ask the ESP32 for new data and update the numbers on the page
    async function updateData() {
      try {
        const response = await fetch('/data');
        const json = await response.json();
        document.getElementById('temp').textContent = json.temp.toFixed(1);
        document.getElementById('hum').textContent  = json.hum.toFixed(1);
      } catch (e) {
        console.log('Error fetching data:', e);
      }
    }
    setInterval(updateData, 2000);
    updateData();
  </script>
</body>
</html>
)rawliteral";


void handleRoot() {
  server.send(200, "text/html", MAIN_page);
}


void handleData() {
  String json = "{\"temp\":" + String(temperature, 2) + ",\"hum\":" + String(humidity, 2) + "}";
  server.send(200, "application/json", json);
}


void setup() {
  Wire.begin();
  Serial.begin(115200);
  delay(500);

  if (!shtcSensor.begin()) {
    Serial.println("SHTC3 init failed!");
    while (1) delay(10);
  }

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
  Serial.println("Web server started!");
}


void loop() {
  server.handleClient();

  unsigned long now = millis();
  if (now - lastUpdate >= UPDATE_MS) {
    lastUpdate = now;
    shtcSensor.sample();
    temperature = shtcSensor.readTempC();
    humidity = shtcSensor.readHumidity();

    Serial.print("Temp: ");
    Serial.print(temperature, 2);
    Serial.print(" °C, Hum: ");
    Serial.print(humidity, 2);
    Serial.println(" %");
  }
}
