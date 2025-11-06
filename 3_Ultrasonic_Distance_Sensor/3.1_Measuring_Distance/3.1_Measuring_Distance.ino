/**
 **************************************************
 *
 * @file        3.1_Measuring_Distance.ino
 * @brief       Example that shows how to measure distance using the Soldered Ultrasonic Distance Sensor.
 *              The NULA MINI board sends an ultrasonic pulse and measures how long it takes
 *              to bounce back from an object using the EasyC-compatible Soldered library.
 *              The measured distance is displayed in centimeters on the Serial Monitor.
 *              For wiring details, connection diagram, and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

#include "Ultrasonic-distance-sensor-easyC-SOLDERED.h"   // Include Soldered Ultrasonic Sensor library

// --- Pin definitions ---
#define TRIG_PIN  4     // Trigger pin for the ultrasonic sensor
#define ECHO_PIN  3     // Echo pin for the ultrasonic sensor

// --- Create ultrasonic sensor object ---
Ultrasonic_Sensor hc(TRIG_PIN, ECHO_PIN);

void setup() {
  Serial.begin(115200);   // Start Serial Monitor communication
  hc.begin();             // Initialize the ultrasonic sensor

  Serial.println("Ultrasonic Measuring Distance Example started!");
  Serial.println("Move an object in front of the sensor to see the distance change.");
}

void loop() {
  // --- Measure distance in centimeters ---
  float distance = hc.getDistance();

  // --- Print distance to Serial Monitor ---
  Serial.print("Distance: ");
  Serial.print(distance, 2);
  Serial.println(" cm");

  delay(500);  // Short delay between readings
}
