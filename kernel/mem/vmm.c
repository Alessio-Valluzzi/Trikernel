#include "vmm.h"

uint64_t *pml4_table;
uint64_t  pml4_phys;

#define PRESENT (1ULL << 0)
#define WRITE   (1ULL << 1)

void init_vmm()
{   
    uint64_t page = alloc_page();
    

    if(page == 0)
    {   
        panic("Failed to allocate page for PML4 table");
        return;
    }

    pml4_phys = alloc_page();
    
    if(pml4_phys == 0)
    {   
        panic("Failed to allocate page for PML4 physical address");
        return;
    }
    
    pml4_table = (uint64_t *)page;


    clear_page(page);



}


void map_page(uint64_t physical, uint64_t virtual)
{
    uint64_t pml4_index = (virtual >> 39) & 0x1FF;
    uint64_t pdpt_index = (virtual >> 30) & 0x1FF;
    uint64_t pd_index   = (virtual >> 21) & 0x1FF;
    uint64_t pt_index   = (virtual >> 12) & 0x1FF;



    if(!(pml4_table[pml4_index] & PRESENT))
    {
        uint64_t new_pdpt = alloc_page();

        if(new_pdpt == 0)
            return;

        clear_page(new_pdpt);

        pml4_table[pml4_index] =
            new_pdpt | PRESENT | WRITE;
    }


    uint64_t *pdpt_table =
        (uint64_t *)(pml4_table[pml4_index] & ~0xFFF);



    if(!(pdpt_table[pdpt_index] & PRESENT))
    {
        uint64_t new_pd = alloc_page();

        if(new_pd == 0)
            return;

        clear_page(new_pd);

        pdpt_table[pdpt_index] =
            new_pd | PRESENT | WRITE;
    }


    uint64_t *pd_table =
        (uint64_t *)(pdpt_table[pdpt_index] & ~0xFFF);


    if(!(pd_table[pd_index] & PRESENT))
    {
        uint64_t new_pt = alloc_page();

        if(new_pt == 0)
            return;

        clear_page(new_pt);

        pd_table[pd_index] =
            new_pt | PRESENT | WRITE;
    }


    uint64_t *pt_table =
        (uint64_t *)(pd_table[pd_index] & ~0xFFF);


    // PT -> RAM fisica

    pt_table[pt_index] =
        (physical & ~0xFFF) | PRESENT | WRITE;


    asm volatile(
        "invlpg (%0)"
        :
        : "r"(virtual)
        : "memory"
    );
}



void unmap_page(uint64_t virtual)
{
    uint64_t pml4_index = (virtual >> 39) & 0x1FF;
    uint64_t pdpt_index = (virtual >> 30) & 0x1FF;
    uint64_t pd_index   = (virtual >> 21) & 0x1FF;
    uint64_t pt_index   = (virtual >> 12) & 0x1FF;


    uint64_t *pdpt_table =
        (uint64_t *)(pml4_table[pml4_index] & ~0xFFF);

    uint64_t *pd_table =
        (uint64_t *)(pdpt_table[pdpt_index] & ~0xFFF);

    uint64_t *pt_table =
        (uint64_t *)(pd_table[pd_index] & ~0xFFF);


    pt_table[pt_index] = 0;


    asm volatile(
        "invlpg (%0)"
        :
        : "r"(virtual)
        : "memory"
    );
}