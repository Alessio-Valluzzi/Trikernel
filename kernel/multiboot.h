#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include "types.h"

struct multiboot_tag_framebuffer
{
    uint32_t type;
    uint32_t size;

    uint64_t addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;

    uint8_t bpp;
    uint8_t type_attr;
    uint16_t reserved;
};

struct multiboot_tag
{
    uint32_t type;
    uint32_t size;
};


#endif