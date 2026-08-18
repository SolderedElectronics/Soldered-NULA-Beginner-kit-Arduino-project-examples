
/**
 **************************************************
 *
 * @file        4.1_Print_Message.ino
 * @brief       Example that shows how to display a message on an LCD screen.
 *              The LCD is controlled using the Soldered LCD library.
 *              This example demonstrates the basics of initializing the display and printing text.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

#include "LCD-SOLDERED.h"   // Include the Soldered LCD library

/*
Create an LCD object with 16 columns and 2 rows.
These numbers correspond to the width and height of the display.
*/
LCD lcd(16, 2);

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
  /*
  Nothing needs to run repeatedly in this example.
  The message remains visible until the board is reset or powered off.
  */
}
