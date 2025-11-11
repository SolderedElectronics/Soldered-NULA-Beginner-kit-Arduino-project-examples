/**
 * @file 7_5_shift_register_binary_counter.ino
 * @brief Demonstrates how to use the 74HC595 shift register to control multiple LEDs with only three pins on the NULA MINI.
 *        This example creates a 4-bit binary counter that counts from 0 to 15, displaying the count on 4 LEDs connected
 *        to the shift register outputs. It introduces the concepts of serial data shifting and output latching.
 * @author Soldered
 **/

int latchPin = 3;  // ST_CP pin (Latch)
int clockPin = 4; // SH_CP pin (Clock)
int dataPin = 2;  // DS pin (Data)

int counter = 0;   // 4-bit counter (0–15)

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  // Limit counter to 4 bits (0–15)
  byte value = counter & 0x0F;  

  digitalWrite(latchPin, LOW);    // Prepare shift register for data
  shiftOut(dataPin, clockPin, MSBFIRST, value);  // Send data bits
  digitalWrite(latchPin, HIGH);   // Latch outputs to LEDs

  counter++;  // Increment counter
  if (counter > 15){
    counter = 0;  // Wrap around to 0 after 15
  } 

  delay(500);  // Wait half a second
}
