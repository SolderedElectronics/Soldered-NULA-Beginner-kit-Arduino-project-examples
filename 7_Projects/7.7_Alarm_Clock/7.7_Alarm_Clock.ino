/**
 **************************************************
 *
 * @file        7.7_Alarm_Clock.ino
 * @brief       Project that builds a Wi-Fi connected alarm clock. The board joins your network, asks an NTP server on
 *              the internet what the time is, and shows the current time on the LCD display. Two buttons set the alarm
 *              hour and minute, and when the alarm time is reached the buzzer sounds a series of beeps.
 *              It brings together the LCD from section 4, the button debouncing from section 2.2, the buzzer from
 *              section 2.4 and the Wi-Fi connection from section 6.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
The Soldered library for the LCD display.
*/
#include "LCD-SOLDERED.h"

/*
The WiFi library contains everything needed to join a Wi-Fi network.
*/
#include <WiFi.h>

/*
The time library gives us the functions for working with dates and times, and for asking an NTP server what the time is.
NTP stands for Network Time Protocol, and it is how nearly every device on the internet keeps its clock correct.
*/
#include <time.h>

/*
These two variables hold the name of your Wi-Fi network (the SSID) and its password. Replace the text between the
quotation marks with your own network details.
*/
const char* WIFI_SSID = "your ssid";
const char* WIFI_PASS = "your password";

/*
This is the address of the NTP server we ask for the time. pool.ntp.org is a free, worldwide service that hands your
request to whichever time server is nearest to you, so it is a good default to leave as it is.
*/
const char* ntpServer = "pool.ntp.org";

/*
NTP servers always answer in UTC, the world reference time, so we have to tell the board how far our own time zone sits
from it. The offset is given in seconds, so one hour is 3600. Croatia in winter is one hour ahead of UTC, which would
be 3600 here.
The second value is the extra offset for daylight saving time. Set it to 3600 during summer time in a country that uses
it, and leave it at 0 otherwise.
*/
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;

/*
Here we create our display object, which we named "lcd". An object is our way of talking to the display: every function
we call on it, we call through this name. It needs no pin numbers because it uses easyC, which is Soldered's name for an
I2C connection over a single cable, and I2C always uses the same two pins on the board (IO6 and IO7 on the NULA board).
*/
LCD lcd(16, 2); // Qwiic 16x2 LCD

/*
These are the variables to which we pass the numbers of pins that we had connected the two BUTTONS and the buzzer to.
One button counts the alarm hour up, the other counts the alarm minute up.
The NULA board has a pin naming logic as follows: IO2, where 2 is the number that we give to the variable.
*/
const int BUTTON_HOUR = 2;
const int BUTTON_MIN  = 3;
const int BUZZER_PIN  = 4;

/*
These variables hold the alarm time we are counting towards, starting at 07:00, and remember whether the alarm has
already gone off, so that it sounds once and not over and over during the same minute.
*/
int alarmHour = 7;
int alarmMinute = 0;
bool alarmTriggered = false;

/*
This variable remembers which minute the alarm last went off in. Without it, stopping the alarm with a button press
inside the very minute it fired would let it fire again immediately, over and over until the minute was out. We start it
at -1, a value no real minute can have, so that the first alarm is never blocked.
*/
int lastAlarmMinute = -1;

/*
These are the variables used for button debouncing, the same technique explained in example 2.2. Because we have two
buttons here, each one needs its own pair of variables: one remembering its previous state and one remembering when
that state last changed.
*/
bool lastHourState = HIGH;
bool lastMinState = HIGH;
unsigned long lastHourChangeMs = 0;
unsigned long lastMinChangeMs = 0;
const unsigned long debounceMs = 25;

/*
These two variables control how often we ask the NTP server for the time again. A board's own clock drifts slowly, so
checking in every once in a while keeps it accurate.
*/
unsigned long lastSync = 0;
const unsigned long syncInterval = 60000;

/*
This is a function we wrote ourselves. It asks for the current time and reports whether it succeeded.
getLocalTime() fills in the structure we hand it and returns true on success or false on failure. On a failure the
structure is left exactly as it was, which would be full of meaningless values, so we pass that answer back to the
caller and let it decide what to do rather than using numbers we cannot trust.
The "*" in the parameter means we are handing over the location of the structure rather than a copy of it, which is what
allows the function to fill in our own variable instead of one of its own.
*/
bool getLocalTimeData(struct tm *timeinfo) {
  if (!getLocalTime(timeinfo)) {
    Serial.println("Failed to obtain time from NTP");
    return false;
  }
  return true;
}

/*
This function sounds the alarm. tone() makes the buzzer produce a sound of the given frequency, 1000 Hz in this case,
and noTone() stops it again. Repeating that five times in a row is what turns one long sound into a series of beeps.
Feel free to experiment with the frequency, the number of repeats and the two delays.
*/
void beepAlarm() {
  for (int i = 0; i < 5; i++) {
    tone(BUZZER_PIN, 1000);
    delay(300);
    noTone(BUZZER_PIN);
    delay(100);
  }
}

