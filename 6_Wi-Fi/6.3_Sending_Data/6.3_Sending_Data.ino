/**
 **************************************************
 *
 * @file        6.3_Sending_Data.ino
 * @brief       Example that shows how to send data from the NULA MINI
 *              to a web server using an HTTP POST request.
 *              In this example, we send a random number every few seconds
 *              to a custom webhook URL hosted on webhook.site.
 *
 *              For wiring details and more information, check the example
 *              documentation at: <link placeholder>
 *
 * @author      Soldered
 ***************************************************
 */

#include <WiFi.h>        // Include Wi-Fi library for network connection
#include <HTTPClient.h>  // Include HTTP client library to send POST requests

// --- Replace these with your Wi-Fi credentials ---
const char* ssid = "your ssid";
const char* password = "your password";

/*
Enter your unique Webhook.site URL here.
Go to https://webhook.site, copy the link provided, and paste it below.
Example: https://webhook.site/your-unique-id
*/
const char* webhookURL = "your unique url";

// --- Time between data posts (in milliseconds) ---
const unsigned long POST_INTERVAL = 5000;
unsigned long lastPost = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Wi-Fi POST Request Example");

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  unsigned long now = millis();

  if (now - lastPost >= POST_INTERVAL) {
    lastPost = now;

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      // Create a random number between 0 and 100
      int randomNumber = random(0, 101);

      // Create the data payload
      String postData = "number=" + String(randomNumber);

      Serial.println("----------------------------------");
      Serial.println("Sending POST request to webhook.site...");
      Serial.print("Data: ");
      Serial.println(postData);

      // Begin connection and send POST request
      http.begin(webhookURL);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpResponseCode = http.POST(postData);

      // Check server response
      if (httpResponseCode > 0) {
        Serial.print("Server response code: ");
        Serial.println(httpResponseCode);
        String response = http.getString();
        Serial.println("Response body:");
        Serial.println(response);
      } else {
        Serial.print("POST failed. Error: ");
        Serial.println(http.errorToString(httpResponseCode));
      }

      http.end(); // Close connection
    } else {
      Serial.println("Wi-Fi not connected. Trying to reconnect...");
      WiFi.begin(ssid, password);
    }
  }
}
