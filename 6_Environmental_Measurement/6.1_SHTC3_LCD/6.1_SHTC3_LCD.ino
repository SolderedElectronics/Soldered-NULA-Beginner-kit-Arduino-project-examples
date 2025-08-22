/**
 **************************************************
 *
 * @file        SHTC3_LCD_Display.ino
 * @brief       Example that shows how to measure temperature and humidity with the SHTC3 sensor
 *              and display the values both on the Serial Monitor and on a 16x2 LCD.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************/

#include "SHTC3-SOLDERED.h"    // Include the Soldered SHTC3 sensor library
#include "LCD-SOLDERED.h"      // Include the Soldered LCD library

// --- Sensor & LCD objects ---
SHTC3 shtcSensor;              // Create an SHTC3 sensor object
LCD lcd(16, 2);                // Create a 16x2 LCD object

// --- Update interval (in milliseconds) ---
const unsigned long UPDATE_MS = 2000;  // Time between sensor reads
unsigned long lastUpdate = 0;          // Stores the last time we updated

void setup() {
  /*
  Start I2C communication. Both the SHTC3 sensor and the LCD use I2C.
  */
  Wire.begin();                

  /*
  Start the Serial Monitor, used here for debugging and to show readings on the PC.
  */
  Serial.begin(115200);         

  /*
  Initialize the SHTC3 sensor. 
  If initialization fails, we print a message to the Serial Monitor.
  */
  if (!shtcSensor.begin()) {    
    Serial.println("SHTC3 init failed!");
  }

  /*
  Initialize the LCD, turn on the backlight, and show a starting message.
  */
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SHTC3 + LCD");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(800);       // Brief pause for the startup message
  lcd.clear();      // Clear display before main loop begins
}

void loop() {
  /*
  millis() returns the number of milliseconds since the program started. 
  Here we use it to update the sensor and LCD every UPDATE_MS milliseconds.
  */
  unsigned long now = millis();
  if (now - lastUpdate >= UPDATE_MS) {
    lastUpdate = now;

    /*
    Take a new measurement from the sensor.
    */
    shtcSensor.sample();

    /*
    Read temperature (in °C) and humidity (in %).
    */
    float tC  = shtcSensor.readTempC();
    float hum = shtcSensor.readHumidity();

    // --- Serial output ---
    Serial.print("Temp: ");
    Serial.print(tC, 2);       // Print with 2 decimals
    Serial.print(" C, Hum: ");
    Serial.print(hum, 2);      // Print with 2 decimals
    Serial.println(" %");

    // --- LCD output ---
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(tC, 1);          // Print with 1 decimal (fits better)
    lcd.print(" C   ");        // Extra spaces clear leftover characters

    lcd.setCursor(0, 1);
    lcd.print("Hum:  ");
    lcd.print(hum, 1);
    lcd.print(" %   ");
  }
}
