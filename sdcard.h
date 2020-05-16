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

#include <stdint.h>

#define NAME_LEN 13

void sdcard_init();
int sdcard_filesAvailable();
bool sdcard_getFileName(int i, char* dest);
void sdcard_pickFileName(char* dest);
int sdcard_load(const char *filename, uint8_t *buf);
void sdcard_save(const char *filename, uint8_t *buf, int len);

