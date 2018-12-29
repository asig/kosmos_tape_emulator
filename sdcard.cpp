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
