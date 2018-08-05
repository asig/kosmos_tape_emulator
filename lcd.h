#pragma once

#include "types.h"

void lcd_init();
void lcd_showTitle(char *s, Alignment align = CENTER);
void lcd_showStatus(char *s, Alignment align = CENTER);


