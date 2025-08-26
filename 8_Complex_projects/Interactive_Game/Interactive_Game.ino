#include <Wire.h>
#include <string.h>
#include "LCD-SOLDERED.h"   // Soldered LCD library

// ======================
// Hardware (your pins)
// ======================
const int BTN_UP_PIN   = 10;   // SPIN (INPUT_PULLDOWN; pressed = HIGH)
const int BTN_DOWN_PIN = 11;   // BET  (INPUT_PULLDOWN; pressed = HIGH)
const int SPK_PIN      = 18;   // Speaker / piezo
const int LED_R_PIN    = 17;   // RGB: Red
const int LED_G_PIN    = 26;   // RGB: Green
const int LED_B_PIN    = 27;   // RGB: Blue
const bool LED_ACTIVE_HIGH = true;

const int LCD_COLS = 16;
const int LCD_ROWS = 2;

LCD lcd(LCD_COLS, LCD_ROWS);

// ======================
// LED helpers
// ======================
inline void ledWritePin(int pin, bool on) {
  digitalWrite(pin, LED_ACTIVE_HIGH ? (on ? HIGH : LOW) : (on ? LOW : HIGH));
}
inline void setLED(bool r, bool g, bool b) {
  ledWritePin(LED_R_PIN, r);
  ledWritePin(LED_G_PIN, g);
  ledWritePin(LED_B_PIN, b);
}
inline void ledOff()      { setLED(false,false,false); }
inline void ledSpinning() { setLED(false,false,true); }  // blue
inline void ledWin()      { setLED(false,true,false); }  // green
inline void ledLose()     { setLED(true,false,false); }  // red

// ======================
// Symbols (no enums)
// ======================
const uint8_t SYM_CHERRY = 0;
const uint8_t SYM_LEMON  = 1;
const uint8_t SYM_ORANGE = 2;
const uint8_t SYM_PLUM   = 3;
const uint8_t SYM_BELL   = 4;
const uint8_t SYM_BAR    = 5;

// Weighted reels (more repeats = more common)
const uint8_t reel0[] = {
  SYM_CHERRY, SYM_LEMON,  SYM_ORANGE, SYM_PLUM,
  SYM_CHERRY, SYM_LEMON,  SYM_ORANGE, SYM_PLUM,
  SYM_BELL,   SYM_CHERRY, SYM_LEMON,  SYM_ORANGE,
  SYM_PLUM,   SYM_CHERRY, SYM_LEMON,  SYM_ORANGE,
  SYM_BELL,   SYM_CHERRY, SYM_PLUM,   SYM_BAR
};
const uint8_t reel1[] = {
  SYM_LEMON,  SYM_ORANGE, SYM_PLUM,   SYM_CHERRY,
  SYM_LEMON,  SYM_ORANGE, SYM_PLUM,   SYM_CHERRY,
  SYM_BELL,   SYM_LEMON,  SYM_ORANGE, SYM_PLUM,
  SYM_CHERRY, SYM_LEMON,  SYM_ORANGE, SYM_BELL,
  SYM_CHERRY, SYM_PLUM,   SYM_BAR,    SYM_CHERRY
};
const uint8_t reel2[] = {
  SYM_ORANGE, SYM_PLUM,   SYM_CHERRY, SYM_LEMON,
  SYM_ORANGE, SYM_PLUM,   SYM_CHERRY, SYM_LEMON,
  SYM_BELL,   SYM_ORANGE, SYM_PLUM,   SYM_CHERRY,
  SYM_LEMON,  SYM_ORANGE, SYM_PLUM,   SYM_BELL,
  SYM_CHERRY, SYM_LEMON,  SYM_BAR,    SYM_CHERRY
};
const uint8_t REEL0_LEN = sizeof(reel0)/sizeof(reel0[0]);
const uint8_t REEL1_LEN = sizeof(reel1)/sizeof(reel1[0]);
const uint8_t REEL2_LEN = sizeof(reel2)/sizeof(reel2[0]);

// Credits/bet (declare early so all funcs see them)
int credits = 20;
int bet     = 1;
const int maxBet = 5;
const int minCreditsToPlay = 1;

// ======================
// Custom glyphs (0..7) — use 0bxxxxx to avoid deprecation warnings
byte chCherry[8] = { 0b00100,0b01110,0b11111,0b01110,0b00100,0b00100,0b01110,0b00000 };
byte chLemon [8] = { 0b00000,0b01110,0b11111,0b11111,0b11111,0b01110,0b00000,0b00000 };
byte chOrange[8] = { 0b00100,0b01110,0b11111,0b11111,0b11111,0b01110,0b00100,0b00000 };
byte chPlum  [8] = { 0b00100,0b01110,0b11111,0b11111,0b01110,0b00100,0b00100,0b00000 };
byte chBell  [8] = { 0b00100,0b01110,0b11111,0b10101,0b00100,0b00100,0b11111,0b00000 };
byte chBar   [8] = { 0b11111,0b10101,0b10101,0b11111,0b10101,0b10101,0b11111,0b00000 };
byte chCoin  [8] = { 0b00100,0b01110,0b11111,0b10101,0b11111,0b01110,0b00100,0b00000 };
byte chArrow [8] = { 0b00000,0b00100,0b00110,0b11111,0b00110,0b00100,0b00000,0b00000 };

