#include "screen.h"

static uint32_t* framebuffer;

static uint64_t screen_width;
static uint64_t screen_height;
static uint64_t screen_pitch;

static int cursor_x = 0;
static int cursor_y = 0;

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


void print(const char* str, uint32_t color)
{
    while (*str)
    {   
        if(str[cursor_x] == '\n') {
            cursor_x = 0;
            cursor_y += 16; // Move to the next line
            str++;
            continue;
        }

        draw_char(*str, cursor_x, cursor_y, color);
        cursor_x += 8; // Move to the next character position
        str++;
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


void set_cursor(int x, int y)
{
    cursor_x = x;
    cursor_y = y;
}


void draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        putpixel(x0, y0, color);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = err * 2;

        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void draw_rectangle(int x, int y, int width, int height, uint32_t color, booln filled)
{   
    if (width <= 0 || height <= 0) {
        return;
    }
    
    if (filled)
    {
        for (int i = 0; i < height; i++)
        {
            draw_line(x, y + i, x + width - 1, y + i, color);
        }
    }
    else
    {
        draw_line(x, y, x + width - 1, y, color); // sopra
        draw_line(x, y + height - 1, x + width - 1, y + height - 1, color); // sotto

        draw_line(x, y, x, y + height - 1, color); // sinistra
        draw_line(x + width - 1, y, x + width - 1, y + height - 1, color); // destra
    }
}

void draw_circle(int cx, int cy, int radius, uint32_t color)
{
    int x = radius;
    int y = 0;
    int decision = 1 - radius;

    while (x >= y)
    {
        putpixel(cx + x, cy + y, color);
        putpixel(cx + y, cy + x, color);
        putpixel(cx - y, cy + x, color);
        putpixel(cx - x, cy + y, color);
        putpixel(cx - x, cy - y, color);
        putpixel(cx - y, cy - x, color);
        putpixel(cx + y, cy - x, color);
        putpixel(cx + x, cy - y, color);

        y++;

        if (decision <= 0)
        {
            decision += 2 * y + 1;
        }
        else
        {
            x--;
            decision += 2 * (y - x) + 1;
        }
    }
}


void draw_bitmap(void *data, int pos_x, int pos_y)
{
    uint8_t *bmp = (uint8_t *)data;

    // Controllo firma BMP: "BM"
    if (bmp[0] != 'B' || bmp[1] != 'M')
        return;


    // Offset dove iniziano i pixel
    uint32_t pixel_offset = *(uint32_t *)(bmp + 10);

    // Larghezza e altezza
    int32_t width  = *(int32_t *)(bmp + 18);
    int32_t height = *(int32_t *)(bmp + 22);

    // Bit per pixel
    uint16_t bpp = *(uint16_t *)(bmp + 28);

    // Supportiamo solo 24 bit
    if (bpp != 24)
        return;


    uint8_t *pixels = bmp + pixel_offset;


    // Ogni riga BMP è allineata a multipli di 4 byte
    int row_size = (width * 3 + 3) & ~3;


    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = y * row_size + x * 3;


            uint8_t blue  = pixels[index];
            uint8_t green = pixels[index + 1];
            uint8_t red   = pixels[index + 2];


            uint32_t color =
                (red << 16) |
                (green << 8) |
                blue;


            // BMP è salvato dal basso verso l'alto
            putpixel(
                pos_x + x,
                pos_y + (height - y - 1),
                color
            );
        }
    }
}