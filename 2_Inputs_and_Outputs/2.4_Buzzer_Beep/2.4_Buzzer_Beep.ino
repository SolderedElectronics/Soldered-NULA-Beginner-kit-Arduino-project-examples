/**
 **************************************************
 *
 * @file        2.4_Buzzer_Beep.ino
 * @brief       Example that shows how to generate simple sounds using a passive buzzer.
 *              The NULA MINI board will play a short melody by changing tone frequencies.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
This is a variable to which we assign the number of the pin that we connected the buzzer to.
In this example, we’ll use IO5 as the output pin. If you want, you can use any other digital pin that has ADC capabilities.
*/
const int BUZZER_PIN = 5;

/*
We will use two arrays — one for note frequencies (in Hertz) and one for note durations.
These define a short melody that the buzzer will play.
*/
int melody[] = { 262, 294, 330, 349, 392, 440, 494, 523 }; // C4 to C5
int noteDuration[] = { 4, 4, 4, 4, 4, 4, 4, 2 };           // Quarter notes (last one is half note)

void setup() {
  /*
  The tone() function generates a square wave of a given frequency (and duration)
  on a specific pin, which makes the buzzer produce sound.
  */
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Playing melody...");
  
  // Loop through the melody and play each note
  for (int i = 0; i < 8; i++) {
    int duration = 1000 / noteDuration[i];  // Convert duration type to milliseconds
    tone(BUZZER_PIN, melody[i], duration);  // Play the note
    delay(duration * 1.3);                  // Wait a bit before the next note
  }

  // Stop tone when done
  noTone(BUZZER_PIN);
  Serial.println("Melody finished!");
}

void loop() {
  // Nothing happens here — melody plays once in setup()
}
