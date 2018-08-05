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


