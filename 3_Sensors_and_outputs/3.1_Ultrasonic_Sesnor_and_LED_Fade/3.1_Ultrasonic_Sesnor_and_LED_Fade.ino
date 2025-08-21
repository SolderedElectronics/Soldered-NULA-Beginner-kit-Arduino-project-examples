#include "Ultrasonic-distance-sensor-easyC-SOLDERED.h"

#define ECHOPIN 3
#define TRIGPIN 2

Ultrasonic_Sensor hc(TRIGPIN,ECHOPIN); //Initializer for sensor without easyC

void setup()
{
  Serial.begin(115200);  //Begin serial communication with PC via UART
  hc.begin();  //Initialize sensor
}

void loop()
{
  Serial.print("Distance from obstacle is: ");
  Serial.print(hc.getDistance());               //Get distance saved in sensors register
  Serial.println(" cm.");                       
  Serial.print("Time took wave to return: ");
  Serial.print(hc.getDuration());               //Get time needed for bounced ultrasound wave 
                                                //to return to the sensor
  Serial.println(" uS.");
  delay(250);
}