/**
 **************************************************
 *
 * @file        5.1_Reading_Temperature_and_Humidity.ino
 * @brief       Example that shows how to measure temperature and humidity using the Soldered SHTC3 sensor.
 *              The sensor communicates with the board over easyC, so only one cable is needed and no pins have to
 *              be defined. The measured values are printed to the Serial Monitor.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
Including a library gives us access to ready-made functions that do the hard work for us. Here we include the
Soldered library for the SHTC3 sensor. If the Arduino IDE reports that this file cannot be found, install the library
through Sketch -> Include Library -> Manage Libraries and search for "Soldered SHTC3".
*/
#include "SHTC3-SOLDERED.h"

/*
Here we create our sensor object, which we named "shtcSensor". An object is our way of talking to the sensor: every
function we call on it, we call through this name. Notice that we do not pass any pin numbers this time. The SHTC3
uses easyC, which is Soldered's name for an I2C connection over a single cable, and I2C always uses the same two
pins on the board (IO6 and IO7 on the NULA board), so the library already knows where to find the sensor.
*/
SHTC3 shtcSensor;

/*
This variable defines how much time passes between two measurements, in milliseconds. 2000 milliseconds is two
seconds. Feel free to experiment with this value.
*/
const unsigned long UPDATE_MS = 2000;

/*
This variable remembers the moment when we took the last measurement. We compare it against the current time to know
when the next measurement is due. It has to be an unsigned long because the numbers the clock gives us grow large.
*/
unsigned long lastUpdate = 0;

void setup() {

  /*
  Serial.begin() establishes serial communication between your board and your computer via a USB cable. We use it
  here so we can display the temperature and humidity readings on the Serial Monitor.
  */
  Serial.begin(115200);

  /*
  begin() prepares the sensor for use and starts the I2C communication. It also tells us whether the sensor answered:
  the function returns true on success and false on failure. We check the result and print a message either way, so
  that if nothing shows up later we know whether the problem is the cable or the code.
  The "!" in front of the call means "not", so this if statement reads as "if the sensor did not start".
  */
  if (!shtcSensor.begin()) {
    Serial.println("SHTC3 initialization failed!");
  } else {
    Serial.println("SHTC3 sensor ready!");
  }
}

void loop() {

  /*
  millis() is a function that returns the number of milliseconds passed since the board began running the current
  program. We use it instead of delay() so that the board stays free to do other work between measurements, which
  matters as soon as your program has more than one job. This number will overflow (go back to zero) after
  approximately 50 days.
  */
  unsigned long now = millis();

  /*
  Here we check how much time has passed since the last measurement. Only when UPDATE_MS milliseconds have gone by do
  we take a new reading, and we immediately remember the current time as the new starting point.
  */
  if (now - lastUpdate >= UPDATE_MS) {
    lastUpdate = now;

    /*
    sample() tells the sensor to perform a fresh measurement and store the result inside itself. We have to call it
    before reading the values, otherwise we would keep getting the result of the previous measurement.
    */
    shtcSensor.sample();

    /*
    readTempC() returns the temperature from the last measurement in degrees Celsius, and readHumidity() returns the
    relative humidity in percent. Both are decimal numbers, which is why we store them in float variables.
    */
    float temperature = shtcSensor.readTempC();
    float humidity = shtcSensor.readHumidity();

    /*
    Serial.print() prints data without skipping to the next line, while Serial.println() prints the data and then
    skips to the next line. By combining them we build one readable line out of several pieces. The number 2 after a
    value tells the function how many decimal places to show.
    */
    Serial.print("Temperature: ");
    Serial.print(temperature, 2);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity, 2);
    Serial.println(" %");
  }
}
