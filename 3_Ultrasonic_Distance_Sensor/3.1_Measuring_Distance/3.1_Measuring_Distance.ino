/**
 **************************************************
 *
 * @file        3.1_Measuring_Distance.ino
 * @brief       Example that shows how to measure distance using the Soldered Ultrasonic Distance Sensor.
 *              The sensor sends out a short ultrasonic pulse and then listens for its echo. The board measures
 *              how long the echo took to come back and calculates the distance from that time.
 *              The measured distance is printed in centimeters to the Serial Monitor.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
Including a library gives us access to ready-made functions that do the hard work for us. Here we include the
Soldered library for the Ultrasonic Distance Sensor, so we don't have to calculate the distance from raw timings
ourselves. If the Arduino IDE reports that this file cannot be found, install the library through
Sketch -> Include Library -> Manage Libraries and search for "Soldered Ultrasonic".
*/
#include "Ultrasonic-distance-sensor-easyC-SOLDERED.h"

/*
This is a variable to which we pass the number of pin that we had connected the sensor's TRIG pin to. TRIG is short
for "trigger", this is the pin the board uses to tell the sensor to send out an ultrasonic pulse.
The NULA board has a pin naming logic as follows: IO4, where 4 is the number that we give to the variable.
If you wish to use a different pin, make sure you are using a IO__ marked pin.
*/
const int TRIG_PIN = 4;

/*
This is a variable to which we pass the number of pin that we had connected the sensor's ECHO pin to. ECHO is the
pin on which the sensor reports back how long it took for the pulse to return.
*/
const int ECHO_PIN = 3;

/*
Here we create our sensor object, which we named "hc". An object is our way of talking to the sensor: every function
we call on it, we call through this name. We pass it the two pin numbers we defined above, in the order TRIG first
and ECHO second, so the library knows how the sensor is wired to the board.
*/
Ultrasonic_Sensor hc(TRIG_PIN, ECHO_PIN);

void setup() {

  /*
  Serial.begin() establishes serial communication between your board and another device, in this example, to your
  computer via a USB cable. We use it here so we can print the measured distance on the Serial Monitor.
  */
  Serial.begin(115200);

  /*
  begin() prepares the sensor for use. The library sets the TRIG pin as an output and the ECHO pin as an input for us.
  A sensor should always be initialized before we start reading from it.
  */
  hc.begin();

  //Print out the initial message so we know that the program started successfully.
  Serial.println("Ultrasonic Measuring Distance Example started!");
  Serial.println("Move an object in front of the sensor to see the distance change.");
}

void loop() {

  /*
  getDistance() is a function that makes the sensor take one measurement and returns the result in centimeters.
  Behind the scenes the library sends the pulse on the TRIG pin, times the echo on the ECHO pin, and converts that
  time into a distance using the speed of sound.
  */
  float distance = hc.getDistance();

  /*
  Serial.print() prints data to the Serial Monitor without skipping to the next line, while Serial.println() prints
  the data and then skips to the next line. By combining them we can build one readable line out of several pieces.
  */
  Serial.print("Distance: ");
  Serial.print(distance, 2);
  Serial.println(" cm");

  /*
  delay() is a function that starts a pause in the code. Without this pause the readings would scroll by far too
  quickly to read. Feel free to experiment with this value.
  */
  delay(500);
}
