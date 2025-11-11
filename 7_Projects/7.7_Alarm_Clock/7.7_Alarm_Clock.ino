/**
 **************************************************
 *
 * @file        7_7_Alarm_Clock.ino
 * @brief       Wi-Fi-connected alarm clock using the NULA MINI.
 *              The device connects to Wi-Fi, gets real-time data from an NTP server,
 *              and shows the current time on a Qwiic LCD display.
 *              Two buttons (with external pull-down resistors) set the alarm hour and minute.
 *              When the alarm time is reached, a passive buzzer plays a tone until a button is pressed.
 *              For details and connection diagrams, see the example documentation: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

#include "LCD-SOLDERED.h"
#include <WiFi.h>
#include <time.h>

const char* WIFI_SSID = "your ssid";
const char* WIFI_PASS = "your password";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;     // Adjust to your timezone
const int daylightOffset_sec = 0; // Set to 3600 if you have DST

LCD lcd(16, 2); // Qwiic 16x2 LCD

const int BUTTON_HOUR = 2;  // Hour set button
const int BUTTON_MIN  = 3;  // Minute set button
const int BUZZER_PIN  = 4;  // Passive buzzer pin

int alarmHour = 7;
int alarmMinute = 0;
bool alarmTriggered = false;

// Debounce logic
bool lastHourState = LOW;
bool lastMinState = LOW;
unsigned long lastHourChangeMs = 0;
unsigned long lastMinChangeMs = 0;
const unsigned long debounceMs = 25;

unsigned long lastSync = 0;
const unsigned long syncInterval = 60000; // Sync every 60 seconds

void getLocalTimeData(struct tm *timeinfo) {
  if (!getLocalTime(timeinfo)) {
    Serial.println("Failed to obtain time from NTP");
  }
}

// Play tone on passive buzzer
void beepAlarm() {
  for (int i = 0; i < 5; i++) {
    tone(BUZZER_PIN, 1000); // 1 kHz tone
    delay(300);
    noTone(BUZZER_PIN);
    delay(100);
  }
}

void setup() {
  Serial.begin(115200);

  lcd.begin();
  lcd.backlight();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_HOUR, INPUT); // external pull-down
  pinMode(BUTTON_MIN, INPUT);  // external pull-down

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  lcd.clear();
  lcd.print("WiFi connected");
  delay(1000);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  lcd.clear();
  lcd.print("Time synced!");
  delay(1000);
  lcd.clear();
}

void loop() {
  struct tm timeinfo;
  getLocalTimeData(&timeinfo);

  // Display current time
  char timeStr[6];
  sprintf(timeStr, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(timeStr);

  // Display alarm
  lcd.setCursor(0, 1);
  lcd.print("Alarm ");
  if (alarmHour < 10) lcd.print("0");
  lcd.print(alarmHour);
  lcd.print(":");
  if (alarmMinute < 10) lcd.print("0");
  lcd.print(alarmMinute);

  // Handle hour button
  bool hourReading = digitalRead(BUTTON_HOUR);
  unsigned long now = millis();
  if (hourReading != lastHourState && (now - lastHourChangeMs) > debounceMs) {
    lastHourChangeMs = now;
    if (lastHourState == LOW && hourReading == HIGH) {
      alarmHour++;
      if (alarmHour > 23) alarmHour = 0;
    }
    lastHourState = hourReading;
  }

  // Handle minute button
  bool minReading = digitalRead(BUTTON_MIN);
  if (minReading != lastMinState && (now - lastMinChangeMs) > debounceMs) {
    lastMinChangeMs = now;
    if (lastMinState == LOW && minReading == HIGH) {
      alarmMinute++;
      if (alarmMinute > 59) alarmMinute = 0;
    }
    lastMinState = minReading;
  }

  // Check if it's time for alarm
  if (timeinfo.tm_hour == alarmHour && timeinfo.tm_min == alarmMinute && !alarmTriggered) {
    alarmTriggered = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ALARM!");
    Serial.println("Alarm Triggered!");
    beepAlarm();
  }

  // Stop alarm if button pressed
  if (alarmTriggered && (hourReading == HIGH || minReading == HIGH)) {
    alarmTriggered = false;
    lcd.clear();
  }

  // Re-sync time occasionally
  if (millis() - lastSync > syncInterval) {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    lastSync = millis();
  }

  delay(100);
}
