/**
 **************************************************
 *
 * @file        5.1_Reading_Temperature_and_Humidity.ino
 * @brief       Example that shows how to measure temperature and humidity using
 *              the Soldered SHTC3 sensor. The values are printed to the Serial Monitor.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

#include "SHTC3-SOLDERED.h"    // Include the Soldered SHTC3 sensor library

// --- Create an SHTC3 sensor object ---
SHTC3 shtcSensor;

// --- Update interval (in milliseconds) ---
const unsigned long UPDATE_MS = 2000;  // Time between sensor reads
unsigned long lastUpdate = 0;          // Stores last update time

void setup() {
  /*
  Start the Serial Monitor to show temperature and humidity readings.
  */
  Serial.begin(115200);

  /*
  Initialize the SHTC3 sensor.
  If initialization fails, we print an error message.
  */
  if (!shtcSensor.begin()) {
    Serial.println("SHTC3 initialization failed!");
  } else {
    Serial.println("SHTC3 sensor ready!");
  }
}

void loop() {
  /*
  millis() returns the number of milliseconds since the program started.
  Here we use it to update the readings every UPDATE_MS milliseconds.
  */
  unsigned long now = millis();
  if (now - lastUpdate >= UPDATE_MS) {
    lastUpdate = now;

    /*
    Take a new measurement from the SHTC3 sensor.
    */
    shtcSensor.sample();

    /*
    Read temperature (in °C) and humidity (in %RH).
    */
    float temperature = shtcSensor.readTempC();
    float humidity = shtcSensor.readHumidity();

    /*
    Print the results to the Serial Monitor.
    */
    Serial.print("Temperature: ");
    Serial.print(temperature, 2);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity, 2);
    Serial.println(" %");
  }
}
