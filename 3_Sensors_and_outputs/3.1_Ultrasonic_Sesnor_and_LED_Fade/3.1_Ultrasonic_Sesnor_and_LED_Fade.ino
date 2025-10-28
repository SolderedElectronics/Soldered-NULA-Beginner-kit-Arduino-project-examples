/**
 **************************************************
 *
 * @file        Ultrasonic_LED_brightness.ino
 * @brief       Example showing how to use the HC-SR04 ultrasonic sensor
 *              with an LED to create an automatic distance-based brightness
 *              control system. The LED becomes brighter as an object gets
 *              closer to the sensor, and dimmer as it moves away.
 * 
 * @author      Soldered
 ***************************************************
 */

#include "Ultrasonic-distance-sensor-easyC-SOLDERED.h"   // Include Soldered ultrasonic sensor library

// --- Pin definitions ---
#define TRIGPIN     4     // Trigger pin for the HC-SR04
#define ECHOPIN     3     // Echo pin for the HC-SR04
#define LEDPIN      2     // PWM-capable LED pin (can be changed if needed)

// --- Create ultrasonic sensor object ---
Ultrasonic_Sensor hc(TRIGPIN, ECHOPIN);

// --- LED brightness parameters ---
const int MIN_DISTANCE = 2;      // Minimum distance to consider (in cm)
const int MAX_DISTANCE = 50;     // Maximum distance to consider (in cm)
const int MIN_BRIGHTNESS = 0;    // Minimum LED brightness (0 = off)
const int MAX_BRIGHTNESS = 255;  // Maximum LED brightness (255 = full brightness)

void setup()
{
  Serial.begin(115200);   // Start serial communication with PC via UART
  hc.begin();             // Initialize ultrasonic sensor

  pinMode(LEDPIN, OUTPUT); // Set LED pin as output
}

void loop()
{
  // --- Measure distance from the sensor ---
  float distance = hc.getDistance();   // Distance in centimeters
  long duration  = hc.getDuration();   // Echo round-trip time in microseconds (optional info)

  // --- Print distance readings to Serial Monitor ---
  Serial.print("Distance from obstacle: ");
  Serial.print(distance);
  Serial.println(" cm");

  // --- Clamp distance within a valid range ---
  if (distance < MIN_DISTANCE) distance = MIN_DISTANCE;
  if (distance > MAX_DISTANCE) distance = MAX_DISTANCE;

  // --- Map distance to LED brightness ---
  // Closer object = brighter LED, farther object = dimmer LED
  int brightness = map(distance, MAX_DISTANCE, MIN_DISTANCE, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  brightness = constrain(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);

  // --- Apply brightness level to LED ---
  analogWrite(LEDPIN, brightness);   // PWM signal to control brightness

  // --- Print brightness info for debugging ---
  Serial.print("LED Brightness Level: ");
  Serial.println(brightness);

  delay(150);  // Small delay for stability and readability
}
