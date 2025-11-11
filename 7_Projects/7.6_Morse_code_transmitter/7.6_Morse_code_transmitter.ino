/**
 * @file 7_6_morse_code_transmitter.ino
 * @brief Morse code transmitter using LED on NULA MINI.
 *        This project converts text entered in the Serial Monitor into Morse code
 *        and blinks it using an LED. Each dot and dash corresponds to timed LED flashes.
 *        Demonstrates character mapping, serial input, and timing control.
 * @author Soldered
 */
#define LED_PIN 2  // LED output pin

int DOT_DURATION = 300;                // base time unit (ms)
int DASH_DURATION = DOT_DURATION * 3;
int SYMBOL_GAP = DOT_DURATION;         // gap between dots/dashes
int LETTER_GAP = DOT_DURATION * 3;     // gap between letters
int WORD_GAP = DOT_DURATION * 7;       // gap between words


struct MorseEntry { char letter; const char *code; };
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
const int MORSE_COUNT = sizeof(morseTable) / sizeof(MorseEntry);

// Lookup function
const char* getMorseCode(char c) {
  c = toupper(c);
  for (int i = 0; i < MORSE_COUNT; i++)
    if (morseTable[i].letter == c) return morseTable[i].code;
  return "";
}

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

void transmitText(const String &text) {
  Serial.println("\n--- TRANSMITTING ---");
  Serial.print("Text:   "); Serial.println(text);
  Serial.print("Morse:  ");

  // Display Morse translation in serial
  for (unsigned int i = 0; i < text.length(); i++) {
    const char *code = getMorseCode(text[i]);
    Serial.print(code);
    Serial.print(" ");
  }
  Serial.println("\n--------------------");

  // Blink Morse code on LED
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
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Enter text to send via Morse code:");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() > 0) transmitText(input);
  }
}
