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
