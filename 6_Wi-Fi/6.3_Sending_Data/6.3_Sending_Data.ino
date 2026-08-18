/**
 **************************************************
 *
 * @file        6.3_Sending_Data.ino
 * @brief       Example that shows how to send data from the NULA board to a server on the internet using an HTTP POST
 *              request. Every few seconds the board makes up a random number and sends it to a webhook, which is a
 *              web address that simply collects whatever is sent to it and shows it to you in your browser.
 *              In example 6.1 we read data from the internet, here we write data to it.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
The WiFi library contains everything needed to join a Wi-Fi network.
*/
#include <WiFi.h>

/*
The HTTPClient library lets us speak HTTP, the language browsers use to talk to websites.
*/
#include <HTTPClient.h>

/*
These two variables hold the name of your Wi-Fi network (the SSID) and its password. Replace the text between the
quotation marks with your own network details.
*/
const char* ssid = "your ssid";
const char* password = "your password";

/*
This variable holds the address we send our data to. Open https://webhook.site in a browser, copy the unique link it
shows you at the top of the page, and paste it between the quotation marks below. Keep that browser tab open and you
will see every value the board sends appear in it.
Example: https://webhook.site/your-unique-id
*/
const char* webhookURL = "your unique url";

/*
This variable defines how much time passes between two messages, in milliseconds. 5000 milliseconds is five seconds.
Feel free to experiment with this value, but be aware that sending data very often is impolite towards whichever
server is receiving it.
*/
const unsigned long POST_INTERVAL = 5000;

/*
This variable remembers the moment when we sent the last message, so we know when the next one is due.
*/
unsigned long lastPost = 0;

void setup() {

  /*
  Serial.begin() establishes serial communication between your board and your computer via a USB cable. We use it here
  to follow the connection process and to see what the server answers.
  */
  Serial.begin(115200);
  Serial.println();
  Serial.println("Wi-Fi POST Request Example");

  /*
  randomSeed() gives the random number generator a starting point. Without it the board would produce the very same
  sequence of "random" numbers after every reset, which is easy to mistake for a broken program. esp_random() reads the
  hardware random number generator built into the chip, which gives us a genuinely different starting point every time.
  */
  randomSeed(esp_random());

  /*
  WiFi.begin() starts the connection attempt using the network name and password we defined above. The function only
  starts the process, it does not wait for it to finish.
  */
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");

  /*
  Because of that, we wait for the connection ourselves. WiFi.status() tells us the current state and WL_CONNECTED is
  the value it reports once we are online. One dot is printed every half second so we can see the board is still
  trying.
  */
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  /*
  WiFi.localIP() returns the address the router handed out to our board.
  */
  Serial.println();
  Serial.println("Connected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  /*
  millis() is a function that returns the number of milliseconds passed since the board began running the current
  program. We use it instead of delay() so the board stays free to do other work between messages.
  */
  unsigned long now = millis();

  /*
  Here we check how much time has passed since the last message. Only when POST_INTERVAL milliseconds have gone by do
  we send a new one, and we immediately remember the current time as the new starting point.
  */
  if (now - lastPost >= POST_INTERVAL) {
    lastPost = now;

    /*
    Before sending anything we check that we are still online. A Wi-Fi connection can drop at any time, and trying to
    send data without one would only waste time and print errors.
    */
    if (WiFi.status() == WL_CONNECTED) {

      /*
      Here we create our HTTP client object, which we named "http". We create it inside the if statement so that a
      fresh one is used for every message.
      */
      HTTPClient http;

      /*
      random() is a function that returns a whole random number. The first value is the lowest number it may return
      and the second one is the first number it may not return, so this call gives us a number from 0 to 100.
      In a real project this is where a sensor reading would go.
      */
      int randomNumber = random(0, 101);

      /*
      Here we build the data we are going to send. The format "name=value" is the same one a browser uses when you
      submit a simple web form, and String() converts our number into text so it can be joined to the rest with a plus
      sign.
      */
      String postData = "number=" + String(randomNumber);

      Serial.println("----------------------------------");
      Serial.println("Sending POST request to webhook.site...");
      Serial.print("Data: ");
      Serial.println(postData);

      /*
      begin() prepares the request by telling the client which address we want to reach. Nothing is sent yet.
      */
      http.begin(webhookURL);

      /*
      addHeader() adds one line of extra information to the request. A header tells the server something about the
      request itself, and this particular one tells it in which format our data is written, so it knows how to read it.
      */
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      /*
      POST() sends the request together with our data and waits for the answer. POST is the HTTP method used for
      sending data to a server, while GET, which we used in example 6.1, is the one used for reading data from it.
      The function returns a response code: numbers of 200 and above mean the server answered, while a negative number
      means we never got an answer at all.
      */
      int httpResponseCode = http.POST(postData);

      /*
      Here we check whether the server answered. If it did, we print the response code and the body of the answer.
      */
      if (httpResponseCode > 0) {
        Serial.print("Server response code: ");
        Serial.println(httpResponseCode);
        String response = http.getString();
        Serial.println("Response body:");
        Serial.println(response);
      } else {

        /*
        errorToString() turns the negative error code into a short sentence, which is much easier to understand than
        the number on its own.
        */
        Serial.print("POST failed. Error: ");
        Serial.println(http.errorToString(httpResponseCode));
      }

      /*
      end() closes the connection and frees the memory the client was using. Always close a connection once you are
      done with it, otherwise a program that makes many requests will slowly run out of memory.
      */
      http.end();
    } else {

      /*
      If we are not connected, we simply start a new connection attempt and try again on the next pass.
      */
      Serial.println("Wi-Fi not connected. Trying to reconnect...");
      WiFi.begin(ssid, password);
    }
  }
}
