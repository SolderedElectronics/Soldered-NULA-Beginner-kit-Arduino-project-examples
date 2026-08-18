/**
 **************************************************
 *
 * @file        7.6_Morse_code_transmitter.ino
 * @brief       Project that turns text you type into the Serial Monitor into Morse code and blinks it out on an LED.
 *              Morse code represents every letter as a pattern of short and long signals, called dots and dashes.
 *              Along the way the example introduces three new ideas: reading text from the Serial Monitor, storing a
 *              lookup table, and writing your own functions to keep a longer program readable.
 *              For details, connection diagram and more, check out the example documentation at: <link placeholder>
 * @author      Soldered
 ***************************************************
 */

/*
This is a variable to which we pass the number of pin that we had connected the LED to.
The NULA board has a pin naming logic as follows: IO2, where 2 is the number that we give to the variable.
If you wish to use a different pin, make sure you are using a IO__ marked pin.
*/
const int LED_PIN = 2;

/*
Morse code does not measure its signals in milliseconds but in units, and every other duration is a multiple of one
unit. That is why only the first value below is a real number and the rest are calculated from it: change DOT_DURATION
alone and the whole transmission speeds up or slows down while staying correct Morse.
The standard proportions are a dash three units long, one unit of silence between the dots and dashes of a letter,
three units between letters, and seven units between words.
*/
const int DOT_DURATION = 300;
const int DASH_DURATION = DOT_DURATION * 3;
const int SYMBOL_GAP = DOT_DURATION;
const int LETTER_GAP = DOT_DURATION * 3;
const int WORD_GAP = DOT_DURATION * 7;

/*
A struct lets us bundle several values into one thing of our own making. Here each MorseEntry holds a character and the
dots and dashes that stand for it, so the two always travel together.
*/
struct MorseEntry { char letter; const char *code; };

/*
This is our lookup table: an array of the structs above, one for every character we can send. A lookup table is simply
a list we search through instead of writing out dozens of if statements, and it has the nice property that adding a new
character means adding one line here and changing nothing else.
The last entry maps a space to a space, which is how we recognise the gap between two words.
*/
const MorseEntry morseTable[] = {
  {'A', ".-"},   {'B', "-..."}, {'C', "-.-."}, {'D', "-.."},  {'E', "."},
  {'F', "..-."}, {'G', "--."},  {'H', "...."}, {'I', ".."},   {'J', ".---"},
  {'K', "-.-"},  {'L', ".-.."}, {'M', "--"},   {'N', "-."},   {'O', "---"},
  {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."},  {'S', "..."},  {'T', "-"},
  {'U', "..-"},  {'V', "...-"}, {'W', ".--"},  {'X', "-..-"}, {'Y', "-.--"},
  {'Z', "--.."},
  {'1', ".----"},{'2', "..---"},{'3', "...--"},{'4', "....-"},{'5', "....."},
  {'6', "-...."},{'7', "--..."},{'8', "---.."},{'9', "----."},{'0', "-----"},
  {' ', " "}
};

/*
Here we work out how many entries the table above actually has, instead of counting them by hand. sizeof() tells us how
much memory something takes up, so the size of the whole table divided by the size of one entry gives us the number of
entries. Written this way the number stays correct even after you add a character of your own.
*/
const int MORSE_COUNT = sizeof(morseTable) / sizeof(MorseEntry);

/*
This is a function we wrote ourselves. Up to now our sketches only used setup() and loop() plus functions from
libraries, but as a program grows it helps to give a job a name of its own.
This one takes a character and hands back the dots and dashes that stand for it. toupper() turns a lowercase letter
into an uppercase one, so that typing "sos" works just as well as "SOS". Then we walk through the table from the start
and return as soon as we find a match. If the character is not in the table at all we return empty text, which the
transmitting function below simply skips over.
*/
const char* getMorseCode(char c) {
  c = toupper(c);
  for (int i = 0; i < MORSE_COUNT; i++)
    if (morseTable[i].letter == c) return morseTable[i].code;
  return "";
}

/*
This function blinks out a single dot or dash. It picks the right duration for the symbol it was given, switches the
LED on for exactly that long, and then switches it off and waits one more unit, which is the silence that separates one
symbol from the next.
*/
void blinkSymbol(char symbol) {
  int duration;
  if(symbol=='.'){
    duration=DOT_DURATION;
  }
  else{
    duration=DASH_DURATION;
  }
  digitalWrite(LED_PIN, HIGH);
  delay(duration);
  digitalWrite(LED_PIN, LOW);
  delay(SYMBOL_GAP);
}

/*
This function transmits a whole piece of text. It does the work twice over: first it prints the translation to the
Serial Monitor so you can read along, and then it blinks the same thing out on the LED.
The "&" in the parameter means the text is handed over without being copied, which saves memory on longer messages.
*/
void transmitText(const String &text) {
  Serial.println("\n--- TRANSMITTING ---");
  Serial.print("Text:   "); Serial.println(text);
  Serial.print("Morse:  ");

  /*
  This first loop walks through the text one character at a time. length() tells us how many characters there are, and
  text[i] gives us the one at position i, counting from zero. For each of them we look up the code and print it.
  */
  for (unsigned int i = 0; i < text.length(); i++) {
    const char *code = getMorseCode(text[i]);
    Serial.print(code);
    Serial.print(" ");
  }
  Serial.println("\n--------------------");

  /*
  This second loop walks through the very same text again, but this time it blinks instead of printing.
  A space is not blinked at all, it is a pause, so when we find one we wait a word gap and skip the rest of this pass
  with continue. Otherwise the inner loop blinks the symbols of the letter one by one, and once they are done we wait a
  letter gap before moving on to the next letter.
  */
  for (unsigned int i = 0; i < text.length(); i++) {
    const char *code = getMorseCode(text[i]);
    if (*code == ' ') { 
      delay(WORD_GAP); 
      continue; 
    }
    for (int j = 0; code[j]; j++){ 
      blinkSymbol(code[j]);
    }
    delay(LETTER_GAP);
  }

  Serial.println("\nTransmission complete!\n");
}

void setup() {

  /*
  Serial.begin() establishes serial communication between your board and your computer via a USB cable. In this example
  we use it in both directions for the first time: to print messages out, and to read the text you type in.
  */
  Serial.begin(115200);

  /*
  pinMode() is a function that configures the specified pin to behave either as an input or in this case as an output.
  As our pin needs to turn on the LED, we will put the pin in OUTPUT mode.
  */
  pinMode(LED_PIN, OUTPUT);

  //Invite the user to type something. Type into the box at the top of the Serial Monitor and press Enter.
  Serial.println("Enter text to send via Morse code:");
}

void loop() {

  /*
  Serial.available() tells us how many characters have arrived from the computer and are waiting to be read. It returns
  zero when nothing has been typed, so this if statement is how we do nothing at all until the user sends us something.
  */
  if (Serial.available()) {

    /*
    readStringUntil() collects the arriving characters into text and stops at the character we name, in this case the
    newline that the Serial Monitor sends when you press Enter.
    trim() then removes any stray spaces or leftover line endings from both ends, which is worth doing because
    different systems end their lines slightly differently.
    */
    String input = Serial.readStringUntil('\n');
    input.trim();

    /*
    Finally we check that something is actually left after trimming, so that pressing Enter on an empty line does not
    start a transmission, and hand the text over to our own function above.
    */
    if (input.length() > 0) transmitText(input);
  }
}