// ======================
// Buttons (debounced; active HIGH)
// ======================
struct DebouncedButton {
  int pin;
  bool lastStable = LOW;
  bool lastRead   = LOW;
  unsigned long lastChangeMs = 0;
  const unsigned long debounceMs = 25;

  void begin() { pinMode(pin, INPUT_PULLDOWN); }
  bool rose() { // LOW->HIGH edge
    bool r = digitalRead(pin);
    unsigned long now = millis();
    if (r != lastRead) { lastRead = r; lastChangeMs = now; }
    if ((now - lastChangeMs) > debounceMs && r != lastStable) {
      lastStable = r;
      if (lastStable == HIGH) return true;
    }
    return false;
  }
};
DebouncedButton btnSpin{BTN_UP_PIN};
DebouncedButton btnBet {BTN_DOWN_PIN};

// ======================
// Sound
// ======================
void beep(unsigned int freq, unsigned int durMs) { tone(SPK_PIN, freq, durMs); }
void thudStop() { tone(SPK_PIN, 200, 50); }
void jingleWin() {
  tone(SPK_PIN, 880, 120); delay(130);
  tone(SPK_PIN, 1175,120); delay(130);
  tone(SPK_PIN, 1567,160); delay(170);
}

// ======================
// State (no enums)
// ======================
const uint8_t STATE_IDLE       = 0;
const uint8_t STATE_SPINNING   = 1;
const uint8_t STATE_SHOWRESULT = 2;
uint8_t state = STATE_IDLE;

// Reel runtime
struct ReelRuntime {
  uint8_t idx;
  int pos;
  unsigned long lastStepMs;
  unsigned long stepIntervalMs;
  unsigned long stopAtMs;
  bool stopped;
};
ReelRuntime reels[3];

unsigned long resultShownAt = 0;
const unsigned long resultHoldMs = 1200;

// ======================
// LCD diff writer
// ======================
char lcdBuf[2][17]; // cache of what's currently shown
void lcdInitBuf(){ for(int r=0;r<2;r++){ for(int c=0;c<16;c++) lcdBuf[r][c]=' '; lcdBuf[r][16]='\0'; } }
inline void lcdWriteAt(uint8_t row, uint8_t col, uint8_t ch){
  if (row>1||col>15) return;
  if (lcdBuf[row][col]==(char)ch) return;
  lcd.setCursor(col,row);
  lcd.write(ch);
  lcdBuf[row][col]=(char)ch;
}
inline void lcdPrintFixed(uint8_t row, uint8_t col, const char* s){
  for (uint8_t i=0; s[i] && (col+i)<16; i++) lcdWriteAt(row, col+i, (uint8_t)s[i]);
}
inline void lcdClearRow(uint8_t row){ for(int c=0;c<16;c++) lcdWriteAt(row,c,' '); }

// Layout
const uint8_t reelCols[3] = { 3, 7, 11 };
const uint8_t paylineRow = 0;

// ======================
// Helpers (no enums)
// ======================
uint8_t getReelSym(uint8_t ridx, int pos){
  if (ridx==0) return reel0[pos % REEL0_LEN];
  if (ridx==1) return reel1[pos % REEL1_LEN];
  return reel2[pos % REEL2_LEN];
}
uint8_t getReelLen(uint8_t ridx){
  return (ridx==0)?REEL0_LEN : (ridx==1)?REEL1_LEN : REEL2_LEN;
}
int payout(uint8_t s) {
  switch (s) {
    case SYM_BAR:    return 10;
    case SYM_BELL:   return 5;
    case SYM_PLUM:   return 4;
    case SYM_ORANGE: return 3;
    case SYM_LEMON:  return 2;
    case SYM_CHERRY: return 1;
  }
  return 0;
}

// ======================
// UI
// ======================
void showHud(){
  lcdClearRow(1);
  lcdWriteAt(1,0, 6); // coin glyph
  char cr[6]; snprintf(cr,sizeof(cr),"%2d", (int)credits);
  lcdPrintFixed(1,1, cr);
  lcdPrintFixed(1,5, "Bet");
  char bb[4]; snprintf(bb,sizeof(bb),"%d",(int)bet);
  lcdPrintFixed(1,8, bb);
  lcdWriteAt(1,11, 7); // arrow
  lcdPrintFixed(1,12, "Spin");
}

