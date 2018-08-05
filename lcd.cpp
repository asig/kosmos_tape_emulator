#include "lcd.h"
#include "config.h"
#include <LiquidCrystal.h>
#include <string.h>
#include <stdio.h>

// LCD pins
#define LCD_RS 2
#define LCD_EN 3
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

#define B(b5,b4,b3,b2,b1) (b5<<4 | b4<<3 | b3<<2 | b2<<1 | b1<<0)

#define DEF_CHAR(c, \
  b0_5,b0_4,b0_3,b0_2,b0_1, \
  b1_5,b1_4,b1_3,b1_2,b1_1, \
  b2_5,b2_4,b2_3,b2_2,b2_1, \
  b3_5,b3_4,b3_3,b3_2,b3_1, \
  b4_5,b4_4,b4_3,b4_2,b4_1, \
  b5_5,b5_4,b5_3,b5_2,b5_1, \
  b6_5,b6_4,b6_3,b6_2,b6_1, \
  b7_5,b7_4,b7_3,b7_2,b7_1) \
  { uint8_t pat[]={ \
    (b0_5<<4 | b0_4<<3 | b0_3<<2 | b0_2<<1 | b0_1<<0), \
    (b1_5<<4 | b1_4<<3 | b1_3<<2 | b1_2<<1 | b1_1<<0), \
    (b2_5<<4 | b2_4<<3 | b2_3<<2 | b2_2<<1 | b2_1<<0), \
    (b3_5<<4 | b3_4<<3 | b3_3<<2 | b3_2<<1 | b3_1<<0), \
    (b4_5<<4 | b4_4<<3 | b4_3<<2 | b4_2<<1 | b4_1<<0), \
    (b5_5<<4 | b5_4<<3 | b5_3<<2 | b5_2<<1 | b5_1<<0), \
    (b6_5<<4 | b6_4<<3 | b6_3<<2 | b6_2<<1 | b6_1<<0), \
    (b7_5<<4 | b7_4<<3 | b7_3<<2 | b7_2<<1 | b7_1<<0)};\
    lcd.createChar(c,pat); \
  }

void lcd_init() {
  lcd.begin(16, 2);

  DEF_CHAR(CHAR_UP,
    0,0,0,0,0,
    0,0,0,0,0,
    0,0,1,0,0,
    0,1,0,1,0,
    1,0,0,0,1,
    0,0,0,0,0,
    0,0,0,0,0,
    0,0,0,0,0);

   DEF_CHAR(CHAR_DOWN,
    0,0,0,0,0,
    0,0,0,0,0,
    1,0,0,0,1,
    0,1,0,1,0,
    0,0,1,0,0,
    0,0,0,0,0,
    0,0,0,0,0,
    0,0,0,0,0);
    
   DEF_CHAR(CHAR_LEFT,
    0,0,0,0,0,
    0,0,0,1,0,
    0,0,1,0,0,
    0,1,0,0,0,
    0,0,1,0,0,
    0,0,0,1,0,
    0,0,0,0,0,
    0,0,0,0,0);

   DEF_CHAR(CHAR_RIGHT,
    0,0,0,0,0,
    0,1,0,0,0,
    0,0,1,0,0,
    0,0,0,1,0,
    0,0,1,0,0,
    0,1,0,0,0,
    0,0,0,0,0,
    0,0,0,0,0);

   DEF_CHAR(CHAR_PLAY,
    0,0,0,0,0,
    0,1,0,0,0,
    0,1,1,0,0,
    0,1,1,1,0,
    0,1,1,0,0,
    0,1,0,0,0,
    0,0,0,0,0,
    0,0,0,0,0);

   DEF_CHAR(CHAR_RECORD,
    0,0,0,0,0,
    0,1,1,1,0,
    1,1,1,1,1,
    1,1,1,1,1,
    1,1,1,1,1,
    0,1,1,1,0,
    0,0,0,0,0,
    0,0,0,0,0);
}

void lcd_showLine(const char *s, Alignment align) {
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

void lcd_showLineF(Alignment align, const char *fmt, va_list arg) {  
  char buf[DISPLAY_WIDTH+1];
  vsnprintf(buf, DISPLAY_WIDTH+1, fmt, arg);
  lcd_showLine(buf, align);
}

void lcd_showTitle(Alignment align, const char* fmt, ...) {
  lcd.setCursor(0,0);
  va_list args;
  va_start (args, fmt);
  lcd_showLineF(align, fmt, args);
  va_end (args);
}

void lcd_showStatus(Alignment align, const char* fmt, ...) {
  lcd.setCursor(0,1);
  va_list args;
  va_start (args, fmt);
  lcd_showLineF(align, fmt, args);
  va_end (args);
}


