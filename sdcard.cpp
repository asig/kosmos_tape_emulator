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

#include "sdcard.h"
#include "config.h"
#include "lcd.h"
#include <SD.h>

// SD-Card (other pins are default ICSP pins)
#define SD_CS 8

int nofFiles = 0;

void sdcard_init() {
  if (!SD.begin(SD_CS)) {
    lcd_showStatus(CENTER, "SDC not ready!");
    while (1);
  }

  File root = SD.open("/");
  for (;;) {
    File entry =  root.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    if (!entry.isDirectory()) {
      nofFiles++;
    }
    entry.close();
  }
}

int sdcard_filesAvailable() {
  return nofFiles;
}

bool sdcard_getFileName(int i, char* dest) {
  if (i < 0 || i >= nofFiles) {
    return false;
  } 
  File root = SD.open("/");
  for (;;) {
    File entry =  root.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    if (!entry.isDirectory()) {
      if (i-- == 0) {
        strncpy(dest, entry.name(), NAME_LEN);
        entry.close();
        return true;
      }
    }
    entry.close();
  }
  return false;
}

void sdcard_pickFileName(char* dest) {
  // Find max number
  int max = -1;
  File root = SD.open("/");
  for (;;) {
    File entry =  root.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    if (entry.isDirectory()) {
      continue;
    }
    strncpy(dest, entry.name(), NAME_LEN);
    #ifdef DEBUG
    Serial.print("sdcard_pickFileName: name = "); Serial.println(dest);
    #endif
    if (strncmp(dest, "IMG", 3) == 0 &&  strncmp(&dest[6], ".BIN", 4) == 0 &&  isdigit(dest[3]) && isdigit(dest[4]) && isdigit(dest[5])) {
      int nr = (dest[3]-'0')*100 + (dest[4]-'0')*10 + (dest[5]-'0');
      #ifdef DEBUG
      Serial.print("sdcard_pickFileName: nr = "); Serial.println(nr, DEC);
      #endif
      if (nr > max) {
        max = nr;
        #ifdef DEBUG
        Serial.print("sdcard_pickFileName: new max = "); Serial.println(max, DEC);
        #endif
      }
    }    
    entry.close();
  }
  sprintf(dest, "IMG%03d.BIN", max+1);  
  #ifdef DEBUG
  Serial.print("sdcard_pickFileName: result = "); Serial.println(dest);
  #endif
}

int sdcard_load(const char *filename, uint8_t *buf) {
  File file = SD.open(filename, FILE_READ);
  int len = file.size();
  if (len != 256 && len != 512) {
    lcd_showTitle(CENTER, "Error!");
    lcd_showStatus(CENTER, "Invalid length.");
    delay(2000);
    return 0;
  }
  file.read(buf, len);
  file.close();
  return len;
}

void sdcard_save(const char *filename, uint8_t *buf, int len) {
  if (len != 256 && len != 512) {
    lcd_showTitle(CENTER, "Error!");
    lcd_showStatus(CENTER, "Invalid length.");
    delay(2000);
    return;
  }
  File file = SD.open(filename, FILE_WRITE);
  file.write(buf, len);
  file.close();
}
