/**
 **************************************************
 *
 * @file        3.2_Distance_Fade_LED.ino
 * @brief       Example that shows how to control LED brightness based on the measured distance
 *              using the Soldered Ultrasonic Distance Sensor EasyC library.
 *              The closer the object, the brighter the LED becomes.
 *              For wiring details, connection diagram, and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

#include "Ultrasonic-distance-sensor-easyC-SOLDERED.h"  // Include Soldered Ultrasonic Sensor library

// --- Pin definitions ---
#define TRIG_PIN  4     // Trigger pin for the sensor
#define ECHO_PIN  3     // Echo pin for the sensor
#define LED_PIN   2     // LED connected to IO2 (PWM-capable pin)

// --- Create ultrasonic sensor object ---
Ultrasonic_Sensor hc(TRIG_PIN, ECHO_PIN);

void setup() {
  Serial.begin(115200);  // Start Serial communication
  pinMode(LED_PIN, OUTPUT);

  hc.begin();  // Initialize ultrasonic sensor

  Serial.println("Distance Fade LED Example started!");
}

void loop() {
  // --- Measure distance ---
  float distance = hc.getDistance();  // Distance in centimeters
  long duration  = hc.getDuration();  // Echo round-trip time in microseconds (for info)

  // --- Print distance to Serial Monitor ---
  Serial.print("Distance: ");
  Serial.print(distance, 1);
  Serial.print(" cm");

  // --- Limit distance range to useful values ---
  if (distance < 2) distance = 2;
  if (distance > 50) distance = 50;

  // --- Map distance to LED brightness ---
  // Closer object = higher brightness
  int brightness = map(distance, 2, 50, 4095, 0);
  analogWrite(LED_PIN, brightness);

  // --- Print brightness level ---
  Serial.print(" -> Brightness: ");
  Serial.println(brightness);

  delay(100);
}
