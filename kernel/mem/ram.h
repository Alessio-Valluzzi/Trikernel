#ifndef RAM_H
#define RAM_H

#include "types.h"
#include "limine.h"
#include "pmm.h"
#include "vmm.h"

#define PAGE_SIZE 4096
#define BITMAP_START_REGION 0



void init_ram(struct limine_memmap_response *memmap_response);
uint64_t ram_get_total();
uint64_t ram_get_usable();

#endif