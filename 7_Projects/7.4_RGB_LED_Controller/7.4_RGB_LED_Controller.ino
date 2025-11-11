/**
 **************************************************
 *
 * @file        7.4_RGB_LED_Controller.ino
 * @brief       Example that uses a photoresistor (LDR) to control the RGB LED color
 *              smoothly. The LED color changes gradually from red (in darkness)
 *              through green and blue to white (in bright light).
 *
 *              Demonstrates mapping one analog sensor to multiple PWM outputs
 *              for full RGB blending.
 *
 * @author      Soldered
 ***************************************************
 */

#define LDR_PIN   5   // Analog input pin for photoresistor
#define RED_PIN   2   // PWM pin for red LED channel
#define GREEN_PIN 3   // PWM pin for green LED channel
#define BLUE_PIN  4   // PWM pin for blue LED channel

int ldrValue = 0;     // Photoresistor reading (0–4095)
int r = 0, g = 0, b = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.println("RGB LED Controller with full-spectrum color mapping started");
}

void loop() {
  // --- Read light level from photoresistor ---
  ldrValue = analogRead(LDR_PIN);
  Serial.print("LDR value: ");
  Serial.println(ldrValue);

  // --- Map LDR value (0–4095) to RGB channels ---
  if (ldrValue <= 1365) {
    // Dark → transition from Red (255,0,0) to Green (0,255,0)
    r = map(ldrValue, 0, 1365, 255, 0);
    g = map(ldrValue, 0, 1365, 0, 255);
    b = 0;
  }
  else if (ldrValue <= 2730) {
    // Medium → transition from Green (0,255,0) to Blue (0,0,255)
    r = 0;
    g = map(ldrValue, 1366, 2730, 255, 0);
    b = map(ldrValue, 1366, 2730, 0, 255);
  }
  else {
    // Bright → transition from Blue (0,0,255) to White (255,255,255)
    r = map(ldrValue, 2731, 4095, 0, 255);
    g = map(ldrValue, 2731, 4095, 0, 255);
    b = 255;
  }

  // --- Write PWM values to RGB pins ---
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);

  // --- Debug output ---
  Serial.print("RGB: ");
  Serial.print(r); Serial.print(", ");
  Serial.print(g); Serial.print(", ");
  Serial.println(b);

  delay(100);
}
