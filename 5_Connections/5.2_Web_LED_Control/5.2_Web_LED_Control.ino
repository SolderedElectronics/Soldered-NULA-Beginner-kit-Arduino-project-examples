/**
 **************************************************
 *
 * @file        5.2_Web_LED_Control.ino
 * @brief       Example that shows how to control an LED on the NULA MINI
 *              board through a Wi-Fi web interface. The web page allows
 *              the user to:
 *                - Turn the LED ON or OFF using buttons.
 *                - See the real-time LED status automatically updated.
 *
 *              The NULA MINI runs a small web server that serves a simple
 *              HTML page and handles HTTP requests from the browser.
 *
 *              For wiring details and more information, check the example
 *              documentation at: <link placeholder>
 *
 * @author      Soldered
 ***************************************************/

#include <WiFi.h>          // Include Wi-Fi library for network connection
#include <WebServer.h>     // Include WebServer library to host a simple web server

/*
Wi-Fi credentials.
Change these to match your Wi-Fi network name (SSID) and password.
*/
const char* ssid = "Stefan";
const char* password = "granatir";

/*
Pin number where the LED is connected.
*/
const int LED_PIN = 4;

/*
Create a web server object that listens on port 80 (default HTTP port).
*/
WebServer server(80);

/*
HTML content that will be sent to the browser when the user visits the NULA MINI’s IP address.
This page includes:
- Two buttons (ON / OFF)
- Automatic status updates using JavaScript
*/
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>NULA MINI LED Control</title>
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; }
    button { padding: 15px 30px; margin: 10px; font-size: 20px; }
    .status { font-size: 24px; margin-top: 20px; }
  </style>
</head>
<body>
  <h1>NULA MINI LED Control</h1>
  <button onclick="fetch('/led/on').then(()=>updateStatus())">ON</button>
  <button onclick="fetch('/led/off').then(()=>updateStatus())">OFF</button>
  <div class="status" id="status">Loading status...</div>

  <script>
    // Function that requests LED status from the NULA MINI and updates the page
    async function updateStatus() {
      let res = await fetch('/led/status');
      let text = await res.text();
      document.getElementById('status').innerHTML = 'LED is ' + text.toUpperCase();
    }

    // Run immediately after page load and update every 2 seconds
    updateStatus();
    setInterval(updateStatus, 2000);
  </script>
</body>
</html>
)rawliteral";

/*
Handler for the root URL "/"
Sends the HTML page stored in flash memory (PROGMEM).
*/
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

/*
Handler for "/led/on"
Turns the LED ON and responds with a simple text message.
*/
void handleLedOn() {
  digitalWrite(LED_PIN, HIGH);
  server.send(200, "text/plain", "on");
}

/*
Handler for "/led/off"
Turns the LED OFF and responds with a simple text message.
*/
void handleLedOff() {
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/plain", "off");
}

/*
Handler for "/led/status"
Returns the current LED state as plain text ("on" or "off").
This is used by the webpage’s JavaScript for live updates.
*/
void handleLedStatus() {
  String status = digitalRead(LED_PIN) ? "on" : "off";
  server.send(200, "text/plain", status);
}

/*
Setup function — runs once at startup.
Initializes serial communication, connects to Wi-Fi, configures the LED pin,
and starts the web server.
*/
void setup() {
  Serial.begin(115200);               // Start serial communication for debugging
  pinMode(LED_PIN, OUTPUT);           // Configure LED pin as output
  digitalWrite(LED_PIN, LOW);         // Ensure LED starts turned off

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);         // Start connecting to Wi-Fi

  /*
  Wait until Wi-Fi is connected.
  The dots (".") show progress in the Serial Monitor.
  */
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());     // Display the ESP32’s IP address

  /*
  Define routes (URLs) and their corresponding functions.
  */
  server.on("/", handleRoot);
  server.on("/led/on", handleLedOn);
  server.on("/led/off", handleLedOff);
  server.on("/led/status", handleLedStatus);

  /*
  Start the HTTP server.
  */
  server.begin();
  Serial.println("HTTP server started");
}

/*
Main loop — runs continuously.
Handles incoming client requests for the web server.
*/
void loop() {
  server.handleClient();
}
