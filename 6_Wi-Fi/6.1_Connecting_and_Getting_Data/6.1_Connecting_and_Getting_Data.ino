/**
 **************************************************
 *
 * @file        6.1_Connecting_and_Getting_Data.ino
 * @brief       Example that shows how to connect the NULA board to a Wi-Fi network and then ask a website for data.
 *              The board makes an HTTP GET request to example.com and prints whatever the website sends back to the
 *              Serial Monitor. This is the first step towards any project that needs data from the internet.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
The WiFi library contains everything needed to join a Wi-Fi network. It comes together with the board definition, so
there is nothing to install for this one.
*/
#include <WiFi.h>

/*
The HTTPClient library lets us speak HTTP, the language browsers use to talk to websites. Without it we would have to
build the requests out of raw text ourselves.
*/
#include <HTTPClient.h>

/*
These two variables hold the name of your Wi-Fi network (the SSID) and its password. Replace the text between the
quotation marks with your own network details, otherwise the board has nothing to connect to.
Note that most boards, including the NULA board, can only connect to 2.4 GHz networks and not to 5 GHz ones.
*/
const char* ssid = "your ssid";
const char* password = "your password";

void setup() {

  /*
  Serial.begin() establishes serial communication between your board and your computer via a USB cable. In this
  example the Serial Monitor is the only place where we can see what is happening, so it is essential here.
  */
  Serial.begin(115200);
  Serial.println();
  Serial.println("Wi-Fi GET Request Example");

  /*
  WiFi.begin() starts the connection attempt using the network name and password we defined above. The function does
  not wait for the connection to finish, it only starts the process and immediately returns.
  */
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");

  /*
  Because WiFi.begin() returns immediately, we have to wait for the connection ourselves. WiFi.status() tells us the
  current state of the connection, and WL_CONNECTED is the value it reports once we are online. This while loop keeps
  running as long as we are not connected yet, printing one dot every half second so we can see that the board is
  still trying.
  */
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  /*
  Once we are through the loop above, we are connected. WiFi.localIP() returns the address the router handed out to
  our board. Write it down, we will need it in the next example.
  */
  Serial.println();
  Serial.println("Connected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  /*
  Here we create our HTTP client object, which we named "http", and the address we want to visit. A URL is the same
  kind of address you type into a browser.
  */
  HTTPClient http;
  String url = "http://example.com/";

  Serial.print("Requesting data from: ");
  Serial.println(url);

  /*
  begin() prepares the request by telling the client which address we want to reach. Nothing is sent over the network
  yet at this point.
  */
  http.begin(url);

  /*
  GET() actually sends the request and waits for the website to answer. GET is the HTTP method used for reading data,
  which is exactly what a browser does every time you open a page.
  The function returns a response code: numbers of 200 and above mean the server answered, while a negative number
  means we never got an answer at all.
  */
  int httpCode = http.GET();

  /*
  Here we check whether we got an answer. If we did, we print the response code and then the body of the response,
  which for example.com is the HTML of the page itself.
  */
  if (httpCode > 0) {
    Serial.print("HTTP Response Code: ");
    Serial.println(httpCode);

    /*
    getString() returns the content the server sent us as text.
    */
    String payload = http.getString();
    Serial.println("Received data:");
    Serial.println("----------------------------------");
    Serial.println(payload);
    Serial.println("----------------------------------");
  } else {

    /*
    errorToString() turns the negative error code into a short sentence, which is much easier to understand than the
    number on its own.
    */
    Serial.print("Request failed. Error: ");
    Serial.println(http.errorToString(httpCode));
  }

  /*
  end() closes the connection and frees the memory the client was using. Always close a connection once you are done
  with it, otherwise a program that makes many requests will slowly run out of memory.
  */
  http.end();
}

void loop() {
  //Nothing happens here, the request was made once in setup()
}
