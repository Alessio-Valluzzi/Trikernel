#ifndef KEYBOARD_H
#define KEYBOARD_H


#include "asm/asm.h"
#include "types.h"

int init_keyboard();
uint8_t keyboard_read();

#endif