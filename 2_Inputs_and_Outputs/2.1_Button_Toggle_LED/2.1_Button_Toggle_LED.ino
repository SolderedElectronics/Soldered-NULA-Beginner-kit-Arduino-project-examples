/**
 **************************************************
 *
 * @file        2.1_Button_Counter.ino
 * @brief       Example that shows how to increase a counter each time a button is pressed.
 *              This example does NOT use any debouncing or signal stabilization.
 *              For details, connection diagram, and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
This is a variable to which we pass the number of the pin that we connected the BUTTON to.
The NULA board has a pin naming logic as follows: IO19, where 19 is the number that we give to the variable.
If you wish to use a different pin, make sure you are using a IO__ marked pin.
*/
const int BUTTON_PIN = 19;

/*
This variable holds the value of our counter.
Each time the button is pressed, the counter value will increase by one.
*/
int counter = 0;

void setup() {

  /*
  pinMode() is a function that configures the specified pin to behave either as an input or an output.
  As our pin needs to detect if the button has been pressed, we will put the pin in INPUT mode.
  */
  pinMode(BUTTON_PIN, INPUT);

  /*
  Serial.begin() initializes serial communication between the NULA board and the computer.
  We use it here so we can print out the current counter value on the Serial Monitor.
  */
  Serial.begin(115200);

  /*
  Print out the initial message so we know that the program started successfully.
  */
  Serial.println("Button Counter Example started!");
  Serial.println("Press the button to increase the counter...");
}

void loop() {

  /*
  digitalRead() is a function that reads the value from a specified digital pin, either HIGH or LOW.
  When the button is pressed, it connects the pin to 3.3V, and the board reads HIGH.
  When it’s released, the pin is connected to GND through a pull-down resistor and reads LOW.
  */
  bool reading = digitalRead(BUTTON_PIN);

  /*
  If the button is pressed, increase the counter by one and print it to the Serial Monitor.
  Since this version does not include debouncing, multiple counts may appear for a single press.
  */
  if (reading == HIGH) {
    counter++;
    Serial.print("Counter: ");
    Serial.println(counter);

    /*
    Wait for the button to be released before allowing another count.
    This prevents the counter from increasing too quickly while the button is still held down.
    */
    while (digitalRead(BUTTON_PIN) == HIGH) {
      // Wait until the button is released
    }
  }
}
