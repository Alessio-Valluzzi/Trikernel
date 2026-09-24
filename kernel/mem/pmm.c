#include "pmm.h"

extern uint64_t hhdm_offset;

static uint64_t bitmap_start_page = 0;
static uint8_t *bitmap_p = 0;
static uint8_t *bitmap = 0;
static uint64_t bitmap_size = 0;

void init_pmm(struct ram_region *regions, uint64_t count)
{
    uint64_t max_address = 0;

    if (regions == 0 || count == 0)
    {
        panic("PMM: no RAM regions");
        return;
    }

    for (uint64_t i = 0; i < count; i++)
    {
        uint64_t end =
            regions[i].base +
            regions[i].length;

        if (end > max_address)
            max_address = end;
    }

    uint64_t total_pages =
        (max_address + PAGE_SIZE - 1) / PAGE_SIZE;

    bitmap_size =
        (total_pages + 7) / 8;

    uint64_t bitmap_pages =
        (bitmap_size + PAGE_SIZE - 1) / PAGE_SIZE;

    for (uint64_t i = BITMAP_START_REGION; i < count; i++)
    {
        uint64_t aligned_base =
            (regions[i].base + PAGE_SIZE - 1)
            & ~(PAGE_SIZE - 1);

        uint64_t region_end =
            regions[i].base +
            regions[i].length;

        if (region_end <= aligned_base)
            continue;

        uint64_t available_length =
            region_end -
            aligned_base;

        if (available_length >= bitmap_pages * PAGE_SIZE)
        {
            bitmap_start_page =
                aligned_base / PAGE_SIZE;

            bitmap_p =
                (uint8_t *)aligned_base;

            bitmap =
                (uint8_t *)(aligned_base + hhdm_offset);

            break;
        }
    }

    if (bitmap == 0)
    {
        panic("PMM: failed to find space for bitmap");
        return;
    }

    for (uint64_t i = 0; i < bitmap_size; i++)
        bitmap[i] = 0xFF;

    for (uint64_t i = 0; i < count; i++)
    {
        uint64_t start_page =
            (regions[i].base + PAGE_SIZE - 1) /
            PAGE_SIZE;

        uint64_t end_page =
            (regions[i].base + regions[i].length) /
            PAGE_SIZE;

        for (uint64_t page = start_page;
             page < end_page;
             page++)
        {
            bitmap[page / 8] &=
                ~(1U << (page % 8));
        }
    }

    for (uint64_t page = bitmap_start_page;
         page < bitmap_start_page + bitmap_pages;
         page++)
    {
        bitmap[page / 8] |=
            (1U << (page % 8));
    }
}

uint64_t alloc_page(void)
{
    if (bitmap == 0)
    {
        panic("PMM: bitmap is NULL");
        return 0;
    }

    if (bitmap_size == 0)
    {
        panic("PMM: bitmap_size is 0");
        return 0;
    }

    for (uint64_t i = 0; i < bitmap_size; i++)
    {
        if (bitmap[i] != 0xFF)
        {
            for (uint8_t bit = 0; bit < 8; bit++)
            {
                if ((bitmap[i] & (1U << bit)) == 0)
                {
                    bitmap[i] |= (1U << bit);

                    uint64_t page =
                        (i * 8) + bit;

                    return page * PAGE_SIZE;
                }
            }
        }
    }

    return 0;
}

void free_page(uint64_t address)
{
    if (bitmap == 0)
    {
        panic("PMM: bitmap is NULL");
        return;
    }

    uint64_t page =
        address / PAGE_SIZE;

    uint64_t byte =
        page / 8;

    if (byte >= bitmap_size)
    {
        panic("PMM: invalid page");
        return;
    }

    bitmap[byte] &=
        ~(1U << (page % 8));
}

uint64_t alloc_pages(uint64_t num_pages)
{
    if (bitmap == 0)
    {
        panic("PMM: bitmap is NULL");
        return 0;
    }

    if (bitmap_size == 0)
    {
        panic("PMM: bitmap_size is 0");
        return 0;
    }

    if (num_pages == 0)
        return 0;

    uint64_t start_page = 0;
    uint64_t found_pages = 0;

    for (uint64_t i = 0; i < bitmap_size; i++)
    {
        if (bitmap[i] == 0xFF)
        {
            found_pages = 0;
            continue;
        }

        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if ((bitmap[i] & (1U << bit)) == 0)
            {
                if (found_pages == 0)
                    start_page = (i * 8) + bit;

                found_pages++;

                if (found_pages == num_pages)
                {
                    for (uint64_t page = start_page;
                         page < start_page + num_pages;
                         page++)
                    {
                        bitmap[page / 8] |=
                            (1U << (page % 8));
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

    return 0;
}

void free_pages(uint64_t address, uint64_t num_pages)
{
    if (bitmap == 0)
    {
        panic("PMM: bitmap is NULL");
        return;
    }

    if (num_pages == 0)
        return;

    uint64_t start_page =
        address / PAGE_SIZE;

    uint64_t end_page =
        start_page + num_pages;

    for (uint64_t page = start_page;
         page < end_page;
         page++)
    {
        uint64_t byte =
            page / 8;

        if (byte >= bitmap_size)
        {
            panic("PMM: invalid page range");
            return;
        }

        bitmap[byte] &=
            ~(1U << (page % 8));
    }
}

void clear_page(uint64_t address)
{
    uint8_t *page =
        (uint8_t *)(address + hhdm_offset);

    for (uint64_t i = 0;
         i < PAGE_SIZE;
         i++)
    {
        page[i] = 0;
    }
}