void setup() {

  /*
  Serial.begin() establishes serial communication between your board and your computer via a USB cable. We use it here
  to follow the connection to the network and to the time server.
  */
  Serial.begin(115200);

  /*
  Here we prepare the display. begin() starts the communication with it and backlight() turns on its light so the text
  is readable.
  */
  lcd.begin();
  lcd.backlight();

  /*
  pinMode() is a function that configures the specified pin to behave either as an input or as an output.
  The buzzer is something we write to, so it goes into OUTPUT mode, while the two buttons are things we read, so they
  go into INPUT_PULLUP mode. INPUT_PULLUP switches on a small resistor inside the chip that ties each pin to 3.3V,
  which means a pin sits at HIGH while its button is released and goes LOW while it is pressed.
  */
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_HOUR, INPUT_PULLUP);
  pinMode(BUTTON_MIN, INPUT_PULLUP);

  /*
  clear() wipes anything left on the screen from before, and setCursor() chooses where the next text will appear: the
  first number is the column and the second is the row, both counting from zero.
  */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  /*
  WiFi.begin() starts the connection attempt. The function only starts the process, it does not wait for it to finish,
  so we wait for it ourselves in the loop below. WiFi.status() tells us the current state and WL_CONNECTED is the value
  it reports once we are online.
  */
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  lcd.clear();
  lcd.print("WiFi connected");
  delay(1000);

  /*
  configTime() tells the board which NTP server to use and how far our time zone is from UTC, and starts the first
  request for the time in the background. It returns straight away, before any answer has arrived.
  */
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  /*
  So here we wait for that first answer, because until it arrives the board has no idea what time it is and would show
  nonsense on the display. We give it up to ten seconds and then carry on either way, so that a missing internet
  connection cannot leave the clock stuck here forever.
  */
  lcd.clear();
  lcd.print("Syncing time...");
  struct tm startupTime;
  unsigned long syncStart = millis();
  bool timeReady = false;
  while (!timeReady && millis() - syncStart < 10000) {
    timeReady = getLocalTime(&startupTime);
    delay(200);
  }

  lcd.clear();
  if (timeReady) {
    Serial.println("Time synced!");
    lcd.print("Time synced!");
    lastSync = millis();
  } else {
    Serial.println("Time sync failed, the clock will keep trying.");
    lcd.print("Sync failed!");
  }
  delay(1000);
  lcd.clear();
}

void loop() {

  /*
  Here we ask for the current time through our own function above. A struct tm holds a full date and time split into
  separate fields, of which we only use two: tm_hour and tm_min.
  */
  struct tm timeinfo;

  /*
  If we could not read the time, there is nothing sensible to show and certainly nothing to compare the alarm against,
  so we wait a moment and try again on the next pass. return ends this pass through loop() early, and because the board
  calls loop() again immediately we are straight back at the next attempt.
  */
  if (!getLocalTimeData(&timeinfo)) {
    delay(1000);
    return;
  }

  /*
  sprintf() builds a piece of text out of several values, following a pattern we give it. "%02d" means "a whole number
  written with at least two digits, padded with a zero if needed", which is what turns 7 minutes past into "07" instead
  of "7". The result is stored in the timeStr array and then printed to the display.
  */
  char timeStr[6];
  sprintf(timeStr, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(timeStr);

  /*
  On the second line we show the alarm time. Here we pad the numbers by hand instead of using sprintf(): if the value is
  smaller than 10 we print a zero in front of it first.
  */
  lcd.setCursor(0, 1);
  lcd.print("Alarm ");
  if (alarmHour < 10) lcd.print("0");
  lcd.print(alarmHour);
  lcd.print(":");
  if (alarmMinute < 10) lcd.print("0");
  lcd.print(alarmMinute);

  /*
  This is the debouncing logic for the hour button, the same one explained in example 2.2. digitalRead() reads the value
  from the pin, millis() returns the number of milliseconds passed since the board began running the current program,
  and together they let us ignore any change that comes too soon after the previous one to be a real press.
  Because the buttons use pull-up resistors, the readings are the other way around from what you might expect: a press
  is the moment the reading goes from HIGH to LOW, and that is exactly the moment we count the hour up. After 23 we
  start over at 0, since there is no hour 24.
  */
  bool hourReading = digitalRead(BUTTON_HOUR);
  unsigned long now = millis();
  if (hourReading != lastHourState && (now - lastHourChangeMs) > debounceMs) {
    lastHourChangeMs = now;
    if (lastHourState == HIGH && hourReading == LOW) {
      alarmHour++;
      if (alarmHour > 23) alarmHour = 0;
    }
    lastHourState = hourReading;
  }

  /*
  The very same logic for the minute button, counting up to 59 before starting over.
  */
  bool minReading = digitalRead(BUTTON_MIN);
  if (minReading != lastMinState && (now - lastMinChangeMs) > debounceMs) {
    lastMinChangeMs = now;
    if (lastMinState == HIGH && minReading == LOW) {
      alarmMinute++;
      if (alarmMinute > 59) alarmMinute = 0;
    }
    lastMinState = minReading;
  }

  /*
  Here we check whether it is time for the alarm. All three conditions have to be true at once: the hour has to match,
  the minute has to match, and the alarm must not have gone off already. That last check is what the alarmTriggered
  variable is for, since without it the alarm would sound again and again for the whole minute.
  */
  if (timeinfo.tm_hour == alarmHour && timeinfo.tm_min == alarmMinute && !alarmTriggered
      && timeinfo.tm_min != lastAlarmMinute) {
    alarmTriggered = true;
    lastAlarmMinute = timeinfo.tm_min;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ALARM!");
    Serial.println("Alarm Triggered!");
    beepAlarm();
  }

  /*
  Pressing either button after the alarm has gone off clears the warning from the display and arms the alarm again for
  the next day.
  */
  if (alarmTriggered && (hourReading == LOW || minReading == LOW)) {
    alarmTriggered = false;
    lcd.clear();
  }

  /*
  And here we ask the NTP server for the time again every so often, so that the clock does not drift.
  */
  if (millis() - lastSync > syncInterval) {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    lastSync = millis();
  }

  /*
  A short pause so the display is not rewritten thousands of times per second. Keeping it short also keeps the buttons
  feeling responsive.
  */
  delay(100);
}
