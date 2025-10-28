/**
 **************************************************
 *
 * @file        LCD_Three_Buttons.ino
 * @brief       Example that shows how to use three buttons with an LCD:
 *              - Button 1 displays a message only while pressed.
 *              - Button 2 increments and shows a counter.
 *              - Button 3 shows a progress bar animation.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************/

#include "LCD-SOLDERED.h"   // Include the Soldered LCD library

/*
Create an LCD object.
The parameters (16, 2) mean the LCD has 16 columns and 2 rows.
*/
LCD lcd(16, 2);   

/*
These are the button pins:
- Button 1 shows a temporary message while pressed.
- Button 2 increments a counter.
- Button 3 shows a progress bar animation.
*/
const int BTN1 = 3;  
const int BTN2 = 4;  
const int BTN3 = 5;  

/*
This variable stores the counter value for Button 2.
*/
int counter = 0;

/*
This variable stores the last state of Button 2. 
It helps us detect when the button is pressed (LOW → HIGH) to increment the counter only once per press.
*/
bool lastBtn2State = LOW;    

void setup() {
  /*
  Configure all three button pins as INPUT, since they will read the state of the buttons.
  */
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(BTN3, INPUT);

  /*
  Initialize the LCD so it is ready to use.
  */
  lcd.begin();

  /*
  Uncomment this line if your LCD module has a backlight and you want to enable it.
  */
  // lcd.backlight(); 
}

void loop() {
  /*
  Read the current state of all three buttons.
  HIGH = button pressed, LOW = button not pressed.
  */
  bool btn1State = digitalRead(BTN1);
  bool btn2State = digitalRead(BTN2);
  bool btn3State = digitalRead(BTN3);

  /*
  Clear the LCD before writing a new message each loop iteration.
  */
  lcd.clear();   

  // --- Button 3 has the highest priority (progress bar) ---
  if (btn3State == HIGH) {
    static int pos = 0;      // Position of the progress bar (static keeps value between loops)

    lcd.setCursor(0, 0);
    lcd.print("Loading...");
    lcd.setCursor(0, 1);

    /*
    Draw the progress bar using '#' characters.
    */
    for (int i = 0; i < pos; i++) {
      lcd.print("#");
    }

    /*
    Move the progress bar one step forward each time loop runs.
    Reset to 0 when it reaches the end of the line (16 characters).
    */
    pos++;
    if (pos > 16) pos = 0;  

    /*
    Delay controls the speed of the animation.
    */
    delay(150);              
  }

  // --- Button 2 shows counter ---
  else if (btn2State == HIGH) {
    /*
    Only increase counter when button is pressed down (transition from LOW → HIGH).
    */
    if (lastBtn2State == LOW) {
      counter++;
    }

    lcd.setCursor(0, 0);
    lcd.print("Counter:");
    lcd.setCursor(0, 1);
    lcd.print(counter);

    /*
    Small delay to avoid bouncing effects and multiple increments per press.
    */
    delay(200);  
  }

  // --- Button 1 shows message while pressed ---
  else if (btn1State == HIGH) {
    lcd.setCursor(0, 0);
    lcd.print("Button 1");
    lcd.setCursor(0, 1);
    lcd.print("is pressed!");
  }

  // --- Nothing pressed ---
  else {
    lcd.setCursor(0, 0);
    lcd.print("Press a button");
  }

  /*
  Save the state of Button 2 to check for transitions on the next loop iteration.
  */
  lastBtn2State = btn2State;

  /*
  Small delay to reduce flicker and CPU usage.
  */
  delay(50);
}
