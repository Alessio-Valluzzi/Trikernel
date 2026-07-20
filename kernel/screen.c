#include "screen.h"

static uint32_t* framebuffer;

static uint64_t screen_width;
static uint64_t screen_height;
static uint64_t screen_pitch;


void screen_init(struct limine_framebuffer* fb)
{
    framebuffer = (uint32_t*)fb->address;

    screen_width = fb->width;
    screen_height = fb->height;
    screen_pitch = fb->pitch;
}


void putpixel(uint32_t x, uint32_t y, uint32_t color)
{
    if (x >= screen_width || y >= screen_height)
        return;

    uint32_t* pixel =
        (uint32_t*)((uint8_t*)framebuffer +
        y * screen_pitch +
        x * 4);

    *pixel = color;
}

extern unsigned char font[];

void draw_char(char c, int x, int y, uint32_t color)
{
    unsigned char *glyph = &font[(unsigned char)c * 16];

    for (int row = 0; row < 16; row++)
    {
        uint8_t bits = glyph[row];

        for (int col = 0; col < 8; col++)
        {
            if (bits & (0x80 >> col))
            {
                putpixel(x + col, y + row, color);
            }
        }
    }
}


void clear_screen(uint32_t color)
{
    for (uint64_t y = 0; y < screen_height; y++)
    {
        for (uint64_t x = 0; x < screen_width; x++)
        {
            putpixel(x, y, color);
        }
    }
}