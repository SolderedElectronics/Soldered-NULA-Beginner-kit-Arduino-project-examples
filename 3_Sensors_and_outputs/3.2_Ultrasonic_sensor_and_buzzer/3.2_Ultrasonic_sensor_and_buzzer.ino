/**
 **************************************************
 *
 * @file        3.2_Ultrasonic_sensor_and_buzzer.ino
 * @brief       Example showing how to use the HC-SR04 ultrasonic sensor
 *              with a passive buzzer to create a simple reverse-parking
 *              warning system. The buzzer beeps faster as the object
 *              gets closer to the sensor.
 * @author      Soldered
 ***************************************************
 */

#include "Ultrasonic-distance-sensor-easyC-SOLDERED.h"   // Include Soldered ultrasonic sensor library

// --- Pin definitions ---
#define TRIGPIN     4       // Trigger pin for the HC-SR04
#define ECHOPIN     3       // Echo pin for the HC-SR04
#define BUZZER_PIN  2       // Passive buzzer pin

// --- Create ultrasonic sensor object ---
Ultrasonic_Sensor hc(TRIGPIN, ECHOPIN);

// --- Buzzer parameters ---
const int TONE_FREQ = 2700;     // Frequency in Hz (try 2–4 kHz for loudest result)

// --- Timing variables ---
unsigned long lastBeep = 0;     // Time of the last beep toggle
unsigned long beepInterval = 0; // How long between beeps
bool buzzerOn = false;          // Is the buzzer currently on?

void setup()
{
  Serial.begin(115200);   // Start serial communication
  hc.begin();             // Initialize ultrasonic sensor
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.println("Passive-buzzer reverse sensor started");
}

void loop()
{
  // --- Measure distance ---
  float distance = hc.getDistance();   // Distance in centimeters
  long duration  = hc.getDuration();   // Echo round-trip time in microseconds

  // --- Print readings to Serial Monitor ---
  Serial.print("Distance from obstacle: ");
  Serial.print(distance);
  Serial.println(" cm");

  // --- Decide buzzer behaviour based on distance ---
  if (distance > 100) {
    // Far away: no sound
    beepInterval = 0;
    noTone(BUZZER_PIN);
  }
  else if (distance > 60) {
    // 60–100 cm → slow beeps
    beepInterval = 800;
  }
  else if (distance > 30) {
    // 30–60 cm → medium beeps
    beepInterval = 400;
  }
  else if (distance > 10) {
    // 10–30 cm → fast beeps
    beepInterval = 150;
  }
  else {
    // Closer than 10 cm → continuous tone
    tone(BUZZER_PIN, TONE_FREQ);
    beepInterval = 0;
  }

  // --- Handle timed beeping ---
  if (beepInterval > 0) {
    unsigned long now = millis();
    if (now - lastBeep >= beepInterval) {
      lastBeep = now;
      buzzerOn = !buzzerOn;                 // Toggle the buzzer state
      if (buzzerOn) tone(BUZZER_PIN, TONE_FREQ);
      else          noTone(BUZZER_PIN);
    }
  }

  delay(50);  // Small delay for stability
}
