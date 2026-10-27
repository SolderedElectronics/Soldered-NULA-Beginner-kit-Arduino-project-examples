/**
 **************************************************
 *
 * @file        4.1_Print_Message_on_your_own.ino
 * @brief       Variation of the 4.1 Print message example that also counts the seconds the program has been running
 *              on the last three free columns of the bottom row.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Borna Zelenika @ soldered.com
 ***************************************************
 */

#include "LCD-SOLDERED.h"   // Include the Soldered LCD library

/*
Create an LCD object with 16 columns and 2 rows.
These numbers correspond to the width and height of the display.
*/
LCD lcd(16, 2);

int seconds = 0;

void setup() {
  /*
  Initialize the LCD display and turn on the backlight (if supported by your module).
  */
  lcd.begin();
  lcd.backlight();

  /*
  Clear the display to ensure a clean start.
  */
  lcd.clear();

  /*
  Set the cursor to the first column of the first row.
  Then print a simple message on the LCD screen.
  */
  lcd.setCursor(0, 0);
  lcd.print("Hello, NULA!");

  /*
  Move to the second line and print another message. Keep in mind that this display fits exactly 16 characters per row,
  so anything longer is simply cut off at the edge. Count the characters of your own messages before printing them.
  */
  lcd.setCursor(0, 1);
  lcd.print("Let's start!");
}

void loop() {
  lcd.setCursor(13, 1);
  lcd.print(seconds);
  seconds++;
  delay(1000);
}
