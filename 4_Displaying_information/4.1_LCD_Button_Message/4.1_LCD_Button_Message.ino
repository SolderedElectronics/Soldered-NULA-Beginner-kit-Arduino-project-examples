/**
 **************************************************
 *
 * @file        LCD_Button_Message.ino
 * @brief       Example that shows how to display a message on an LCD when a button is pressed.
 *              The LCD is controlled using the Soldered LCD library. 
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************/

#include "LCD-SOLDERED.h"   // Include the Soldered LCD library

/*
Create an LCD object. 
The parameters (16, 2) mean the LCD has 16 columns and 2 rows.
*/
LCD lcd(16,2);

/*
This is a variable to which we pass the number of the pin that we connected the button to. 
On the NULA board, IO3 corresponds to pin number 3.
*/
const int BUTTON_PIN = 3;

void setup() {
  /*
  Configure the button pin as INPUT, since it will read the state of the button. 
  */
  pinMode(BUTTON_PIN, INPUT); 

  /*
  Initialize the LCD so it is ready to use.
  */
  lcd.begin();

  /*
  Turn on the LCD backlight (if the display module supports it). 
  */
  lcd.backlight(); 
}

void loop() {
  /*
  digitalRead() is used to check if the button is pressed (HIGH) or not pressed (LOW).
  */
  bool reading = digitalRead(BUTTON_PIN);

  /*
  Clear the LCD before writing a new message. 
  Without this, characters from the last message might remain on the display.
  */
  lcd.clear();  

  /*
  If the button is pressed, show a message on the first row of the LCD. 
  */
  if (reading == HIGH) {
    lcd.setCursor(0,0);                 // Set cursor to column 0, row 0
    lcd.print(F("Button pressed!"));    // Print the message
  } 
  else {
    /*
    If the button is not pressed, we just clear the screen. 
    (The lcd.clear() above already handles this, but we call it again to ensure the display stays blank.)
    */
    lcd.setCursor(0,0);
    lcd.clear();
  }

  /*
  Small delay to reduce flickering on the display and prevent the loop from running too fast.
  */
  delay(50); 
}
