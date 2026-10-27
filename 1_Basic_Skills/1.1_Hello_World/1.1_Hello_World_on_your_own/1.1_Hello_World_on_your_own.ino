/**
 **************************************************
 *
 * @file        1.1_Hello_World_on_your_own.ino
 * @brief       Variation of the 1.1 Hello World example that prints "Hello, World!" once a second for as long as the
 *              board has power.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

void setup() {
  /*
  Serial.begin() establishes serial communication between your board and another device, in this
  example, to your computer via a USB cable. In this example we will be communicating with the Serial Monitor, so ensure
  that the baud variable is set to one of the baud rates listed in the Serial Monitor's baud rate dropdown menu. 
  */
  Serial.begin(115200);
}

void loop() {

  /*
  Serial.println() Prints data to serial port as human-readable ASCII text followed by a carriage return character ('\r')
  and a newline character ('\n'). In other words, function prints the text and then skips to the next line.
  */
  Serial.println("Hello, World!");

  // Wait one second before the next pass, or the lines would pour out too fast to read
  delay(1000);
}
