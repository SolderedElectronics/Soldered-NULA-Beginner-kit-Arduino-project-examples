/**
 **************************************************
 *
 * @file        2.2_Button_Debounce.ino
 * @brief       Example that shows how to toggle LED light with a press of a button. 
 *              In this example we will be using a button reading technique called debouncing. This technique gives us
 *              an easy way to stabilize the button readings.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
This is a variable to which we pass the number of pin that we had connected the BUTTON to.
The NULA board has a pin naming logic as follows: IO19, where 19 is the number that we give to the variable.
If you wish to use a different pin, make sure you are using a IO__ marked pin. 
*/
const int BUTTON_PIN = 19;

/*
This is a variable to which we pass the number of pin that we had connected the LED to. 
The NULA board has a pin naming logic as follows: IO4, where 4 is the number that we give to the variable.
If you wish to use a different pin, make sure you are using a IO__ marked pin. 

Remember that the LED needs a 330 Ohm resistor in series with it. That resistor limits how much current flows, and
without it the LED draws more than either it or the pin is built for, so both can be damaged.
*/
const int LED_PIN = 4;

/*
Those are the variables used for button debouncing. To learn more about button debouncing, check out our example
documentation at: <link placeholder>
*/
bool ledState = false;
bool lastState = HIGH;
unsigned long lastChangeMs = 0;
const unsigned long debounceMs = 25;

void setup() {

  /*
  pinMode() is a function that configures the specified pin to behave either as an input or in this case as an output.
  This simply means that the pin "listens" for the available data when in input mode, and "writes" data when in output mode.
  As our pin needs to detect if the button has been pressed, we will put the pin in INPUT_PULLUP mode.
  INPUT_PULLUP switches on a small resistor inside the chip that gently ties the pin to 3.3V, so the pin reads HIGH
  while the button is released and the button only has to connect it to GND to read LOW.
  */
  pinMode(BUTTON_PIN, INPUT_PULLUP);  

  /*
  pinMode() is a function that configures the specified pin to behave either as an input or in this case as an output.
  This simply means that the pin "listens" for available data when in input mode, and "writes" data when in output mode.
  As our pin needs to turn on the LED, we will put the pin in OUTPUT mode.
  */
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  
  //digitalRead() is a function that reads the value from a specified digital pin, either HIGH or LOW.
  bool reading = digitalRead(BUTTON_PIN);

  /*
  millis() is a function that returns the number of milliseconds passed since the board began running the current program.
  In this example, we use this function to check out if the debouncing period has finished. This number will overflow 
  (go back to zero), after approximately 50 days.
  */
  unsigned long now = millis();
 
  /*
  This is our debouncing logic, we check if the button has been pressed and if enough time has passed so that we don't get 
  false readings because of the noise in the signal.
  */
  if (reading != lastState && (now - lastChangeMs) > debounceMs) {
    lastChangeMs = now;

    /*
    As we are using the pull-up method for connecting the button, the readings are the other way around from what you
    might expect: the pin reads HIGH while the button is released and LOW while it is pressed. So we need to toggle the
    LED when the button state goes from HIGH to LOW.
    */
    if (lastState == HIGH && reading == LOW) {

      //We toggle the ledState
      ledState = !ledState;

      //We turn the LED on or off depending on the current ledState                
      if (ledState == true) {
        digitalWrite(LED_PIN, HIGH);
      } else {
        digitalWrite(LED_PIN, LOW);
      }
    }
    lastState = reading;
  }
}
