/**
 **************************************************
 *
 * @file        3.2_Distance_Fade_LED.ino
 * @brief       Example that shows how to control the brightness of an LED based on the distance measured by the
 *              Soldered Ultrasonic Distance Sensor. The closer an object is to the sensor, the brighter the LED
 *              becomes. This example introduces analogWrite() and map(), two functions that let us turn a measured
 *              value into a brightness level.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
Include the Soldered library for the Ultrasonic Distance Sensor, so we can read the distance with a single function
call instead of timing the echo ourselves.
*/
#include "Ultrasonic-distance-sensor-easyC-SOLDERED.h"

/*
Here we create our sensor object, which we named "hc". The sensor connects over easyC, which is Soldered's name for an
I2C connection over a single cable, so we pass no pin numbers: I2C always uses the same two pins on the board (IO6 and
IO7 on the NULA board) and the library already knows to look there.
*/
Ultrasonic_Sensor hc;

/*
This is a variable to which we pass the number of pin that we had connected the LED to. Because we want to dim this
LED and not only switch it on and off, we have to pick a pin that supports PWM. PWM is explained further down in
this example.
The NULA board has a pin naming logic as follows: IO2, where 2 is the number that we give to the variable.
If you wish to use a different pin, make sure you are using a IO__ marked pin.

Remember that the LED needs a 330 Ohm resistor in series with it. That resistor limits how much current flows, and
without it the LED draws more than either it or the pin is built for, so both can be damaged.
*/
const int LED_PIN = 2;

/*
These two variables define the distance range we care about, in centimeters. Anything closer than the minimum counts
as "as close as possible" and anything further than the maximum counts as "far away". Feel free to experiment with
these values.
*/
const int MIN_DISTANCE = 2;
const int MAX_DISTANCE = 50;

/*
This is how long we wait after asking for a measurement, in milliseconds. The sensor gives up listening for an echo
after 38 milliseconds, so waiting a little longer than that means the answer is always ready when we ask for it.
*/
const int MEASURE_WAIT_MS = 50;

void setup() {

  /*
  Serial.begin() establishes serial communication between your board and your computer via a USB cable. We use it
  here so we can watch both the distance and the brightness on the Serial Monitor.
  */
  Serial.begin(115200);

  /*
  pinMode() is a function that configures the specified pin to behave either as an input or in this case as an output.
  As our pin needs to light up the LED, we will put the pin in OUTPUT mode.
  */
  pinMode(LED_PIN, OUTPUT);

  /*
  analogWriteResolution() defines how many bits are used for the brightness values we write with analogWrite().
  By default the board expects 8 bits, meaning values from 0 to 255, and anything larger is simply refused. Because we
  want the finer control of 12 bits, from 0 to 4095, we have to say so here, otherwise the LED would not dim at all.
  */
  analogWriteResolution(LED_PIN, 12);

  /*
  begin() prepares the sensor for use, starting the I2C communication and telling the library which address to talk to.
  */
  hc.begin();

  //Print out the initial message so we know that the program started successfully.
  Serial.println("Distance Fade LED Example started!");
}

void loop() {

  /*
  Reading an easyC sensor takes two steps. takeMeasure() asks the sensor to send out a pulse and time the echo, and the
  wait afterwards gives it the moment it needs to finish that work and store the answer.
  */
  hc.takeMeasure();
  delay(MEASURE_WAIT_MS);

  /*
  getDistance() then fetches the stored answer, already converted into centimeters. The value comes back as a whole
  number of centimeters, which is all the accuracy this sensor can honestly offer.
  */
  int distance = hc.getDistance();

  /*
  The sensor answers with 0 when it heard no echo at all, which happens when nothing is in range or when the surface in
  front of it scatters the sound away. That is not a real measurement, so we skip the rest of this pass instead of
  treating it as an object pressed right up against the sensor.
  return ends this pass through loop() early, and because the board calls loop() again immediately we are straight back
  at the next measurement.
  */
  if (distance == 0) {
    Serial.println("No echo received, nothing in range.");
    return;
  }

  //Print the measured distance to the Serial Monitor.
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm");

  /*
  Here we keep the measured distance inside the range we defined above. This is called clamping, and we do it because
  the next step expects a value inside a known range. Without it, an object 300 cm away would give us a brightness
  value far outside anything the LED can use.
  */
  if (distance < MIN_DISTANCE) distance = MIN_DISTANCE;
  if (distance > MAX_DISTANCE) distance = MAX_DISTANCE;

  /*
  map() is a function that takes a number from one range and rescales it into another range. Here we take the
  distance, which goes from MIN_DISTANCE to MAX_DISTANCE, and rescale it into a brightness, which goes from 4095 down
  to 0. Notice that the output range is reversed: the smallest distance gives the largest brightness, which is
  exactly the effect we want.
  */
  int brightness = map(distance, MIN_DISTANCE, MAX_DISTANCE, 4095, 0);

  /*
  analogWrite() is a function that writes an "in between" value to a pin instead of only HIGH or LOW. It does this
  using PWM, which stands for Pulse Width Modulation: the pin is switched on and off very quickly, and the longer it
  stays on during each cycle, the brighter the LED looks to our eyes.
  */
  analogWrite(LED_PIN, brightness);

  //Print the brightness value as well, so we can see how it changes together with the distance.
  Serial.print(" -> Brightness: ");
  Serial.println(brightness);
}
