#include "pmm.h"

struct ram_region usable_regions[128];
uint64_t region_count = 0;
uint64_t bitmap_start_page;
static uint8_t *bitmap = 0;
static uint64_t bitmap_size = 0;


void init_pmm(uint64_t total_ram)
{
    uint64_t total_pages = total_ram / PAGE_SIZE;
    bitmap_size = (total_pages + 7) / 8;

    for(uint64_t i = BITMAP_START_REGION; i < region_count; i++)
    {
        if(usable_regions[i].length >= bitmap_size)
        {   
            bitmap_start_page = usable_regions[i].base / PAGE_SIZE;
            bitmap = (uint8_t *)usable_regions[i].base;
            break;
        }
    }
    

    for(uint64_t i = 0; i < bitmap_size; i++)
    {
        bitmap[i] = 0xFF;
    }

    for(uint64_t i = 0; i < region_count; i++)
    {   
        struct ram_region *region = &usable_regions[i];
        uint64_t start_page = region->base / PAGE_SIZE;
        uint64_t end_page = (region->base + region->length) / PAGE_SIZE;

        for(uint64_t page = start_page; page < end_page; page++)
        {
            bitmap[page / 8] &= ~(1 << (page % 8));
        }
    }


    uint64_t bitmap_pages = (bitmap_size + PAGE_SIZE - 1) / PAGE_SIZE;
    for(uint64_t page = bitmap_start_page;
        page < bitmap_start_page + bitmap_pages;
        page++)
    {
        bitmap[page / 8] |= (1ULL << (page % 8));
    }

}



uint64_t alloc_page()
{   

    if(bitmap == 0)
    {
        panic("PMM: bitmap is NULL");
        return 0;
    }

    if(bitmap_size == 0)
    {
        panic("PMM: bitmap_size is 0");
        return 0;
    }

    for(uint64_t i = 0; i < bitmap_size; i++)
    {
        if(bitmap[i] != 0xFF)
        {
            for(uint8_t bit = 0; bit < 8; bit++)
            {
                if((bitmap[i] & (1 << bit)) == 0)
                {
                    bitmap[i] |= (1 << bit);

                    uint64_t page = (i * 8) + bit;

                    return page * PAGE_SIZE;
                }
            }
        }
    }
    return 0;
}

void free_page(uint64_t address)
{
    uint64_t page = address / PAGE_SIZE;

    bitmap[page / 8] &= ~(1 << (page % 8));
}

uint64_t alloc_pages(uint64_t num_pages)
{
    uint64_t start_page = 0;
    uint64_t found_pages = 0;

    for(uint64_t i = 0; i < bitmap_size; i++)
    {
        if(bitmap[i] != 0xFF)
        {
            for(uint8_t bit = 0; bit < 8; bit++)
            {
                if((bitmap[i] & (1 << bit)) == 0)
                {
                    if(found_pages == 0)
                    {
                        start_page = (i * 8) + bit;
                    }
                    found_pages++;

                    if(found_pages == num_pages)
                    {
                        for(uint64_t j = start_page; j < start_page + num_pages; j++)
                        {
                            bitmap[j / 8] |= (1 << (j % 8));
                        }
                        return start_page * PAGE_SIZE;
                    }
                }
                else
                {
                    found_pages = 0;
                }
            }
        }
        else
        {
            found_pages = 0;
        }
    }

    return 0;
}

void free_pages(uint64_t address, uint64_t num_pages)
{
    uint64_t start_page = address / PAGE_SIZE;

    for(uint64_t i = start_page; i < start_page + num_pages; i++)
    {
        bitmap[i / 8] &= ~(1 << (i % 8));
    }
}

void clear_page(uint64_t address)
{
    uint8_t *page = (uint8_t *)address;

    for(uint64_t i = 0; i < PAGE_SIZE; i++)
    {
        page[i] = 0;
    }
}