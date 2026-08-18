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
This is a variable to which we pass the number of pin that we had connected the sensor's TRIG pin to, the pin the
board uses to tell the sensor to send out an ultrasonic pulse.
The NULA board has a pin naming logic as follows: IO4, where 4 is the number that we give to the variable.
If you wish to use a different pin, make sure you are using a IO__ marked pin.
*/
const int TRIG_PIN = 4;

/*
This is a variable to which we pass the number of pin that we had connected the sensor's ECHO pin to, the pin on
which the sensor reports back how long it took for the pulse to return.
*/
const int ECHO_PIN = 3;

/*
This is a variable to which we pass the number of pin that we had connected the LED to. Because we want to dim this
LED and not only switch it on and off, we have to pick a pin that supports PWM. PWM is explained further down in
this example.
*/
const int LED_PIN = 2;

/*
Here we create our sensor object, which we named "hc". We pass it the two pin numbers we defined above, in the order
TRIG first and ECHO second, so the library knows how the sensor is wired to the board.
*/
Ultrasonic_Sensor hc(TRIG_PIN, ECHO_PIN);

/*
These two variables define the distance range we care about, in centimeters. Anything closer than the minimum counts
as "as close as possible" and anything further than the maximum counts as "far away". Feel free to experiment with
these values.
*/
const int MIN_DISTANCE = 2;
const int MAX_DISTANCE = 50;

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
  begin() prepares the sensor for use, setting the TRIG pin as an output and the ECHO pin as an input for us.
  */
  hc.begin();

  //Print out the initial message so we know that the program started successfully.
  Serial.println("Distance Fade LED Example started!");
}

void loop() {

  /*
  getDistance() is a function that makes the sensor take one measurement and returns the result in centimeters.
  */
  float distance = hc.getDistance();

  /*
  getDuration() returns the raw travel time of the echo in microseconds, before it is converted into a distance.
  We only read it here so you can see the number the sensor actually gives us. Keep in mind that calling it sends
  out a second ultrasonic pulse, so if you don't need this value you can safely delete this line.
  */
  long duration = hc.getDuration();

  //Print the measured distance to the Serial Monitor.
  Serial.print("Distance: ");
  Serial.print(distance, 1);
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

  /*
  A short pause between measurements. Keeping it small makes the fading look smooth.
  */
  delay(100);
}
