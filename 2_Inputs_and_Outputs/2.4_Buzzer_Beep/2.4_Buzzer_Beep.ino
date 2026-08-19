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
The NULA board has a pin naming logic as follows: IO5, where 5 is the number that we give to the variable.
*/
const int BUZZER_PIN = 5;

/*
We will use two arrays - one for note frequencies (in Hertz) and one for note durations.
These define a short melody that the buzzer will play.
*/
int melody[] = { 262, 294, 330, 349, 392, 440, 494, 523 }; // C4 to C5
int noteDuration[] = { 4, 4, 4, 4, 4, 4, 4, 2 };           // Quarter notes (last one is half note)

/*
Here we work out how many notes the melody has, instead of counting them by hand. sizeof() tells us how much memory
something takes up, so the size of the whole array divided by the size of one entry gives us the number of entries.
Written this way the number stays correct even after you add a note of your own.
*/
const int NOTE_COUNT = sizeof(melody) / sizeof(melody[0]);

void setup() {

  /*
  pinMode() is a function that configures the specified pin to behave either as an input or in this case as an output.
  As our pin needs to drive the buzzer, we will put the pin in OUTPUT mode.
  */
  pinMode(BUZZER_PIN, OUTPUT);

  /*
  Serial.begin() establishes serial communication between your board and your computer via a USB cable. We use it here
  to let you know when the melody starts and when it is done.
  */
  Serial.begin(115200);
  Serial.println("Playing melody...");

  /*
  A for loop repeats a block of code a set number of times, counting with a variable of its own. Here "i" starts at 0
  and grows by one each pass until it reaches NOTE_COUNT, so the block below runs once for every note in the melody.
  */
  for (int i = 0; i < NOTE_COUNT; i++) {

    /*
    Musical note lengths are written as fractions: a quarter note is a quarter of a whole note. Here we turn that
    fraction into milliseconds by dividing one second by the number in the array, so a 4 becomes 250 ms and a 2
    becomes 500 ms.
    */
    int duration = 1000 / noteDuration[i];

    /*
    tone() is a function that makes the buzzer produce a sound. It generates a square wave of the given frequency,
    which our ears hear as the pitch of the note, and the third value tells it how long to keep playing.
    */
    tone(BUZZER_PIN, melody[i], duration);

    /*
    delay() is a function that starts a pause in the code. We wait slightly longer than the note itself, which leaves a
    short silence between notes so they do not run into each other. Feel free to experiment with the 1.3.
    */
    delay(duration * 1.3);
  }

  /*
  noTone() stops whatever sound the buzzer was making. The last note already ended on its own, but calling this makes
  sure the buzzer is left silent no matter what.
  */
  noTone(BUZZER_PIN);
  Serial.println("Melody finished!");
}

void loop() {
  // Nothing happens here - melody plays once in setup()
}
