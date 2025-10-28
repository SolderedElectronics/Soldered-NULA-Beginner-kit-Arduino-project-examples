/**
 **************************************************
 *
 * @file        LCD_WiFi_Time_Display.ino
 * @brief       Example that shows how to connect to Wi-Fi and display the current time on an LCD using NTP:
 *              - Connects to a Wi-Fi network.
 *              - Fetches the current time from an online NTP (Network Time Protocol) server.
 *              - Continuously updates and displays the time on a 16x2 LCD.
 *              For more details about NTP and connection setup, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

#include "LCD-SOLDERED.h"   // Include the Soldered LCD library
#include <WiFi.h>           // Include Wi-Fi library to connect to a wireless network
#include <WiFiUdp.h>        // Include UDP library (used by NTP to communicate)
#include <NTPClient.h>      // Include NTP client library to get time from the internet

// Replace these values with your WiFi credentials
const char* ssid = "Your SSID";     
const char* password = "Your password";

/*
Create an LCD object with 16 columns and 2 rows.
*/
LCD lcd(16, 2);

/*
Create a UDP object and an NTP client object.
- ntpUDP: handles the low-level data exchange over the internet.
- timeClient: communicates with the NTP server to get the current time.
  Parameters:
    - "pool.ntp.org" is the server address (a public time server).
    - 3600 is the time offset in seconds (3600 = +1 hour, adjust for your timezone).
    - 60000 is the update interval in milliseconds (60 seconds).
*/
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000);

void setup() {
  /*
  Initialize the LCD display and turn on the backlight.
  */
  lcd.begin();
  lcd.backlight();

  /*
  Display a message while connecting to Wi-Fi.
  */
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  /*
  Start connecting to the Wi-Fi network using the given SSID and password.
  */
  WiFi.begin(ssid, password);

  /*
  Wait until the Wi-Fi is connected.
  While waiting, display dots on the LCD to show progress.
  */
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print(".");
  }

  /*
  Once connected, show a confirmation message.
  */
  lcd.clear();
  lcd.print("WiFi Connected");
  delay(1000);
  lcd.clear();

  /*
  Start the NTP client so it can begin requesting time updates.
  */
  timeClient.begin();
}

void loop() {
  /*
  Update the time from the NTP server.
  This contacts the server (if needed) and refreshes the time value.
  */
  timeClient.update();

  /*
  Get the current formatted time as a string (in the format HH:MM:SS).
  */
  String formattedTime = timeClient.getFormattedTime();

  /*
  Display the time on the LCD.
  */
  lcd.setCursor(0, 0);
  lcd.print("Time:");
  lcd.setCursor(0, 1);
  lcd.print(formattedTime);

  /*
  Wait one second before updating again.
  This creates a ticking clock effect.
  */
  delay(1000);
}
