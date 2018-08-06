#pragma once

#include <stdint.h>

#define NAME_LEN 13

void sdcard_init();
int sdcard_filesAvailable();
bool sdcard_getFileName(int i, char* dest);
void sdcard_pickFileName(char* dest);
int sdcard_load(const char *filename, uint8_t *buf);
void sdcard_save(const char *filename, uint8_t *buf, int len);

