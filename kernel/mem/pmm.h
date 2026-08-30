#ifndef PMM_H
#define PMM_H

#include "types.h"
#include "screen/log.h"
#define PAGE_SIZE 4096
#define BITMAP_START_REGION 0

struct ram_region
{
    uint64_t base;
    uint64_t length;
};

extern struct ram_region usable_regions[128];
extern uint64_t region_count;


void init_pmm(uint64_t total_ram);
void free_page(uint64_t address);
uint64_t alloc_page();
uint64_t alloc_pages(uint64_t num_pages);
void free_pages(uint64_t address, uint64_t num_pages);
void clear_page(uint64_t address);

#endif