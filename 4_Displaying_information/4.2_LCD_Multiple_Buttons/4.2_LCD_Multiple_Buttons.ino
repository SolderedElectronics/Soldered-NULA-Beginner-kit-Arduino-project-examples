#include "LCD-SOLDERED.h"

LCD lcd(16, 2);   // 16x2 LCD

// Define button pins
const int BTN1 = 3;  // Shows a message while pressed
const int BTN2 = 4;  // Counter
const int BTN3 = 5;  // Progress bar animation

int counter = 0;             // counter value for button 2
bool lastBtn2State = LOW;    // remember last state for button 2

void setup() {
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(BTN3, INPUT);

  lcd.begin();     // Initialize LCD
  // lcd.backlight(); // Uncomment if your display has backlight support
}

void loop() {
  // Read all three buttons
  bool btn1State = digitalRead(BTN1);
  bool btn2State = digitalRead(BTN2);
  bool btn3State = digitalRead(BTN3);

  lcd.clear();   // Clear screen each loop

  // --- Button 3 has the highest priority (progress bar) ---
  if (btn3State == HIGH) {
    static int pos = 0;      // position of the bar
    lcd.setCursor(0, 0);
    lcd.print("Loading...");
    lcd.setCursor(0, 1);

    // Draw a progress bar across the second line
    for (int i = 0; i < pos; i++) {
      lcd.print("#");
    }

    pos++;
    if (pos > 16) pos = 0;   // loop back
    delay(150);              // controls animation speed
  }

  // --- Button 2 shows counter ---
  else if (btn2State == HIGH) {
    // Only increase counter when button goes from LOW to HIGH
    if (lastBtn2State == LOW) {
      counter++;
    }
    lcd.setCursor(0, 0);
    lcd.print("Counter:");
    lcd.setCursor(0, 1);
    lcd.print(counter);
    delay(200);  // debounce delay
  }

  // --- Button 1 shows message while pressed ---
  else if (btn1State == HIGH) {
    lcd.setCursor(0, 0);
    lcd.print("Button 1");
    lcd.setCursor(0, 1);
    lcd.print("is pressed!");
  }

  // --- Nothing pressed ---
  else {
    lcd.setCursor(0, 0);
    lcd.print("Press a button");
  }

  // Remember last state of button 2
  lastBtn2State = btn2State;
  delay(50);
}
