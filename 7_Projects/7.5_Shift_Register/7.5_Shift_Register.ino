/**
 **************************************************
 *
 * @file        7.5_Shift_Register.ino
 * @brief       Project that shows how to drive many LEDs with only three pins of the board, using a 74HC595 shift
 *              register. The example builds a 4-bit binary counter that counts from 0 to 15 and displays the count on
 *              four LEDs, which is a nice way of seeing how computers count in binary.
 *              It introduces two new ideas: shifting data out one bit at a time, and latching the outputs.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
This is a variable to which we pass the number of pin that we had connected the shift register's latch pin to. On the
74HC595 chip this pin is marked ST_CP. The latch is what tells the chip "the data I sent you is complete, show it now",
which is why the LEDs change all at once instead of flickering through every step.
The NULA board has a pin naming logic as follows: IO3, where 3 is the number that we give to the variable.
If you wish to use a different pin, make sure you are using a IO__ marked pin.
*/
int latchPin = 3;

/*
This is a variable to which we pass the number of pin that we had connected the shift register's clock pin to, marked
SH_CP on the chip. The clock is a pin we switch up and down, and every time it goes up the chip takes in one more bit.
This is how the chip knows when the next bit is ready.
*/
int clockPin = 4;

/*
This is a variable to which we pass the number of pin that we had connected the shift register's data pin to, marked DS
on the chip. This is the pin the bits themselves travel over, one after another.
*/
int dataPin = 2;

/*
This variable holds the value of our counter. Each pass through the loop it grows by one, and after 15 it starts over
from zero.
*/
int counter = 0;

void setup() {

  /*
  pinMode() is a function that configures the specified pin to behave either as an input or in this case as an output.
  All three pins going to the shift register are pins we write to, so all three go into OUTPUT mode.
  */
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {

  /*
  Here we keep our counter inside four bits. The "&" is a bitwise AND, and 0x0F is the hexadecimal way of writing the
  number 15, which in binary is 1111. Combining a number with 1111 this way keeps only its lowest four bits and throws
  the rest away, which is a common trick for making sure a value stays in range.
  A byte is a variable that holds exactly eight bits, which is also exactly how much data the 74HC595 takes at a time.
  */
  byte value = counter & 0x0F;

  /*
  Pulling the latch pin LOW tells the chip that we are about to send new data and that it should not change its outputs
  yet. Without this the LEDs would visibly flicker while the bits are still arriving.
  */
  digitalWrite(latchPin, LOW);

  /*
  shiftOut() is a function that sends one byte out over a data pin, one bit at a time, pulsing the clock pin after each
  bit. MSBFIRST tells it to start with the most significant bit, meaning the leftmost one, which is the order the
  74HC595 expects.
  */
  shiftOut(dataPin, clockPin, MSBFIRST, value);

  /*
  Pulling the latch pin back HIGH tells the chip that the data is complete. Only now do the outputs change, and all
  eight of them change together.
  */
  digitalWrite(latchPin, HIGH);

  /*
  Count one up, and start over from zero once we pass 15, because four LEDs cannot show any number higher than that.
  */
  counter++;
  if (counter > 15){
    counter = 0;
  } 

  /*
  delay() is a function that starts a pause in the code. Half a second is slow enough to follow the counting with your
  eyes. Feel free to experiment with this value.
  */
  delay(500);
}
