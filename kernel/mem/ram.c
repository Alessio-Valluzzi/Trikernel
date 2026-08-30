#include "ram.h"
#include "pmm.h"


static uint64_t total_ram = 0;
static uint64_t usable_ram = 0;


void init_ram(struct limine_memmap_response *memmap)
{
    total_ram = 0;
    usable_ram = 0;

    for(uint64_t i = 0; i < memmap->entry_count; i++)
    {
        struct limine_memmap_entry *entry = memmap->entries[i];

        total_ram += entry->length;

        if(entry->type == LIMINE_MEMMAP_USABLE)
        {
            usable_regions[region_count].base = entry->base;
            usable_regions[region_count].length = entry->length;
            region_count++;
            usable_ram += entry->length;
        }
    }
    init_pmm(total_ram);
    uint64_t test = alloc_page();

    if (test == 0)
        panic("alloc_page failed");
    panic("PRIMA INIT VMM");
    init_vmm();
    
    
}

uint64_t ram_get_total()
{
    return total_ram;
}

uint64_t ram_get_usable()
{
    return usable_ram;
}