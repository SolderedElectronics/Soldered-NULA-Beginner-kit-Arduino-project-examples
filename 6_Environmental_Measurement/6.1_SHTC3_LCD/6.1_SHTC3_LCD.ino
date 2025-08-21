#include <Wire.h>
#include "SHTC3-SOLDERED.h"
#include "LCD-SOLDERED.h"

// --- Sensor & LCD objects ---
SHTC3 shtcSensor;
LCD lcd(16, 2);   // 16x2 LCD

// --- Update interval (ms) ---
const unsigned long UPDATE_MS = 2000;
unsigned long lastUpdate = 0;

void setup() {
  Wire.begin();                 // I2C
  Serial.begin(115200);         // Serial monitor

  // Init SHTC3
  if (!shtcSensor.begin()) {    // if your library's begin() returns bool; if not, just call it
    Serial.println("SHTC3 init failed!");
  }

  // Init LCD
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("SHTC3 + LCD"));
  lcd.setCursor(0, 1);
  lcd.print(F("Starting..."));
  delay(800);
  lcd.clear();
}

void loop() {
  unsigned long now = millis();
  if (now - lastUpdate >= UPDATE_MS) {
    lastUpdate = now;

    // Take a measurement
    shtcSensor.sample();

    float tC  = shtcSensor.readTempC();
    float hum = shtcSensor.readHumidity();

    // --- Serial output ---
    Serial.print("Temp: ");
    Serial.print(tC, 2);
    Serial.print(" C, Hum: ");
    Serial.print(hum, 2);
    Serial.println(" %");

    // --- LCD output (no flicker) ---
    lcd.setCursor(0, 0);
    lcd.print(("Temp: "));
    lcd.print(tC, 1);      // one decimal fits nicely on 16 chars
    lcd.print((" C   ")); // pad spaces to clear leftovers

    lcd.setCursor(0, 1);
    lcd.print(("Hum:  "));
    lcd.print(hum, 1);
    lcd.print((" %   "));
  }

}
