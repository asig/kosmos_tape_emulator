#include "config.h"
#include "types.h"
#include "lcd.h"
#include "sdcard.h"

// Pin defitions

// Buttons
#define BTN_U A0
#define BTN_D A1
#define BTN_L A2
#define BTN_R A3

byte prgBuf[512]; // buffer used for recording or playing a program
int prgLen; // length of data in prgBuf.

void setup() {
  pinMode(BTN_U, INPUT_PULLUP);
  pinMode(BTN_D, INPUT_PULLUP);
  pinMode(BTN_L, INPUT_PULLUP);
  pinMode(BTN_R, INPUT_PULLUP);

  lcd_init();
  lcd_showTitle("CP1 Tape Emul.");

  sdcard_init();
}

void loop() {
//  for (byte b = 0; ; b++){
//    lcd.setCursor(0, 0);
//    lcd.print((int)b); lcd.print(": ");lcd.write(b);
//    delay(1000);
//  }
  
  char mask[5] = {'u', 'd', 'l', 'r', 0};
  if (digitalRead(BTN_U) == LOW) mask[0] = 'U';
  if (digitalRead(BTN_D) == LOW) mask[1] = 'D';
  if (digitalRead(BTN_L) == LOW) mask[2] = 'L';
  if (digitalRead(BTN_R) == LOW) mask[3] = 'R';

  char buf[17];
  sprintf(buf, "%s %d", mask, millis() / 1000);
  lcd_showStatus(buf, LEFT); 
}

