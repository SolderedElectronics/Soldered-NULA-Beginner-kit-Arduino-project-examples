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
  As our pin needs to detect if the button has been pressed, we will put the pin in INPUT_PULLUP mode.
  INPUT_PULLUP switches on a small resistor inside the chip that gently ties the pin to 3.3V. Without it the pin would
  be floating, meaning it is connected to nothing and picks up random noise, and the board would read presses that
  never happened. Because the resistor holds the pin high, the button only has to connect the pin to GND, so no extra
  parts are needed on the breadboard.
  */
  pinMode(BUTTON_PIN, INPUT_PULLUP);

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
  Note that the reading is the other way around from what you might expect. The pull-up resistor holds the pin at 3.3V
  while the button is released, so the board reads HIGH, and pressing the button connects the pin to GND so it reads
  LOW. A button wired this way is called active low.
  */
  bool reading = digitalRead(BUTTON_PIN);

  /*
  If the button is pressed, increase the counter by one and print it to the Serial Monitor.
  Since this version does not include debouncing, multiple counts may appear for a single press.
  */
  if (reading == LOW) {
    counter++;
    Serial.print("Counter: ");
    Serial.println(counter);

    /*
    Wait for the button to be released before allowing another count.
    This prevents the counter from increasing too quickly while the button is still held down.
    */
    while (digitalRead(BUTTON_PIN) == LOW) {
      // Wait until the button is released
    }
  }
}
