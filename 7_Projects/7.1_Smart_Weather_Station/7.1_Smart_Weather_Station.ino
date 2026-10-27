/**
 **************************************************
 *
 * @file        7.1_Smart_Weather_Station.ino
 * @brief       Project that brings together three things you have already learned separately: the SHTC3 temperature
 *              and humidity sensor from section 5, the LCD display from section 4, and the Wi-Fi connection from
 *              section 6. The board measures temperature and humidity, shows them on the LCD, and sends them to a
 *              webhook on the internet so you can follow the readings from anywhere.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
The WiFi library contains everything needed to join a Wi-Fi network.
*/
#include <WiFi.h>

/*
The HTTPClient library lets us speak HTTP, so we can send our readings to a server.
*/
#include <HTTPClient.h>

/*
The Soldered library for the SHTC3 temperature and humidity sensor.
*/
#include "SHTC3-SOLDERED.h"

/*
The Soldered library for the LCD display.
*/
#include "LCD-SOLDERED.h"

/*
These two variables hold the name of your Wi-Fi network (the SSID) and its password. Replace the text between the
quotation marks with your own network details.
*/
const char* ssid = "your ssid";
const char* password = "your password";

/*
This variable holds the address we send our readings to. Open https://webhook.site in a browser, copy the unique link
it shows you, and paste it between the quotation marks below. Keep that browser tab open and you will see every
reading appear in it.
*/
const char* webhookURL = "your unique url";

/*
Here we create our two objects: one for the sensor and one for the display. An object is our way of talking to a
device: every function we call on it, we call through its name.
The sensor needs no pin numbers because it uses easyC, which is Soldered's name for an I2C connection over a single
cable, and I2C always uses the same two pins on the board (IO6 and IO7 on the NULA board).
*/
SHTC3 shtc3;
LCD lcd(16, 2);

/*
These two variables hold the latest readings. They are decimal numbers, which is why they are floats.
*/
float temperature = 0.0;
float humidity = 0.0;

/*
This variable remembers the moment of the last reading, and the one below it defines how much time passes between two
readings, in milliseconds. 30000 milliseconds is thirty seconds. Feel free to experiment with this value, but keep in
mind that every reading is also sent over the internet.
*/
unsigned long lastUpdate = 0;
const unsigned long UPDATE_MS = 30000;

void setup() {

  /*
  Serial.begin() establishes serial communication between your board and your computer via a USB cable. In a project
  this size the Serial Monitor is our main tool for finding out what went wrong, so we start it first.
  The short delay after it gives the connection a moment to settle, so the first messages are not lost.
  */
  Serial.begin(115200);
  delay(500);

  /*
  Here we prepare the display. begin() starts the communication with it, backlight() turns on its light so the text is
  readable, and clear() wipes anything that was left on the screen from before.
  setCursor() then chooses where the next text will appear: the first number is the column and the second is the row,
  and both start counting at zero. So (0, 0) is the top left corner and (0, 1) is the start of the second line.
  We show a short greeting first, so we can tell at a glance that the display itself works.
  */
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Weather Station");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(1000);
  lcd.clear();

  /*
  begin() on the sensor starts the I2C communication and tells us whether the sensor answered, returning true on
  success and false on failure. The "!" in front means "not", so this reads as "if the sensor did not start".
  If the sensor is missing there is nothing left for this project to measure, so instead of continuing we print the
  problem and stop here. The while(1) loop below never ends, which is a simple way of saying "go no further".
  */
  if (!shtc3.begin()) {
    Serial.println("SHTC3 init failed!");
    lcd.print("SHTC3 error!");
    while (1) delay(100);
  }

  /*
  WiFi.begin() starts the connection attempt. The function only starts the process, it does not wait for it to finish,
  so we tell the user what is going on both on the Serial Monitor and on the display.
  */
  Serial.print("Connecting to Wi-Fi");
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  WiFi.begin(ssid, password);

  /*
  Here we wait for the connection ourselves. WiFi.status() tells us the current state and WL_CONNECTED is the value it
  reports once we are online.
  */
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(0, 1);
    lcd.print(".");
  }

  /*
  WiFi.localIP() returns the address the router handed out to our board.
  */
  Serial.println();
  Serial.println("Wi-Fi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  //Let the user know we are online, then clear the display so the readings start on an empty screen.
  lcd.clear();
  lcd.print("WiFi Connected!");
  delay(800);
  lcd.clear();

  Serial.println("Smart Weather Station ready!");
}

void loop() {

  /*
  millis() is a function that returns the number of milliseconds passed since the board began running the current
  program. We use it instead of delay() so the board stays free to do other work between readings.
  */
  unsigned long now = millis();

  /*
  Here we check how much time has passed since the last reading. Only when UPDATE_MS milliseconds have gone by do we
  take a new one, and we immediately remember the current time as the new starting point.
  */
  if (now - lastUpdate >= UPDATE_MS) {
    lastUpdate = now;

    /*
    sample() tells the sensor to perform a fresh measurement, and the two read functions then hand us the results.
    We have to call sample() first, otherwise we would keep getting the previous measurement.
    */
    shtc3.sample();
    temperature = shtc3.readTempC();
    humidity = shtc3.readHumidity();

    //Print the readings to the Serial Monitor. The number 2 tells the function how many decimal places to show.
    Serial.print("Temperature: ");
    Serial.print(temperature, 2);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity, 2);
    Serial.println(" %");

    /*
    Now we show the same readings on the display. We clear it first, because writing shorter text over longer text
    would leave leftover characters behind.
    The (char)223 is the character code the display uses for the degree symbol. The Serial Monitor and the LCD do not
    use the same character set, which is why we write the degree sign one way above and another way here.
    */
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature, 1);
    lcd.print((char)223);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(humidity, 1);
    lcd.print(" %");

    /*
    Before sending anything we check that we are still online. A Wi-Fi connection can drop at any time, and trying to
    send data without one would only waste time and print errors.
    */
    if (WiFi.status() == WL_CONNECTED) {

      /*
      Here we create our HTTP client object, prepare the request with begin(), and describe the format of our data
      with addHeader(). A header tells the server something about the request itself.
      */
      HTTPClient http;
      http.begin(webhookURL);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      /*
      Here we build the data we are going to send. The format "name=value&name=value" is the same one a browser uses
      when you submit a simple web form: the ampersand ("&") separates one value from the next, and String() converts
      our numbers into text so they can be joined to the rest with a plus sign.
      */
      String postData = "temperature=" + String(temperature, 2) + "&humidity=" + String(humidity, 2);

      /*
      POST() sends the request together with our data and waits for the answer. It returns a response code: numbers of
      200 and above mean the server answered, while a negative number means we never got an answer at all.
      */
      int httpCode = http.POST(postData);

      if (httpCode > 0) {
        Serial.print("POST successful! Response code: ");
        Serial.println(httpCode);
      } else {

        //errorToString() turns the negative error code into a short sentence that is easier to understand.
        Serial.print("POST failed. Error: ");
        Serial.println(http.errorToString(httpCode));
      }

      /*
      end() closes the connection and frees the memory the client was using. Always close a connection once you are
      done with it, otherwise a program that makes many requests will slowly run out of memory.
      */
      http.end();
    } else {

      //If we lost the connection, start a new attempt and try again on the next reading.
      Serial.println("Wi-Fi disconnected. Trying to reconnect...");
      WiFi.begin(ssid, password);
    }
  }
}
