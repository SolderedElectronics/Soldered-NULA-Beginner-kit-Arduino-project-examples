// -----------------------------
// Funky Buzzer Song on Button Press
// -----------------------------

// Buzzer pin
const int buzzerPin = 8;

// Button pin
const int buttonPin = 2;

// Define note frequencies
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_E5  659
#define NOTE_G5  784

// A funky sequence (can be changed for style)
int melody[] = {
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_E5, NOTE_D4, NOTE_F4, NOTE_G4, 0,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_E4
};

// Note duration types (4 = quarter note, 8 = eighth, etc.)
int noteDurations[] = {
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 8, 4, 8,
  8, 8, 4, 2, 4
};

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT); // button wired to GND + pin
}

void loop() {
  // button pressed = LOW because of INPUT_PULLUP
  if (digitalRead(buttonPin) == HIGH) {
    playFunkySong();
  }
}

// Function to play the melody
void playFunkySong() {
  int length = sizeof(melody) / sizeof(melody[0]);
  for (int thisNote = 0; thisNote < length; thisNote++) {
    int noteDuration;
    if (noteDurations[thisNote] > 0) {
      noteDuration = 1000 / noteDurations[thisNote];
    } else {
      noteDuration = 500; // default
    }

    if (melody[thisNote] != 0) {
      tone(buzzerPin, melody[thisNote], noteDuration);
    }

    // Pause between notes
    int pauseBetweenNotes = noteDuration * 1.3;
    delay(pauseBetweenNotes);

    noTone(buzzerPin);
  }
}
