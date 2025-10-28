/**
 **************************************************
 *
 * @file        7.2_Speaker_Alarm.ino
 * @brief       Example that plays a funky melody on the buzzer when a button is pressed. 
 *              The melody is defined as a sequence of notes and durations. 
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************/

// --- Pin definitions ---
const int buzzerPin = 2;   // Pin where the buzzer is connected
const int buttonPin = 3;   // Pin where the button is connected

/*
Define note frequencies (in Hz) for the tones used in this melody.
*/
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

/*
Define the melody as an array of notes. 
The number 0 represents a pause (rest) instead of a tone.
*/
int melody[] = {
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_E5, NOTE_D4, NOTE_F4, NOTE_G4, 0,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_E4
};

/*
Define the note durations. 
- 4 = quarter note, 
- 8 = eighth note, 
- 2 = half note, etc.
*/
int noteDurations[] = {
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 8, 4, 8,
  8, 8, 4, 2, 4
};

void setup() {
  /*
  Configure the buzzer pin as OUTPUT, since we will send signals to it.
  */
  pinMode(buzzerPin, OUTPUT);

  /*
  Configure the button pin as INPUT. 
  Make sure to wire the button properly with either a pull-up or pull-down resistor.
  */
  pinMode(buttonPin, INPUT);
}

void loop() {
  /*
  Check the state of the button. 
  If it is pressed (reads HIGH), play the funky melody.
  */
  if (digitalRead(buttonPin) == HIGH) {
    playFunkySong();
  }
}

/*
This function plays the melody defined in the arrays above.
It iterates over all the notes and plays them one by one using tone().
*/
void playFunkySong() {
  int length = sizeof(melody) / sizeof(melody[0]);  // Number of notes in the melody

  for (int thisNote = 0; thisNote < length; thisNote++) {

    /*
    Calculate the note duration in milliseconds.
    Example: 1000 / 4 = 250 ms for a quarter note.
    */
    int noteDuration;
    if (noteDurations[thisNote] > 0) {
      noteDuration = 1000 / noteDurations[thisNote];
    } else {
      noteDuration = 500; // Default value if none given
    }

    /*
    If the note is not a pause (0), play it using tone().
    */
    if (melody[thisNote] != 0) {
      tone(buzzerPin, melody[thisNote], noteDuration);
    }

    /*
    Pause between notes to make them distinct. 
    Multiplying by 1.3 adds a short gap between tones.
    */
    int pauseBetweenNotes = noteDuration * 1.3;
    delay(pauseBetweenNotes);

    /*
    Stop the tone before moving to the next note.
    */
    noTone(buzzerPin);
  }
}
