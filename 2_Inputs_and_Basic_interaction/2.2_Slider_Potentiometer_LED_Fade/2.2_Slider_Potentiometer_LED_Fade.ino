/*
 **************************************************
 *
 * @file        Slider_Potentiometer_LED_Fade.ino
 * @brief       Example that shows how to control LED brightness using a PWM pin and a sliding potentiometer.
 *              For details, connection diagram, and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************/

/*
This is a variable to which we assign the number of the pin that we connected the sliding potentiometer's output to.
For this example, we will use a pin that is able to read analog input, pins that have Analog-Digital Converter (ADC)
capability, because we don't simply want to turn the LED on or off, but control its brightness gradually.

In this example we will be using the pin 3. If you want to use another ADC pin, check out the pinout diagram at
the example documentation:  <link placeholder>
*/
int POT_PIN = 3;

//We store the converted value from the potentiometer's output
int potValue=0;

/*
This is a variable to which we pass the number of pin that we had connected the LED to. 
The NULA board has a pin naming logic as follows: IO4, where 4 is the number that we give to the variable.
If you wish to use a different pin, make sure you are using a IO__ marked pin. 
*/
int LED_PIN = 4;

void setup() {

  /*
  pinMode() is a function that configures the specified pin to behave either as an input or in this case as an output.
  This simply means that the pin "listens" for available data when in input mode, and "writes" data when in output mode.
  As our pin needs to turn on the LED, we will put the pin in OUTPUT mode.
  */
  pinMode(LED_PIN, OUTPUT);

  /*
  analogReadResolution() is a function that lets you set ADC resolution (in bits) of the value returned by analogRead()
  function. The NULA board contains a multichannel, 12-bit ADC. This means that it will map input voltages between 0
  and the operating voltage (+ 3.3V) into integer values between 0 and 4095. This yields a resolution between readings
  of 3.3 volts / 4096 units or  0.000805 volts per unit.

  */
  analogReadResolution(12);

  /*
  analogWriteResolution() is a function that lets you change the PWM resolution (in bits). The default value is usually 8 or 10 bits
  */
  analogWriteResolution(LED_PIN,12);
}

void loop() {
  
  //analogRead() is a function that reads the value from a specified analog input pin. 
  
  potValue=analogRead(POT_PIN);

  /*
  analogWrite() is a function that writes an analog value (PWM wave) to a pin. After the function is called, the pin will
  generate a steady rectangular wave of a specified duty cycle untill the next call to analogWrite() or digitalRead()
  on the same pin.
  */
  analogWrite(LED_PIN, potValue);
  delay(50);

}
