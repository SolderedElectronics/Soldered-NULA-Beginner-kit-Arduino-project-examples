#include "LCD-SOLDERED.h"

LCD lcd(16,2);

const int BUTTON_PIN = 3;

void setup() {
  pinMode(BUTTON_PIN, INPUT); 
  lcd.begin();     // Initialize LCD
  lcd.backlight(); // Turn the backlight on (if supported)
}

void loop() {
  bool reading = digitalRead(BUTTON_PIN);

  lcd.clear();  // clear display each loop before writing

  if (reading == HIGH) {
    lcd.setCursor(0,0);
    lcd.print(F("Button pressed!"));
    
  } else {
    lcd.setCursor(0,0);
    lcd.clear();

  }

  delay(50); // small delay to reduce flicker
}
