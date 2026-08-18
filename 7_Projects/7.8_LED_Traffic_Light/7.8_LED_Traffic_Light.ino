/**
 **************************************************
 *
 * @file        7.8_LED_Traffic_Light.ino
 * @brief       Project that simulates a traffic light with three LEDs, running through green, blinking green, orange,
 *              red and red together with orange, just like the lights on many European roads.
 *              It introduces the finite state machine, or FSM, which is a very common way of writing programs that move
 *              through a fixed series of steps without ever using delay() to wait.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
These are the variables to which we pass the numbers of pins that we had connected the three LEDs to.
The NULA board has a pin naming logic as follows: IO4, where 4 is the number that we give to the variable.
If you wish to use a different pin, make sure you are using a IO__ marked pin.
*/
const int LED_GREEN = 4;
const int LED_ORANGE = 3;
const int LED_RED = 2;

/*
An enum lets us give names to a fixed set of values. Without it we would have to remember that state 0 means green and
state 3 means red, and a mistake there would be very easy to make and very hard to spot. With it we can write GREEN and
RED instead, and the program reads almost like a description of a real traffic light.
Each of these names is one state that our traffic light can be in, and it can only ever be in one of them at a time.
*/
enum TrafficState {
  GREEN,
  GREEN_BLINK,
  ORANGE,
  RED,
  RED_ORANGE
};

/*
This variable holds the state we are in right now. Together with the two below it, it is the whole memory of our state
machine: "lastChange" remembers the moment we entered the current state, and "stateDuration" holds how long we mean to
stay in it. That is all a finite state machine needs: where am I, since when, and for how long.
*/
TrafficState state = GREEN;
unsigned long lastChange = 0;
unsigned long stateDuration = 0;

/*
These three variables are only used by the blinking green state. "greenOn" remembers whether the green LED is currently
lit, "blinkInterval" is how long it stays that way before flipping, and "lastBlink" remembers when it last flipped.
Feel free to experiment with the interval to make the blinking faster or slower.
*/
bool greenOn = true;
const unsigned long blinkInterval = 400;
unsigned long lastBlink = 0;

void setup() {

  /*
  pinMode() is a function that configures the specified pin to behave either as an input or in this case as an output.
  All three LEDs are things we write to, so all three pins go into OUTPUT mode.
  */
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_ORANGE, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  /*
  Here we set the starting state of our machine. millis() returns the number of milliseconds passed since the board
  began running the current program, so storing it now means "the green state began at this moment". The duration says
  we mean to stay green for 5000 milliseconds, which is five seconds.
  */
  state = GREEN;
  lastChange = millis();
  stateDuration = 5000;
}

void loop() {

  /*
  We read the clock once at the top of the loop and use that one value everywhere below, which keeps all the comparisons
  in this pass consistent with each other.
  */
  unsigned long now = millis();

  /*
  A switch statement is a tidier way of writing a long chain of if statements when we are comparing one variable against
  several fixed values. Here it lets us give each state its own block of code, and only the block belonging to the
  current state runs.
  Notice that every block does the same two things: it sets the LEDs the way this state wants them, and then it checks
  whether its time is up. If it is, it names the next state, remembers the current moment as the new starting point, and
  sets how long the next state should last. That pattern repeating five times is the entire state machine.
  The break at the end of each block tells the switch statement to stop there instead of falling through into the next
  one.
  */
  switch (state) {

    case GREEN:

      //Green on, everything else off.
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_ORANGE, LOW);
      digitalWrite(LED_RED, LOW);

      //After five seconds, move on to the blinking green state and give it three seconds.
      if (now - lastChange >= stateDuration) {
        state = GREEN_BLINK;
        lastChange = now;
        stateDuration = 3000;
      }
      break;

    case GREEN_BLINK:

      /*
      Here we leave the green LED alone, because the blinking below is what decides whether it is on or off. The other
      two stay off.
      */
      digitalWrite(LED_ORANGE, LOW);
      digitalWrite(LED_RED, LOW);

      /*
      This is the blinking itself. Every time the interval has passed we flip greenOn to its opposite value with the "!"
      operator and write the new value to the pin. Because this runs on the clock rather than on delay(), the state
      machine above keeps working the whole time the LED is blinking.
      */
      if (now - lastBlink >= blinkInterval) {
        greenOn = !greenOn;
        digitalWrite(LED_GREEN, greenOn);
        lastBlink = now;
      }

      //After three seconds of blinking, move on to orange and give it two seconds.
      if (now - lastChange >= stateDuration) {
        state = ORANGE;
        lastChange = now;
        stateDuration = 2000;
      }
      break;

    case ORANGE:

      //Orange on, everything else off.
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_ORANGE, HIGH);
      digitalWrite(LED_RED, LOW);

      //After two seconds, move on to red and give it five seconds.
      if (now - lastChange >= stateDuration) {
        state = RED;
        lastChange = now;
        stateDuration = 5000;
      }
      break;

    case RED:

      //Red on, everything else off.
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_ORANGE, LOW);
      digitalWrite(LED_RED, HIGH);

      //After five seconds, move on to red together with orange and give it two seconds.
      if (now - lastChange >= stateDuration) {
        state = RED_ORANGE;
        lastChange = now;
        stateDuration = 2000;
      }
      break;

    case RED_ORANGE:

      /*
      Red and orange lit at the same time, which on many European traffic lights is the warning that green is coming.
      */
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_ORANGE, HIGH);
      digitalWrite(LED_RED, HIGH);

      //After two seconds we are back at the start, and the whole cycle begins again.
      if (now - lastChange >= stateDuration) {
        state = GREEN;
        lastChange = now;
        stateDuration = 5000;
      }
      break;
  }
}
