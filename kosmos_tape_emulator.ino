#include "config.h"
#include "types.h"
#include "lcd.h"
#include "sdcard.h"

// Pin defitions

// Buttons
#define BTN_U_PIN A0
#define BTN_D_PIN A1
#define BTN_L_PIN A2
#define BTN_R_PIN A3

#define BTN_U 1
#define BTN_D 2
#define BTN_L 4
#define BTN_R 8

#define DATA_INDICATOR_PIN 13
#define KOSMOS_DATA_PIN 8

uint8_t prgBuf[512]; // buffer used for recording or playing a program
int prgLen; // length of data in prgBuf.

void setup() {
  pinMode(BTN_U_PIN, INPUT_PULLUP);
  pinMode(BTN_D_PIN, INPUT_PULLUP);
  pinMode(BTN_L_PIN, INPUT_PULLUP);
  pinMode(BTN_R_PIN, INPUT_PULLUP);

  pinMode(DATA_INDICATOR_PIN, OUTPUT);
  
  lcd_init();
  lcd_showTitle(CENTER, "CP1 Tape Emul.");
  
  lcd_showStatus(CENTER, "Reading files...");
  sdcard_init();
  lcd_showStatus(CENTER, "%d files found.", sdcard_filesAvailable());
  delay(1000);  
}

int getBtns() {
  int btns = 0;
  if (!digitalRead(BTN_U_PIN)) btns |= BTN_U;
  if (!digitalRead(BTN_D_PIN)) btns |= BTN_D;
  if (!digitalRead(BTN_L_PIN)) btns |= BTN_L;
  if (!digitalRead(BTN_R_PIN)) btns |= BTN_R;
  return btns;
}

int waitBtn(int btnMask) {
  int wanted;
  do {
    wanted = getBtns() & btnMask;
  } while (wanted == 0);
  while ((getBtns() & btnMask) !=0 ) ;
  return wanted;
}

int waitPin(int pin, int val, int indicator, int timeout) {
  int start = millis();
  for(;;) {
    int read = digitalRead(pin);
    digitalWrite(indicator, read);
    if (read == val) {
      return millis() - start;
    }
    if (millis() - start > timeout) {
      return -1;
    }
  }
}

void doPlay() {  
  pinMode(KOSMOS_DATA_PIN, OUTPUT);

  // 1) Choose file
  lcd_showTitle(LEFT, "Select file:");
  int cur;
  char filename[NAME_LEN];
  for(;;) {  
    sdcard_getFileName(cur, filename);
    lcd_showStatus(LEFT, "%s", filename);
    int btn = waitBtn(BTN_L | BTN_U | BTN_D | BTN_R);
    if (btn & BTN_U) {
      if (--cur < 0) cur = sdcard_filesAvailable()-1;
    } else if (btn & BTN_D) {
      if (++cur >= sdcard_filesAvailable()) cur = 0;
    } else if (btn & BTN_L) {
      return;
    } else {
      break;
    }    
  }

  // 2) load file
  lcd_showTitle(LEFT, "Loading");
  lcd_showStatus(LEFT, filename);
  prgLen = sdcard_load(filename, prgBuf);
  if (prgLen == 0) {
    return;
  }

  // 3) Send Lead-In
  lcd_showTitle(LEFT, "Sending Lead-In:");
  lcd_showStatus(LEFT, "");
  digitalWrite(KOSMOS_DATA_PIN, HIGH);
  digitalWrite(DATA_INDICATOR_PIN, HIGH);
  int now = millis();
  int delta;
  do {
    if (!digitalRead(BTN_L_PIN)) goto stop;
    delta = millis() - now;
    lcd_showStatus(LEFT, "%d.%02d secs", delta/1000, (delta % 1000)/10);
  } while (delta < 15000);

  // 4) send bytes
  lcd_showTitle(LEFT, "Sending Data:");
  lcd_showStatus(LEFT, "");
  for (int i = 0; i < prgLen; i++) {
    if (!digitalRead(BTN_L_PIN)) goto stop;
    lcd_showStatus(LEFT, "Byte %d of %d.", i+1, prgLen);
    for (int j = 0; j < 8; j++) {
      if (prgBuf[i] & (1<<j)) {
        // Bit 1
        digitalWrite(KOSMOS_DATA_PIN, 0);
        digitalWrite(DATA_INDICATOR_PIN, 0);
        delay(35);
        digitalWrite(KOSMOS_DATA_PIN, 1);
        digitalWrite(DATA_INDICATOR_PIN, 1);
        delay(65);
      } else {
        // Bit 0
        digitalWrite(KOSMOS_DATA_PIN, 0);
        digitalWrite(DATA_INDICATOR_PIN, 0);
        delay(65);
        digitalWrite(KOSMOS_DATA_PIN, 1);
        digitalWrite(DATA_INDICATOR_PIN, 1);
        delay(35);      
      }
    }
  }
  digitalWrite(KOSMOS_DATA_PIN, 0);
  digitalWrite(DATA_INDICATOR_PIN, 0);
  lcd_showStatus(LEFT, "Done."); 
  delay(2000);
  return;

  stop:
  digitalWrite(KOSMOS_DATA_PIN, 0);
  digitalWrite(DATA_INDICATOR_PIN, 0);
  lcd_showStatus(LEFT, "Stopped.");
  delay(2000);   
}

void doRecord() {
  pinMode(KOSMOS_DATA_PIN, INPUT_PULLUP);

  // 1) Select file name

  lcd_showTitle(LEFT, "Receiving Data:");

  // 3) Read lead-in
  lcd_showStatus(LEFT, "Lead-In...");
  int t;
  t = waitPin(KOSMOS_DATA_PIN, 1, DATA_INDICATOR_PIN, 20000); if (t < 0) goto timeout;
  t = waitPin(KOSMOS_DATA_PIN, 0, DATA_INDICATOR_PIN, 20000); if (t < 0) goto timeout;
  
  // 4) Read bytes
  for(;;) {
    uint8_t data = 0;
    lcd_showStatus(LEFT, "Byte %d", prgLen + 1);
    for(int i = 0; i < 8; i++) {
      int t1 = waitPin(KOSMOS_DATA_PIN, 1, DATA_INDICATOR_PIN, 1000); if (t1 < 0) break;
      int t2 = waitPin(KOSMOS_DATA_PIN, 0, DATA_INDICATOR_PIN, 1000); if (t1 < 0) break;
      if (t1 < t2) {
        // Bit 1
        data |= (1<<i);
      } else {
        // Bit 0
      }
    }
    prgBuf[prgLen++] = data;
  }

  lcd_showStatus(LEFT, "%d bytes read.", prgLen);
  delay(2000);
  return;

  timeout:
  lcd_showStatus(LEFT, "Timeout!");
  delay(2000);
}

void loop() {   //1234567890123456"
  lcd_showTitle(CENTER, "CP1 Tape Emul.");
  lcd_showStatus(CENTER, "%c: Play  %c: Rec", CHAR_LEFT, CHAR_RIGHT);
  int btn = waitBtn( BTN_L | BTN_R );
  if (btn & BTN_L) {
    doPlay();
  } else {
    doRecord();
  }
}

