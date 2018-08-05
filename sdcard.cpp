#include "sdcard.h"
#include "config.h"
#include "lcd.h"
#include <SD.h>

// SD-Card (other pins are default ICSP pins)
#define SD_CS   8

#define MAX_FILES 64
#define NAME_LEN 13

char files[MAX_FILES][NAME_LEN];
int nofFiles = 0;

void sdcard_init() {
  if (!SD.begin(SD_CS)) {
    lcd_showStatus("SDC not ready!");
    while (1);
  }

  lcd_showStatus("Reading files...");
  File root = SD.open("/");
  while (nofFiles < MAX_FILES) {
    File entry =  root.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    if (!entry.isDirectory()) {
      strncpy(files[nofFiles], entry.name(), NAME_LEN); 
      nofFiles++;
    }
    entry.close();
  }
  char buf[17];
  sprintf(buf, "%d files found", nofFiles);
  lcd_showStatus(buf);
}


