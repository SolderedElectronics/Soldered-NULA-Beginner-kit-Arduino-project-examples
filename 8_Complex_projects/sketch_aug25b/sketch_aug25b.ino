#include "LCD-SOLDERED.h"

LCD lcd(16, 2);

// --- Pins (external pulldown resistors used) ---
const int BTN_UP   = 10;   
const int BTN_DOWN = 11;   

// --- Debounce config ---
const unsigned long debounceMs = 25;

// --- Debounced states (your names) ---
int buttonUpState = LOW;       // debounced state: LOW idle, HIGH when pressed
int buttonDownState = LOW;

// --- Raw states + per-button timers (added for reliability) ---
int lastRawUp = LOW;
int lastRawDown = LOW;
unsigned long lastChangeUpMs = 0;
unsigned long lastChangeDownMs = 0;

// Keeping your original name defined to avoid compile errors if referenced elsewhere
unsigned long lastChangeMs = 0;  // not used in the loop; per-button timers are used


const char row1[]={' ', ' ', '#'};
const char row2[]={' ', '#', ' '};

struct Display{
  
}
void setup() {
  pinMode(BTN_UP, INPUT);    // using external pulldown resistors
  pinMode(BTN_DOWN, INPUT);

  lcd.begin();
  lcd.clear();
  lcd.print("Ready");
}

void loop() {
  unsigned long now = millis();

  int rawUp = digitalRead(BTN_UP);
  if (rawUp != lastRawUp) {
    lastChangeUpMs = now;       
    lastRawUp = rawUp;
  }

  if ((now - lastChangeUpMs) > debounceMs && rawUp != buttonUpState) {
    buttonUpState = rawUp;      
    if (buttonUpState == HIGH) {
      lcd.clear();
      lcd.print("UP pressed");
      Serial.println("up");
      
    }
  }

  int rawDown = digitalRead(BTN_DOWN);
  if (rawDown != lastRawDown) {
    lastChangeDownMs = now;
    lastRawDown = rawDown;
  }

  if ((now - lastChangeDownMs) > debounceMs && rawDown != buttonDownState) {
    buttonDownState = rawDown;
    if (buttonDownState == HIGH) {
      lcd.clear();
      lcd.print("DOWN pressed");
      Serial.println("down");
      
    }
  }
}
