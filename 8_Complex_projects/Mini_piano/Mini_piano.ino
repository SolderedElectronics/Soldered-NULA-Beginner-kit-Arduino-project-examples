/**
 **************************************************
 *
 * @file        7.2_Buzzer_Alarm.ino
 * @brief       Example that plays a funky melody on the buzzer when a button is pressed. 
 *              The melody is defined as a sequence of notes and durations. 
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************/

#define BUZZER_PIN 18

// Button pins
#define BTN1 2
#define BTN2 3
#define BTN3 4
#define BTN4 5

// Frequencies for each button (in Hz)
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349

void setup() {
  // Initialize button pins as inputs with internal pull-up resistors
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(BTN3, INPUT);
  pinMode(BTN4, INPUT);

  // Initialize buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Check buttons (active LOW)
  if (digitalRead(BTN1) == HIGH) {
    tone(BUZZER_PIN, NOTE_C4);
  } 
  else if (digitalRead(BTN2) == HIGH) {
    tone(BUZZER_PIN, NOTE_D4);
  } 
  else if (digitalRead(BTN3) == HIGH) {
    tone(BUZZER_PIN, NOTE_E4);
  } 
  else if (digitalRead(BTN4) == HIGH) {
    tone(BUZZER_PIN, NOTE_F4);
  } 
  else {
    // No button pressed – stop tone
    noTone(BUZZER_PIN);
  }

  delay(50); // Small debounce delay
}
