/**
 **************************************************
 *
 * @file        LED_blinking.ino
 * @brief       Example that shows how to control the blinking of a simple LED.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************/

/*
This is a variable to which we pass the number of pin that we had connected the LED to. 
The NULA board has a pin naming logic as follows: IO4, where 4 is the number that we give to the variable.
If you wish to use a different pin, make sure you are using a IO__ marked pin. 
*/
int pinNumber = 4;

/*
This is a variable that defines the blinking time, in milliseconds.
Feel free to experiment with this value.
*/
int delayMS=1000;

void setup() {

  /*
  pinMode() is a function that configures the specified pin to behave either as an input or in this case as an output.
  This simply means that the pin "reads" the available data when in input mode, and "writes" data when in output mode.
  As our pin needs to turn on the LED, we will put the pin in OUTPUT mode.
  */
  pinMode(pinNumber,OUTPUT);
}

void loop() {

  /*
  digitalWrite() is a function that gives us the ability to change the value that our pin (previously set as OUTPUT pin) 
  writes. As the name of the function tells us, we can only switch between digital values, high and low. Those values
  are represented by different voltage levels, high is 5V while low is 0V. We will start with putting the pin in HIGH
  mode, giving the LED 5V and lighting it up. 
  */
  digitalWrite(pinNumber,HIGH);

  /*
  delay() is a function that starts a pause in the code. Duration of the pause is defined with the ms parameter.
  In this case, we want to wait for a bit after we turned on the LED.
  */
  delay(delayMS);

  //We put the pin in LOW mode, turning the LED off.
  digitalWrite(pinNumber,LOW);

  //Leave the LED turned off for a bit.
  delay(delayMS);
}
