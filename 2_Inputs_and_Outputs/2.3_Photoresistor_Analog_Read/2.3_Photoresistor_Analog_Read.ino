/**
 **************************************************
 *
 * @file        2.3_Photoresistor_Analog_Read.ino
 * @brief       Example that shows how to read light intensity using a photoresistor (LDR)
 *              connected to the analog input of the NULA MINI board.
 *              The program prints the measured value to the Serial Monitor.
 *
 *              For wiring details, circuit diagram, and more, check out the example
 *              documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
This is a variable to which we assign the number of the pin that we connected the photoresistor's output to.
The NULA MINI board uses analog-capable pins (ADC pins) to read varying voltages. 
In this example, we will use IO5, which supports analog input.
*/
const int LDR_PIN = 5;  // Analog input pin for photoresistor

/*
This variable will store the raw analog value read from the sensor.
Since the NULA board uses a 12-bit ADC, the returned value will range from 0 to 4095.
*/
int lightValue = 0;

void setup() {

  /*
  Serial.begin() starts serial communication between the board and the computer.
  We use it to display the measured values on the Serial Monitor.
  */
  Serial.begin(115200);

  /*
  analogReadResolution() defines how many bits are used for ADC readings.
  The NULA MINI supports 12-bit resolution, which means analogRead() returns values from 0 to 4095.
  */
  analogReadResolution(12);

  /*
  Print a startup message to confirm that the program is running.
  */
  Serial.println("Cover or shine light on the sensor to see value changes...");
}

void loop() {

  /*
  analogRead() reads the voltage at the given analog pin and converts it into a digital number.
  The higher the light intensity, the lower the resistance of the photoresistor, and the higher the voltage read.
  */
  lightValue = analogRead(LDR_PIN);

  /*
  Print the measured value to the Serial Monitor.
  */
  Serial.print("Light level: ");
  Serial.println(lightValue);

  /*
  Add a short delay so the readings are easier to observe.
  */
  delay(500);
}
