/**
 **************************************************
 *
 * @file        4.2_Auto_Scroll_Text.ino
 * @brief       Example that shows how to automatically scroll text on a 16x2 LCD.
 *              The LCD is controlled using the Soldered LCD library.
 *              This example demonstrates how to use lcd.scrollDisplayLeft() and lcd.scrollDisplayRight()
 *              to move text smoothly across the screen.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

#include "LCD-SOLDERED.h"   // Include the Soldered LCD library

/*
Create an LCD object with 16 columns and 2 rows.
These parameters correspond to your display size.
*/
LCD lcd(16, 2);

/*
Create a message that will scroll across the display.
*/
String message = " Hello from NULA MINI! ";

/*
The speed of scrolling in milliseconds. Higher number = slower scroll.
*/
const int scrollDelay = 300;

void setup() {
  /*
  Initialize the LCD and turn on the backlight.
  */
  lcd.begin();
  lcd.backlight();

  /*
  Print the message once. Only the visible part will show initially.
  */
  lcd.print(message);
}

void loop() {
  /*
  Scroll the text one position to the left, creating a moving effect.
  */
  lcd.scrollDisplayLeft();

  /*
  Wait a short time before the next movement to control scrolling speed.
  */
  delay(scrollDelay);
}
