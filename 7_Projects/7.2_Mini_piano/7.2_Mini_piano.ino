/**
 **************************************************
 *
 * @file        7.2_Mini_piano.ino
 * @brief       Project that turns four buttons and a passive buzzer into a small piano. Each button has its own
 *              frequency, and pressing it plays that note for as long as you hold the button down.
 *              It builds on the button reading from section 2.1 and the buzzer from section 2.4.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
This is a variable to which we pass the number of pin that we had connected the buzzer to.
The NULA board has a pin naming logic as follows: IO18, where 18 is the number that we give to the variable.
If you wish to use a different pin, make sure you are using a IO__ marked pin.
*/
const int BUZZER_PIN = 18;

/*
These are the variables to which we pass the numbers of pins that we had connected the four BUTTONS to. Each button
gets a pin of its own, because the board has to be able to tell them apart.
*/
const int BTN1 = 2;
const int BTN2 = 3;
const int BTN3 = 4;
const int BTN4 = 5;

/*
These are the frequencies of the four notes, in Hertz. A frequency is how many times per second the buzzer moves back
and forth, and it is what our ears hear as the pitch of a sound: the higher the number, the higher the note.
The names come from the musical scale, where C4 is the C in the middle of a piano keyboard. Feel free to experiment
with these values, or look up the frequencies of other notes and build your own scale.
*/
const int NOTE_C4 = 262;
const int NOTE_D4 = 294;
const int NOTE_E4 = 330;
const int NOTE_F4 = 349;

void setup() {

  /*
  pinMode() is a function that configures the specified pin to behave either as an input or as an output.
  As these pins need to detect if a button has been pressed, we will put them in INPUT_PULLUP mode. INPUT_PULLUP
  switches on a small resistor inside the chip that ties each pin to 3.3V, which means a pin sits at HIGH while its
  button is released and goes LOW while it is pressed.
  */
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP);

  /*
  The buzzer is the one pin that has to write instead of read, so we put it in OUTPUT mode.
  */
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {

  /*
  digitalRead() is a function that reads the value from a specified digital pin, either HIGH or LOW. Because of the
  pull-up resistors, a pressed button reads LOW.
  tone() is a function that makes the buzzer produce a sound of the given frequency. Called with only a pin and a
  frequency, as it is here, it keeps playing until we stop it ourselves.
  Notice the "else if" chain: the board checks the buttons in order and stops at the first one it finds pressed, so
  pressing two buttons at once plays only the note that comes first in this list.
  */
  if (digitalRead(BTN1) == LOW) {
    tone(BUZZER_PIN, NOTE_C4);
  } 
  else if (digitalRead(BTN2) == LOW) {
    tone(BUZZER_PIN, NOTE_D4);
  } 
  else if (digitalRead(BTN3) == LOW) {
    tone(BUZZER_PIN, NOTE_E4);
  } 
  else if (digitalRead(BTN4) == LOW) {
    tone(BUZZER_PIN, NOTE_F4);
  } 
  else {

    /*
    If none of the buttons is pressed we end up here. noTone() stops whatever sound the buzzer was making, which is
    what makes the note stop as soon as you let go of the button.
    */
    noTone(BUZZER_PIN);
  }

  /*
  delay() is a function that starts a pause in the code. This very short pause gives the button contacts a moment to
  settle, which stops a single press from being read as several. Section 2.2 explains this in detail under the name
  debouncing.
  */
  delay(50);
}
