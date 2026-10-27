/**
 **************************************************
 *
 * @file        4.2_Auto_Scroll_Text_on_your_own.ino
 * @brief       Variation of the 4.2 Auto scroll text example that makes the message travel from left to right instead
 *              of right to left.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Borna Zelenika @ soldered.com
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
  Scroll the text one position to the right, creating a moving effect.
  */
  lcd.scrollDisplayRight();

  /*
  Wait a short time before the next movement to control scrolling speed.
  */
  delay(scrollDelay);
}
