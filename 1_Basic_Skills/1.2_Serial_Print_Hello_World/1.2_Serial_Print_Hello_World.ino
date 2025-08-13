/**
 **************************************************
 *
 * @file        Serial_Print_Hello_World.ino
 * @brief       Example that shows basic functionality of the Serial Monitor, writing data only.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************/

void setup() {
  /*
  Serial.begin() establishes serial communication between your board and another device, in this
  example, to your computer via a USB cable. In this example we will be communicating with the Serial Monitor, so ensure
  that the baud variable is set to one of the baud rates listed in the Serial Monitor's baud rate dropdown menu. 
  */
  Serial.begin(115200);

  /*
  Serial.println() Prints data to serial port as human-readable ASCII text followed by a carriage return character ('\r')
  and a newLine character ('\n'). In other words, function prints the text and then skips to the next line.
  */
  Serial.println("Hello, World!");
}

void loop() {
  //Nothing happens here, message was printed once in setup()
}
