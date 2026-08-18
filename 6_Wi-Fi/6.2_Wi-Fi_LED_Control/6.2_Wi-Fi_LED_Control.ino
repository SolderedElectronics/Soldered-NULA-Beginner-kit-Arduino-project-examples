/**
 **************************************************
 *
 * @file        6.2_Wi-Fi_LED_Control.ino
 * @brief       Example that shows how to control an LED from a web page. The NULA board joins your Wi-Fi network and
 *              then becomes a small web server of its own, which means you can open it in the browser of your phone
 *              or computer. The page has an ON and an OFF button and shows the current state of the LED.
 *              In the previous example the board asked a website for data, here the board is the one being asked.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
The WiFi library contains everything needed to join a Wi-Fi network.
*/
#include <WiFi.h>

/*
The WebServer library does the hard work of being a web server: it listens for browsers that want to connect and
hands each request over to a function we wrote.
*/
#include <WebServer.h>

/*
The ESPmDNS library lets the board announce itself on your network under a friendly name. Thanks to it you can open
http://nulamini.local/ in your browser instead of having to remember the IP address.
*/
#include <ESPmDNS.h>

/*
These two variables hold the name of your Wi-Fi network (the SSID) and its password. Replace the text between the
quotation marks with your own network details. Your phone or computer has to be on the same network as the board,
otherwise it will not be able to reach the page.
*/
const char* ssid = "your ssid";
const char* password = "your password";

/*
This is a variable to which we pass the number of pin that we had connected the LED to.
The NULA board has a pin naming logic as follows: IO4, where 4 is the number that we give to the variable.
If you wish to use a different pin, make sure you are using a IO__ marked pin.
*/
const int LED_PIN = 4;

/*
Here we create our web server object, which we named "server". The number 80 is the port it listens on. A port is
like a door number on the board: port 80 is the standard door for web pages, which is why browsers try it by default
and why we do not have to type it into the address bar.
*/
WebServer server(80);

/*
This is the web page itself, written in HTML and stored as one long piece of text. HTML is the language that
describes what a page looks like, the same language example.com sent us in the previous example.
The R"rawliteral( ... )rawliteral" wrapping is a C++ trick that lets us write many lines of text, including
quotation marks, without having to escape every one of them.
PROGMEM tells the board to keep this text in its flash memory rather than in its much smaller working memory.
The page contains two buttons and a small piece of JavaScript, which is code that runs inside the browser and asks
the board for the current LED state every two seconds so the displayed status stays up to date on its own.
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
The four functions below are called handlers. A handler is a function that the server runs when a browser asks for
one particular address. We only write them here, we do not call them ourselves anywhere in the program: further down
in setup() we tell the server which address belongs to which handler, and from then on the server calls them for us.

This first handler answers the root address "/", which is what the browser asks for when you simply open the board's
address. It sends back the web page we prepared above.
The three numbers and words in server.send() are the response code (200 means "here you go"), the type of content we
are sending, and the content itself.
*/
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

/*
This handler answers the address "/led/on". It turns the LED on and answers with the short text "on", which the
JavaScript in the page uses to update the status line.
*/
void handleLedOn() {
  digitalWrite(LED_PIN, HIGH);
  server.send(200, "text/plain", "on");
}

/*
This handler answers the address "/led/off" and does the opposite: it turns the LED off and answers with "off".
*/
void handleLedOff() {
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/plain", "off");
}

/*
This handler answers the address "/led/status" and reports the current state of the LED. digitalRead() gives us the
value the pin currently has, and the question mark is a short way of writing an if statement: if the value is HIGH we
answer "on", otherwise we answer "off".
*/
void handleLedStatus() {
  String status = digitalRead(LED_PIN) ? "on" : "off";
  server.send(200, "text/plain", status);
}

void setup() {

  /*
  Serial.begin() establishes serial communication between your board and your computer via a USB cable. We use it here
  to follow the connection process and to find out the address of the board.
  */
  Serial.begin(115200);

  /*
  pinMode() is a function that configures the specified pin to behave either as an input or in this case as an output.
  As our pin needs to turn on the LED, we will put the pin in OUTPUT mode. Right after that we write LOW to it, so
  that the LED starts out switched off and the status shown on the page matches reality.
  */
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  /*
  WiFi.begin() starts the connection attempt. The function only starts the process, it does not wait for it to finish.
  */
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  /*
  Because of that, we wait for the connection ourselves. WiFi.status() tells us the current state and WL_CONNECTED is
  the value it reports once we are online. The dots (".") show progress in the Serial Monitor.
  */
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  /*
  WiFi.localIP() returns the address the router handed out to our board. Typing this address into a browser on the
  same network opens the page we prepared above.
  */
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /*
  MDNS.begin() claims a name on the local network, in this case "nulamini". If it succeeds, the board can also be
  reached at http://nulamini.local/, which is far easier to remember than a row of numbers. Like begin() functions on
  sensors, it returns true on success and false on failure, so we check the result and print a message either way.
  */
  if(MDNS.begin("nulamini")){
    Serial.println("mDNS responder started!");
    Serial.println("Access the board in your browser at: http://nulamini.local/");
  } else {
    Serial.println("Error starting mDNS responder!");
  }

  /*
  server.on() connects an address to one of the handler functions we wrote above. This is called routing. From here on
  the server knows that a browser asking for "/led/on" should be answered by handleLedOn(), and so on for the rest.
  Notice that we pass the name of the function without brackets: we are handing over the function itself, not calling
  it right now.
  */
  server.on("/", handleRoot);
  server.on("/led/on", handleLedOn);
  server.on("/led/off", handleLedOff);
  server.on("/led/status", handleLedStatus);

  /*
  server.begin() opens the door and starts listening for browsers.
  */
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {

  /*
  server.handleClient() checks whether a browser has sent us anything and, if it has, runs the matching handler
  function. This is the reason the loop has to keep running: if we stopped calling this function the page would stop
  responding. For the same reason, avoid long delay() calls in this loop.
  */
  server.handleClient();
}
