#include "stdint.h"

#ifndef LIB_STDIO_H_
#define LIB_STDIO_H_

uint32_t putstr(const char* s);

void putchar(const char c);

uint8_t getchar();

uint32_t getstr(char* s, uint32_t len);

#endif