#pragma once

#include <LiquidCrystal.h>

#include "types.h"

#define DISPLAY_WIDTH 16

void lcd_init();
void lcd_showTitle(Alignment align, const char *fmt, ...);
void lcd_showStatus(Alignment align, const char *fmt, ...);

enum {
  CHAR_UP = 1,
  CHAR_DOWN = 2,
  CHAR_LEFT = 3,
  CHAR_RIGHT= 4, 
  CHAR_PLAY = 5,
  CHAR_RECORD = 6
};