void showWelcome(){
  lcdClearRow(0);
  lcdPrintFixed(0,0, "Slot Machine");
  showHud();
}

void drawReels() {
  for (uint8_t i=0;i<3;i++) {
    uint8_t s = getReelSym(i, reels[i].pos);
    lcdWriteAt(paylineRow, reelCols[i], s); // s is already custom-char index (0..5)
  }
}

// ======================
// Game logic
// ======================
void startSpin() {
  if (credits < minCreditsToPlay || bet > credits) return;

  credits -= bet;
  showHud();

  randomSeed((uint32_t)analogRead(28) ^ (uint32_t)micros());

  unsigned long now = millis();
  for (uint8_t i=0;i<3;i++) {
    reels[i].idx = i;
    reels[i].pos = random(0, getReelLen(i));
    reels[i].lastStepMs = now;
    reels[i].stepIntervalMs = 30 + i*10;       // initial speed
    reels[i].stopAtMs = now + 1200 + i*450;    // stop spacing
    reels[i].stopped = false;
  }

  ledSpinning();
  beep(600, 80);
  state = STATE_SPINNING;
}

void updateSpin() {
  unsigned long now = millis();
  for (uint8_t i=0;i<3;i++) {
    if (reels[i].stopped) continue;

    if (now - reels[i].lastStepMs >= reels[i].stepIntervalMs) {
      reels[i].lastStepMs = now;
      reels[i].pos = (reels[i].pos + 1) % getReelLen(i);
      drawReels(); // only updates 3 cells max
      if (reels[i].stepIntervalMs < 180) reels[i].stepIntervalMs += 2; // decelerate
    }

    if (now >= reels[i].stopAtMs) {
      reels[i].stopped = true;
      thudStop();
    }
  }

  if (reels[0].stopped && reels[1].stopped && reels[2].stopped) {
    state = STATE_SHOWRESULT;
    resultShownAt = now;

    // Evaluate
    uint8_t s0 = getReelSym(0, reels[0].pos);
    uint8_t s1 = getReelSym(1, reels[1].pos);
    uint8_t s2 = getReelSym(2, reels[2].pos);

    int won = 0;
    if (s0==s1 && s1==s2) {
      won = payout(s0) * bet;
    } else if (s0==SYM_CHERRY && s1==SYM_CHERRY && s2!=SYM_CHERRY) {
      won = 1 * bet; // small consolation
    }

    lcdClearRow(1);
    if (won > 0) {
      credits += won;
      char msg[17]; snprintf(msg, sizeof(msg), "WIN +%d  Cr:%2d", won, credits);
      lcdPrintFixed(1,0, msg);
      ledWin();
      jingleWin();
    } else {
      char msg[17]; snprintf(msg, sizeof(msg), "No win  Cr:%2d", credits);
      lcdPrintFixed(1,0, msg);
      ledLose();
      beep(180, 80);
    }
  }
}

void updateResult() {
  if (millis() - resultShownAt > resultHoldMs) {
    showHud();
    ledOff();
    state = STATE_IDLE;
  }
}

// ======================
// Setup / Loop
// ======================
void setup(){
  pinMode(BTN_UP_PIN,   INPUT_PULLDOWN);
  pinMode(BTN_DOWN_PIN, INPUT_PULLDOWN);

  pinMode(SPK_PIN, OUTPUT);
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  ledOff();

  Wire.begin();
  Wire.setClock(400000); // fast I2C; drop to 100k if unstable

  lcd.begin();
  lcd.noCursor();
  lcd.noBlink();
  lcd.backlight();

  // Load custom glyphs (0..7)
  lcd.createChar(0, chCherry);
  lcd.createChar(1, chLemon);
  lcd.createChar(2, chOrange);
  lcd.createChar(3, chPlum);
  lcd.createChar(4, chBell);
  lcd.createChar(5, chBar);
  lcd.createChar(6, chCoin);
  lcd.createChar(7, chArrow);

  lcd.clear();
  lcd.print("Loading...");
  delay(300);

  // Init buffers and initial reels
  lcdInitBuf();
  for (uint8_t i=0;i<3;i++) reels[i].pos = 0;
  showWelcome();
  drawReels();

  btnSpin.begin();
  btnBet.begin();
}

void loop(){
  if (state == STATE_IDLE) {
    if (btnBet.rose()) {
      bet = (bet % maxBet) + 1;
      if (bet > credits) bet = (credits > 0 ? credits : 1);
      showHud();
      beep(880, 40);
    }
    if (btnSpin.rose()) {
      if (credits >= minCreditsToPlay) startSpin();
      else {
        lcdClearRow(1);
        lcdPrintFixed(1,0,"Out of credits!");
        ledLose();
        beep(120, 200);
      }
    }
  }
  else if (state == STATE_SPINNING) {
    updateSpin();
  }
  else if (state == STATE_SHOWRESULT) {
    updateResult();
  }
}
