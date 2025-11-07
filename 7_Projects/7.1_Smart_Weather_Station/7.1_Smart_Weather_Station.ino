/**
 **************************************************
 *
 * @file        7.1_Smart_Weather_Station.ino
 * @brief       Example project that combines the SHTC3 temperature and humidity sensor,
 *              LCD display, and Wi-Fi connection. The NULA MINI measures
 *              temperature and humidity, displays them on the LCD, and periodically
 *              sends the readings to webhook.site using an HTTP POST request.
 *
 *              For wiring details and more information, check the example documentation at: <link placeholder>
 *
 * @author      Soldered
 ***************************************************
 */

#include <WiFi.h>         // Wi-Fi connection
#include <HTTPClient.h>   // HTTP client for POST requests
#include "SHTC3-SOLDERED.h"  // Soldered SHTC3 temperature and humidity sensor
#include "LCD-SOLDERED.h"    // Soldered LCD display library

// --- Replace with your Wi-Fi credentials ---
const char* ssid = "Soldered";
const char* password = "dasduino";

// --- Replace with your unique webhook.site URL ---
const char* webhookURL = "https://webhook.site/42ff81ac-52d9-4cfa-95d4-ce933fee1f34";

// --- Create sensor and LCD objects ---
SHTC3 shtc3;
LCD lcd(16, 2);

// --- Variables for data ---
float temperature = 0.0;
float humidity = 0.0;
unsigned long lastUpdate = 0;
const unsigned long UPDATE_MS = 30000; // Update every 30 seconds

void setup() {
  Serial.begin(115200);
  delay(500);

  // --- Initialize LCD ---
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Weather Station");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(1000);
  lcd.clear();

  // --- Initialize SHTC3 sensor ---
  if (!shtc3.begin()) {
    Serial.println("SHTC3 init failed!");
    lcd.print("SHTC3 error!");
    while (1) delay(100);
  }

  // --- Connect to Wi-Fi ---
  Serial.print("Connecting to Wi-Fi");
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(0, 1);
    lcd.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.print("WiFi Connected!");
  delay(800);
  lcd.clear();

  Serial.println("Smart Weather Station ready!");
}

void loop() {
  unsigned long now = millis();

  // Take a new measurement every 5 seconds
  if (now - lastUpdate >= UPDATE_MS) {
    lastUpdate = now;

    // --- Read SHTC3 sensor values ---
    shtc3.sample();
    temperature = shtc3.readTempC();
    humidity = shtc3.readHumidity();

    // --- Print to Serial Monitor ---
    Serial.print("Temperature: ");
    Serial.print(temperature, 2);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity, 2);
    Serial.println(" %");

    // --- Display on LCD ---
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature, 1);
    lcd.print((char)223); // Degree symbol
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(humidity, 1);
    lcd.print(" %");

    // --- Send data to webhook.site ---
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(webhookURL);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      String postData = "temperature=" + String(temperature, 2) + "&humidity=" + String(humidity, 2);
      int httpCode = http.POST(postData);

      if (httpCode > 0) {
        Serial.print("POST successful! Response code: ");
        Serial.println(httpCode);
      } else {
        Serial.print("POST failed. Error: ");
        Serial.println(http.errorToString(httpCode));
      }

      http.end();
    } else {
      Serial.println("Wi-Fi disconnected. Trying to reconnect...");
      WiFi.begin(ssid, password);
    }
  }
}
