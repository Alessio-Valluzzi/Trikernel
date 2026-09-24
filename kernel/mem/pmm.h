#ifndef PMM_H
#define PMM_H

#include "types.h"
#include "screen/log.h"
#define PAGE_SIZE 4096
#define MAX_RAM_REGIONS 128
#define BITMAP_START_REGION 0
struct ram_region
{
    uint64_t base;
    uint64_t length;
};

extern struct ram_region usable_regions[MAX_RAM_REGIONS];
extern uint64_t region_count;
extern uint64_t hhdm_offset;

void init_pmm(struct ram_region *regions, uint64_t count);
void free_page(uint64_t address);
uint64_t alloc_page();
uint64_t alloc_pages(uint64_t num_pages);
void free_pages(uint64_t address, uint64_t num_pages);
void clear_page(uint64_t address);

#endif