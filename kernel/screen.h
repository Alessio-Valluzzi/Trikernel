#ifndef SCREEN_H
#define SCREEN_H

#include "types.h"
#include "limine.h"

void screen_init(struct limine_framebuffer* fb);

void putpixel(uint32_t x, uint32_t y, uint32_t color);

void clear_screen(uint32_t color);

void draw_char(char c, int x, int y, uint32_t color);

void print(const char* str, uint32_t color);

void set_cursor(int x, int y);

#endif