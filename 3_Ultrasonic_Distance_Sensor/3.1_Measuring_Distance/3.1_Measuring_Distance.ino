/**
 **************************************************
 *
 * @file        3.1_Measuring_Distance.ino
 * @brief       Example that shows how to measure distance using the Soldered Ultrasonic Distance Sensor.
 *              The sensor sends out a short ultrasonic pulse and then listens for its echo. It measures how long the
 *              echo took to come back, works out the distance from that time, and hands us the answer over easyC.
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
Here we create our sensor object, which we named "hc". An object is our way of talking to the sensor: every function
we call on it, we call through this name.
Notice that we do not pass any pin numbers. This sensor connects over easyC, which is Soldered's name for an I2C
connection over a single cable, and I2C always uses the same two pins on the board (IO6 and IO7 on the NULA board),
so the library already knows where to find it. Plug the Qwiic cable in and there is nothing to wire.
*/
Ultrasonic_Sensor hc;

/*
This is how long we wait after asking for a measurement, in milliseconds. The sensor needs a moment to send its pulse
out and listen for the echo, and it gives up after 38 milliseconds if no echo comes back. We wait a little longer than
that so the answer is always ready by the time we ask for it.
*/
const int MEASURE_WAIT_MS = 50;

void setup() {

  /*
  Serial.begin() establishes serial communication between your board and another device, in this example, to your
  computer via a USB cable. We use it here so we can print the measured distance on the Serial Monitor.
  */
  Serial.begin(115200);

  /*
  begin() prepares the sensor for use. For an easyC sensor this starts the I2C communication and tells the library which
  address to talk to, which for this sensor is 0x30. A sensor should always be initialized before we start reading
  from it.
  */
  hc.begin();

  //Print out the initial message so we know that the program started successfully.
  Serial.println("Ultrasonic Measuring Distance Example started!");
  Serial.println("Move an object in front of the sensor to see the distance change.");
}

void loop() {

  /*
  Reading an easyC sensor takes two steps. takeMeasure() is the first one: it asks the sensor to send out a pulse and
  time the echo. The sensor does that work on its own and remembers the answer, which is why we then have to wait
  before collecting it.
  */
  hc.takeMeasure();
  delay(MEASURE_WAIT_MS);

  /*
  getDistance() is the second step: it fetches the answer the sensor worked out and stored, already converted into
  centimeters. The value comes back as a whole number of centimeters, which is all the accuracy this sensor can
  honestly offer, so we store it in an int rather than in a decimal number.
  */
  int distance = hc.getDistance();

  /*
  The sensor answers with 0 when it heard no echo at all, which happens when nothing is in range or when the surface in
  front of it scatters the sound away. That is not a real measurement, so we say so rather than reporting a distance of
  zero centimeters, which would suggest an object touching the sensor.
  */
  if (distance == 0) {
    Serial.println("No echo received, nothing in range.");
  } else {

    /*
    Serial.print() prints data to the Serial Monitor without skipping to the next line, while Serial.println() prints
    the data and then skips to the next line. By combining them we can build one readable line out of several pieces.
    */
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  /*
  delay() is a function that starts a pause in the code. Without this pause the readings would scroll by far too
  quickly to read, and it also lets the echoes of the last pulse die out before we ask for the next one.
  Feel free to experiment with this value.
  */
  delay(500);
}
