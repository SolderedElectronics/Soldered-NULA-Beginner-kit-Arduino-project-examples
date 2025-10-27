#include "LCD-SOLDERED.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

const char* ssid = "Soldered";
const char* password = "dasduino";
LCD lcd(16,2);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000);
void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Connecting WiFi");
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print(".");
  }

  lcd.clear();
  lcd.print("WiFi Connected");
  delay(1000);
  lcd.clear();

  timeClient.begin();
}

void loop() {
  timeClient.update();

  // Get current time (HH:MM:SS)
  String formattedTime = timeClient.getFormattedTime();

  lcd.setCursor(0, 0);
  lcd.print("Time:");
  lcd.setCursor(0, 1);
  lcd.print(formattedTime);

  delay(1000);  // Update every second

}
