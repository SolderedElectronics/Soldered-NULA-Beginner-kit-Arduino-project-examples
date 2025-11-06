/**
 **************************************************
 *
 * @file        6.1_Connecting_and_Getting_Data.ino
 * @brief       Example that shows how to connect the NULA MINI to a Wi-Fi network
 *              and make a simple HTTP GET request to fetch data from example.com.
 *              This example introduces basic Wi-Fi setup and web communication
 *              using the built-in Wi-Fi library.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

#include <WiFi.h>       // Include the Wi-Fi library for ESP-based NULA boards
#include <HTTPClient.h> // Include the HTTP client library for web requests

// --- Replace these with your Wi-Fi credentials ---
const char* ssid = "your ssid";
const char* password = "your password";

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Wi-Fi GET Request Example");

  /*
  Start connecting to Wi-Fi network using provided credentials.
  */
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  /*
  Create an HTTP client and make a simple GET request to example.com
  */
  HTTPClient http;
  String url = "http://example.com/";

  Serial.print("Requesting data from: ");
  Serial.println(url);

  http.begin(url);                // Initialize connection
  int httpCode = http.GET();      // Send GET request

  if (httpCode > 0) {
    Serial.print("HTTP Response Code: ");
    Serial.println(httpCode);

    String payload = http.getString();  // Get response body
    Serial.println("Received data:");
    Serial.println("----------------------------------");
    Serial.println(payload);
    Serial.println("----------------------------------");
  } else {
    Serial.print("Request failed. Error: ");
    Serial.println(http.errorToString(httpCode));
  }

  http.end(); // Close connection
}

void loop() {
  // Nothing here — we just run the GET request once in setup()
}
