#ifndef SCREEN_H
#define SCREEN_H

#include "types.h"
#include "limine.h"
#include "libs/math/math.h"

void screen_init(struct limine_framebuffer* fb);

void putpixel(uint32_t x, uint32_t y, uint32_t color);

void clear_screen(uint32_t color);

void draw_char(char c, int x, int y, uint32_t color);

void print(const char* str, uint32_t color);

void set_cursor(int x, int y);

void draw_line(int x0, int y0, int x1, int y1, uint32_t color);

void draw_rectangle(int x, int y, int width, int height, uint32_t color, booln filled);

void draw_circle(int cx, int cy, int radius, uint32_t color);

void draw_bitmap(void *data, int pos_x, int pos_y);

#endif