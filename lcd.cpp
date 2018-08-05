#include "lcd.h"
#include "config.h"
#include <LiquidCrystal.h>
#include <string.h>

#define DISPLAY_WIDTH 16

// LCD
#define LCD_RS 2
#define LCD_EN 3
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void lcd_init() {
  lcd.begin(16, 2);
}

void lcd_showLine(char *s, Alignment align) {
  char buf[17] = "                ";
  int len = strlen(s);
  if (len > DISPLAY_WIDTH) {
    len = DISPLAY_WIDTH;
  }
  
  int ofs = 0;
  switch(align) {
    case LEFT: ofs = 0; break;
    case CENTER: ofs = (DISPLAY_WIDTH - len)/2; break;
    case RIGHT: ofs = DISPLAY_WIDTH - len;
  }
  strncpy(&buf[ofs], s, len);
  lcd.print(buf);
}

void lcd_showTitle(char *s, Alignment align) {
  lcd.setCursor(0,0);
  lcd_showLine(s, align);
}

void lcd_showStatus(char *s, Alignment align) {
  lcd.setCursor(0,1);
  lcd_showLine(s, align);
}

