#define STRING_H
#ifdef STRING_H

#include "types.h"

int strcmp(const char *a, const char *b);

void uint8_to_string(uint8_t num, char *str);

void uint64_to_string(uint64_t num, char *str);

void uint16_to_string(uint16_t num, char *str);

void strcat(char *dest, const char *src);

#endif