#include "ram.h"
#include "pmm.h"

struct ram_region usable_regions[MAX_RAM_REGIONS];
uint64_t region_count = 0;

static uint64_t total_ram = 0;
static uint64_t usable_ram = 0;

void init_ram(struct limine_memmap_response *memmap)
{
    total_ram = 0;
    usable_ram = 0;
    region_count = 0;

    if (memmap == 0)
    {
        panic("RAM: memmap is NULL");
        return;
    }

    for (uint64_t i = 0; i < memmap->entry_count; i++)
    {
        struct limine_memmap_entry *entry =
            memmap->entries[i];

        total_ram += entry->length;

        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            if (region_count >= MAX_RAM_REGIONS)
            {
                panic("RAM: too many regions");
                return;
            }

            usable_regions[region_count].base =
                entry->base;

            usable_regions[region_count].length =
                entry->length;

            region_count++;

            usable_ram += entry->length;
        }
    }

    panic("BEFORE PMM");
    init_pmm(usable_regions, region_count);
}