/**
 **************************************************
 *
 * @file        Button_Toggle_LED.ino
 * @brief       Pushbutton toggles an LED on/off using simple if logic (no debouncing).
 *              Demonstrates basic digital input reading and output control.
 * @author      Soldered
 ***************************************************/

//We define the pins used in this example
const int BUTTON_PIN = 2;   //Button connected between pin and GND
const int LED_PIN    = 13;  //On many boards this is the built-in LED

//We create a variable to keep track of LED state
bool ledState = false;

void setup() {
  /*
  pinMode(); configures how a microcontroller pin behaves.
  BUTTON_PIN is set to INPUT_PULLUP so it reads HIGH when not pressed, LOW when pressed.
  LED_PIN is set to OUTPUT so we can turn the LED on and off.
  */
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  /*
  digitalRead(); reads the current logic level of a digital input pin.
  With INPUT_PULLUP: pressed = LOW, released = HIGH.
  */
  if (digitalRead(BUTTON_PIN) == LOW) {
    //If button is pressed, turn LED on
    digitalWrite(LED_PIN, HIGH);
  }
  else {
    //If button is not pressed, turn LED off
    digitalWrite(LED_PIN, HIGH);
  }
}
