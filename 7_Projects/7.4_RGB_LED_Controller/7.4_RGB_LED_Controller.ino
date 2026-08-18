/**
 **************************************************
 *
 * @file        7.4_RGB_LED_Controller.ino
 * @brief       Project that uses a photoresistor to set the colour of an RGB LED. As the light in the room changes,
 *              the colour slides from red in darkness, through green and blue, all the way to white in bright light.
 *              It builds on the photoresistor from section 2.3 and the LED brightness control from section 3.2, and
 *              shows how one sensor reading can drive three outputs at the same time.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
This is a variable to which we pass the number of pin that we had connected the photoresistor's output to. Because we
need to read a whole range of values here and not only HIGH or LOW, this has to be a pin that supports analog input.
The NULA board has a pin naming logic as follows: IO5, where 5 is the number that we give to the variable.
*/
const int LDR_PIN = 5;

/*
These are the variables to which we pass the numbers of pins that we had connected the three colour channels of the
RGB LED to. An RGB LED is really three LEDs in one package, one red, one green and one blue, and by lighting them at
different strengths we can mix any colour we like. All three pins have to support PWM, which is what lets us set a
brightness instead of only on or off.
*/
const int RED_PIN = 2;
const int GREEN_PIN = 3;
const int BLUE_PIN = 4;

/*
This variable will store the raw analog value read from the photoresistor. Since the NULA board uses a 12-bit ADC, the
value will range from 0 in complete darkness to 4095 in bright light.
*/
int ldrValue = 0;

/*
These three variables hold the brightness of each colour channel, from 0 for off to 255 for fully on.
*/
int r = 0, g = 0, b = 0;

/*
These two variables split the light range into three equal parts, which is what gives us our three colour transitions.
4095 divided by three is 1365, so the first part ends there and the second one ends at twice that. Feel free to
experiment with these values to move the colour changes to different light levels.
*/
const int FIRST_THIRD = 1365;
const int SECOND_THIRD = 2730;

void setup() {

  /*
  Serial.begin() establishes serial communication between your board and your computer via a USB cable. We use it here
  to watch both the light level and the mixed colour, which makes it much easier to understand what the code is doing.
  */
  Serial.begin(115200);

  /*
  pinMode() is a function that configures the specified pin to behave either as an input or as an output.
  The photoresistor is something we read, so it goes into INPUT mode, while the three colour channels are things we
  write to, so they go into OUTPUT mode.
  */
  pinMode(LDR_PIN, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  //Print out the initial message so we know that the program started successfully.
  Serial.println("RGB LED Controller with full-spectrum color mapping started");
}

void loop() {

  /*
  analogRead() reads the voltage at the given analog pin and converts it into a number. The more light falls on the
  photoresistor, the lower its resistance and the higher this number becomes.
  */
  ldrValue = analogRead(LDR_PIN);
  Serial.print("LDR value: ");
  Serial.println(ldrValue);

  /*
  This is where the colour is decided. We split the light range into three parts and give each one its own transition,
  so that the colour never jumps: it always slides from wherever it was into the next colour.
  map() is a function that takes a number from one range and rescales it into another range, and we use it here to turn
  a light level into a brightness. Notice how in each part one channel is being mapped upwards while another is mapped
  downwards, which is exactly what makes one colour fade into the next.
  */
  if (ldrValue <= FIRST_THIRD) {

    //Darkest third: fade from red (255, 0, 0) to green (0, 255, 0).
    r = map(ldrValue, 0, FIRST_THIRD, 255, 0);
    g = map(ldrValue, 0, FIRST_THIRD, 0, 255);
    b = 0;
  }
  else if (ldrValue <= SECOND_THIRD) {

    //Middle third: fade from green (0, 255, 0) to blue (0, 0, 255).
    r = 0;
    g = map(ldrValue, FIRST_THIRD + 1, SECOND_THIRD, 255, 0);
    b = map(ldrValue, FIRST_THIRD + 1, SECOND_THIRD, 0, 255);
  }
  else {

    /*
    Brightest third: fade from blue (0, 0, 255) to white (255, 255, 255). White is simply all three channels on at
    once, which is why red and green rise here while blue stays at full brightness.
    */
    r = map(ldrValue, SECOND_THIRD + 1, 4095, 0, 255);
    g = map(ldrValue, SECOND_THIRD + 1, 4095, 0, 255);
    b = 255;
  }

  /*
  analogWrite() is a function that writes an "in between" value to a pin instead of only HIGH or LOW. It does this
  using PWM, which stands for Pulse Width Modulation: the pin is switched on and off very quickly, and the longer it
  stays on during each cycle, the brighter that colour appears. Writing all three at once is what mixes the colour.
  */
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);

  //Print the mixed colour too, so we can compare it against the light level above.
  Serial.print("RGB: ");
  Serial.print(r); Serial.print(", ");
  Serial.print(g); Serial.print(", ");
  Serial.println(b);

  /*
  A short pause between readings. Keeping it small makes the colour changes look smooth.
  */
  delay(100);
}
