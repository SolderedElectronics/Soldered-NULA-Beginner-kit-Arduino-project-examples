/**
 **************************************************
 *
 * @file        7.3_Parking_sensor.ino
 * @brief       Project that builds the parking sensor you know from cars. The Soldered Ultrasonic Distance Sensor
 *              watches for obstacles, and the closer one gets, the faster the buzzer beeps. Under 10 cm the beeping
 *              turns into one continuous tone and an LED lights up as a final warning.
 *              It builds on the distance measuring from section 3.1 and the buzzer from section 2.4.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
Include the Soldered library for the Ultrasonic Distance Sensor, so we can read the distance with a single function
call instead of timing the echo ourselves.
*/
#include "Ultrasonic-distance-sensor-easyC-SOLDERED.h"

/*
This is a variable to which we pass the number of pin that we had connected the sensor's TRIG pin to, the pin the
board uses to tell the sensor to send out an ultrasonic pulse.
The NULA board has a pin naming logic as follows: IO4, where 4 is the number that we give to the variable.
If you wish to use a different pin, make sure you are using a IO__ marked pin.
*/
const int TRIGPIN = 4;

/*
This is a variable to which we pass the number of pin that we had connected the sensor's ECHO pin to, the pin on which
the sensor reports back how long it took for the pulse to return.
*/
const int ECHOPIN = 3;

/*
This is a variable to which we pass the number of pin that we had connected the buzzer to.
*/
const int BUZZER_PIN = 2;

/*
This is a variable to which we pass the number of pin that we had connected the warning LED to.
*/
const int LED_PIN = 5;

/*
Here we create our sensor object, which we named "hc". We pass it the two pin numbers we defined above, in the order
TRIG first and ECHO second, so the library knows how the sensor is wired to the board.
*/
Ultrasonic_Sensor hc(TRIGPIN, ECHOPIN);

/*
This is the frequency of the warning sound, in Hertz. A frequency is how many times per second the buzzer moves back
and forth, and our ears hear it as the pitch of the sound. Small buzzers like this one are loudest somewhere between
2 and 4 kHz, so feel free to experiment with this value until it sounds best to you.
*/
const int TONE_FREQ = 2700;

/*
These three variables are what lets us beep at different speeds without ever stopping the program.
"lastBeep" remembers the moment the buzzer was last switched on or off, "beepInterval" holds how long we want to wait
between those switches, and "buzzerOn" remembers whether the buzzer is currently sounding or silent.
*/
unsigned long lastBeep = 0;
unsigned long beepInterval = 0;
bool buzzerOn = false;

void setup()
{
  /*
  Serial.begin() establishes serial communication between your board and your computer via a USB cable. We use it here
  to watch the measured distance while we test the sensor.
  */
  Serial.begin(115200);

  /*
  begin() prepares the sensor for use, setting the TRIG pin as an output and the ECHO pin as an input for us.
  */
  hc.begin();

  /*
  pinMode() is a function that configures the specified pin to behave either as an input or in this case as an output.
  Both the buzzer and the LED are things we write to, so both go into OUTPUT mode. Right after that we write LOW to the
  LED pin so the warning light starts out switched off.
  */
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  //Print out the initial message so we know that the program started successfully.
  Serial.println("Ultrasonic buzzer + LED reverse sensor started");
}

void loop()
{
  /*
  getDistance() is a function that makes the sensor take one measurement and returns the result in centimeters.
  */
  float distance = hc.getDistance();

  /*
  getDuration() returns the raw travel time of the echo in microseconds, before it is converted into a distance.
  We only read it here so you can see the number the sensor actually gives us. Keep in mind that calling it sends out
  a second ultrasonic pulse, so if you don't need this value you can safely delete this line.
  */
  long duration  = hc.getDuration();

  //Print the measured distance to the Serial Monitor so we can follow along while testing.
  Serial.print("Distance from obstacle: ");
  Serial.print(distance);
  Serial.println(" cm");

  /*
  We switch the LED off at the start of every pass through the loop. Only the closest range switches it back on again
  a few lines below, so this one line saves us from having to turn it off in every other case.
  */
  digitalWrite(LED_PIN, LOW);

  /*
  This chain of if statements is the heart of the project: it turns a distance into a beeping speed. Each range gets a
  different value of beepInterval, and a smaller interval means less waiting between beeps, which we hear as faster
  beeping. The board checks the ranges from the widest down and stops at the first one that matches.
  Feel free to experiment with both the distances and the intervals.
  */
  if (distance > 100) {

    //Nothing in range. noTone() stops whatever sound the buzzer was making and an interval of 0 keeps it silent.
    beepInterval = 0;
    noTone(BUZZER_PIN);
  }
  else if (distance > 60) {

    //Between 60 and 100 cm, beep slowly.
    beepInterval = 800;
  }
  else if (distance > 30) {

    //Between 30 and 60 cm, beep at a medium speed.
    beepInterval = 400;
  }
  else if (distance > 10) {

    //Between 10 and 30 cm, beep quickly.
    beepInterval = 150;
  }
  else {

    /*
    Closer than 10 cm. Here we call tone() without an interval, which makes the buzzer sound continuously instead of
    beeping, and we light up the LED as a final warning.
    */
    tone(BUZZER_PIN, TONE_FREQ);
    beepInterval = 0;
    digitalWrite(LED_PIN, HIGH);
  }

  /*
  This is where the beeping itself happens. millis() is a function that returns the number of milliseconds passed
  since the board began running the current program. By comparing it against the moment of the last switch we can wait
  the right amount of time without using delay(), which would freeze the program and stop us from measuring.
  Every time the interval has passed we flip buzzerOn to its opposite value with the "!" operator, and then either
  start the tone or stop it. Doing that over and over is what produces a beep.
  */
  if (beepInterval > 0) {
    unsigned long now = millis();
    if (now - lastBeep >= beepInterval) {
      lastBeep = now;
      buzzerOn = !buzzerOn;
      if (buzzerOn) tone(BUZZER_PIN, TONE_FREQ);
      else          noTone(BUZZER_PIN);
    }
  }

  /*
  A short pause between measurements. Taking readings much faster than this does not make the sensor more accurate,
  it only keeps the board busy.
  */
  delay(50);
}
