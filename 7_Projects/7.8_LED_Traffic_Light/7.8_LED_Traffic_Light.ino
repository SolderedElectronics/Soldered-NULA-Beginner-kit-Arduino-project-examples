/**
 **************************************************
 *
 * @file        7_8_LED_Traffic_Light.ino
 * @brief       Example showing how to simulate a traffic light sequence using the NULA MINI.
 *              This project demonstrates how to use a finite state machine (FSM)
 *              to manage timed LED states for green, blinking green, orange, red,
 *              and red+orange transitions.
 *              For details and wiring diagrams, see the example documentation: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

const int LED_GREEN = 4;
const int LED_ORANGE = 3;
const int LED_RED = 2;

// Define the states of the traffic light
enum TrafficState {
  GREEN,
  GREEN_BLINK,
  ORANGE,
  RED,
  RED_ORANGE
};

TrafficState state = GREEN;
unsigned long lastChange = 0;
unsigned long stateDuration = 0;

// Blinking green timing
bool greenOn = true;
const unsigned long blinkInterval = 400; // 0.4 s blink interval
unsigned long lastBlink = 0;

void setup() {
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_ORANGE, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // Start in green state
  state = GREEN;
  lastChange = millis();
  stateDuration = 5000; // stay green for 5 s
}

void loop() {
  unsigned long now = millis();

  switch (state) {

    case GREEN:
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_ORANGE, LOW);
      digitalWrite(LED_RED, LOW);

      if (now - lastChange >= stateDuration) {
        state = GREEN_BLINK;
        lastChange = now;
        stateDuration = 3000; // blink green for 3 s
      }
      break;

    case GREEN_BLINK:
      digitalWrite(LED_ORANGE, LOW);
      digitalWrite(LED_RED, LOW);

      if (now - lastBlink >= blinkInterval) {
        greenOn = !greenOn;
        digitalWrite(LED_GREEN, greenOn);
        lastBlink = now;
      }

      if (now - lastChange >= stateDuration) {
        state = ORANGE;
        lastChange = now;
        stateDuration = 2000; // orange for 2 s
      }
      break;

    case ORANGE:
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_ORANGE, HIGH);
      digitalWrite(LED_RED, LOW);

      if (now - lastChange >= stateDuration) {
        state = RED;
        lastChange = now;
        stateDuration = 5000; // red for 5 s
      }
      break;

    case RED:
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_ORANGE, LOW);
      digitalWrite(LED_RED, HIGH);

      if (now - lastChange >= stateDuration) {
        state = RED_ORANGE;
        lastChange = now;
        stateDuration = 2000; // red + orange for 2 s
      }
      break;

    case RED_ORANGE:
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_ORANGE, HIGH);
      digitalWrite(LED_RED, HIGH);

      if (now - lastChange >= stateDuration) {
        state = GREEN;
        lastChange = now;
        stateDuration = 5000; // back to green for 5 s
      }
      break;
  }
}
