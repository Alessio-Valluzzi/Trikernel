#ifndef VMM_H
#define VMM_H

#include "types.h"
#include "pmm.h"
#include "screen/log.h"

void init_vmm();
void map_page(uint64_t physical, uint64_t virtual);
void unmap_page(uint64_t virtual);

#endif