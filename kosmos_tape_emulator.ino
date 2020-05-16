/*
 * Copyright (c) 2018 Andreas Signer <asigner@gmail.com>
 *
 * This file is part of kosmos_tape_emulator.
 *
 * kosmos_tape_emulator is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * kosmos_tape_emulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with kosmos_tape_tool.  If not, see <http://www.gnu.org/licenses/>.
 */

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
#define KOSMOS_DATA_PIN 9

uint8_t prgBuf[512]; // buffer used for recording or playing a program
int prgLen; // length of data in prgBuf.

void setup() {
  pinMode(BTN_U_PIN, INPUT_PULLUP);
  pinMode(BTN_D_PIN, INPUT_PULLUP);
  pinMode(BTN_L_PIN, INPUT_PULLUP);
  pinMode(BTN_R_PIN, INPUT_PULLUP);

  pinMode(DATA_INDICATOR_PIN, OUTPUT);

  lcd_init();

  #ifdef DEBUG
  Serial.begin(115200);
  lcd_showTitle(CENTER, "Debug 115200 bps");
  lcd_showStatus(CENTER, "Connect console.");
  while(!Serial) {
    // Wait...
  }
  #endif
  
  lcd_showTitle(CENTER, "CP2 Emulator");
  
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

int32_t waitPin(int pin, int val, int indicator, int32_t timeout) {
  uint32_t start = millis();
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

bool checkStop() {
  if (digitalRead(BTN_L_PIN)) {
    return false;
  }
  digitalWrite(KOSMOS_DATA_PIN, 0);
  digitalWrite(DATA_INDICATOR_PIN, 0);
  lcd_showStatus(LEFT, "Stopped.");
  delay(2000);   
  return true;
}

void sendByte(uint8_t b) {
  int d1, d2;
  for (int j = 0; j < 8; j++) {
    if (b & (1<<j)) {
      // Bit == 1
      d1 = 35;
      d2 = 65;
    } else {
      // Bit == 0
      d1 = 65;
      d2 = 35;
    }
    digitalWrite(KOSMOS_DATA_PIN, 0);
    digitalWrite(DATA_INDICATOR_PIN, 0);
    delay(d1);
    digitalWrite(KOSMOS_DATA_PIN, 1);
    digitalWrite(DATA_INDICATOR_PIN, 1);
    delay(d2);
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
  uint32_t now = millis();
  uint32_t delta;
  do {
    if (checkStop()) return;
    delta = millis() - now;
    lcd_showStatus(LEFT, "%lu.%02lu secs", delta/1000, (delta % 1000)/10);
  } while (delta < 15000);

  // 4) send bytes
  lcd_showTitle(LEFT, "Sending Data:");
  lcd_showStatus(LEFT, "");

  int blocks = prgLen/256;
  int cnt = 1;
  int block = -1;
  while(blocks-- > 0) {
    block++;
    // Kosmos CP1 serializes byts 0, 255, 254, ..., 1
    uint8_t pos = 0;
    do {
      #ifdef DEBUG
      Serial.print("Sending byte pos "); 
      Serial.print(pos, DEC); 
      Serial.print(" in block "); 
      Serial.print(block, DEC); 
      Serial.print(": "); 
      Serial.println(prgBuf[block*256 + pos], DEC);
      #endif
      if (checkStop()) return;
      lcd_showStatus(LEFT, "Byte %d of %d.", cnt++, prgLen);
      sendByte(prgBuf[block*256 + pos--]);      
    } while (pos > 0);    
  }

  lcd_showStatus(LEFT, "Done."); 

  digitalWrite(KOSMOS_DATA_PIN, 0);
  digitalWrite(DATA_INDICATOR_PIN, 0);
  delay(1000); // Let CP1 notice that we're not sending anymore
  pinMode(KOSMOS_DATA_PIN, INPUT); // Stop driving the data line.

  delay(3000); // Give the user some more time to read the message
  return;
}

void timeout() {
  lcd_showStatus(LEFT, "Timeout!");
  delay(2000);
}

void error() {
  lcd_showStatus(LEFT, "Error!");
  delay(2000);
}

void doRecord() {
  pinMode(KOSMOS_DATA_PIN, INPUT_PULLUP);
  lcd_showTitle(LEFT, "Receiving Data:");

  // 1) Select file name
  char filename[NAME_LEN];
  sdcard_pickFileName(filename);
  lcd_showStatus(LEFT, "%s", filename);
  delay(2000);

  // 3) Read lead-in
  lcd_showStatus(LEFT, "Lead-In...");
  int32_t t;
  t = waitPin(KOSMOS_DATA_PIN, 1, DATA_INDICATOR_PIN, 20000); if (t < 0) { timeout(); return; }
  t = waitPin(KOSMOS_DATA_PIN, 0, DATA_INDICATOR_PIN, 20000); if (t < 0) { timeout(); return; }
  
  // 4) Read bytes
  int block = -1;
  int prgLen = 0;
  int bytesRead = 0;
  for(;;) {
    // Kosmos CP1 serializes byts 0, 255, 254, ..., 1
    block++;
    uint8_t pos = 0;
    do {
      lcd_showStatus(LEFT, "Byte %d", bytesRead+1);
      uint8_t data = 0;
      for(int i = 0; i < 8; i++) {
        if (checkStop()) return;
        int32_t t1 = waitPin(KOSMOS_DATA_PIN, 1, DATA_INDICATOR_PIN, 1000); if (t1 < 0) goto done;
        int32_t t2 = waitPin(KOSMOS_DATA_PIN, 0, DATA_INDICATOR_PIN, 1000); if (t1 < 0) { error(); return; }
        if (t1 < t2) {
          // Bit 1
          data |= (1<<i);
        } else {
          // Bit 0
        }
      }
      bytesRead++;
      prgBuf[block*256 + pos--] = data;
    } while (pos > 0);
  }

  done:
  prgLen = bytesRead;
  lcd_showStatus(LEFT, "%d bytes read.", prgLen);
  sdcard_save(filename, prgBuf, prgLen);
  delay(2000);  
}

void loop() {
  lcd_showTitle(CENTER, "CP2 Emulator");
  lcd_showStatus(CENTER, "%c: Play  %c: Rec", CHAR_LEFT, CHAR_RIGHT);
  int btn = waitBtn( BTN_L | BTN_R );
  if (btn & BTN_L) {
    doPlay();
  } else {
    doRecord();
  }
}

