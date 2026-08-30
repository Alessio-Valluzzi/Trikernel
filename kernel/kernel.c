#include "limine.h"
#include "types.h"
#include "screen/screen.h"
#include "keyboard/keyboard.h"
#include "screen/log.h"
#include "smbios/smbios.h"
#include "mem/ram.h"
#include "libs/string/string.h"

volatile unsigned long test = 123;

__attribute__((used, section(".limine_requests_start")))
volatile uint64_t limine_requests_start[] = LIMINE_REQUESTS_START_MARKER;


__attribute__((used, section(".limine_requests")))
volatile struct limine_framebuffer_request framebuffer_request =
{
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0,
    .response = 0
};


__attribute__((used, section(".limine_requests")))
volatile struct limine_smbios_request smbios_request =
{
    .id = LIMINE_SMBIOS_REQUEST_ID,
    .revision = 0,
    .response = 0
};

__attribute__((used, section(".limine_requests")))
volatile struct limine_memmap_request memmap_request =
{
    .id = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0,
    .response = 0
};

__attribute__((used, section(".limine_requests_end")))
volatile uint64_t limine_requests_end[] = LIMINE_REQUESTS_END_MARKER;


void kernel_main(void)
{   
    if (framebuffer_request.response == 0)
    {
        while (1)
        {
            asm volatile("hlt");
        }
    }

    struct limine_framebuffer *fb =
        framebuffer_request.response->framebuffers[0];

    screen_init(fb);

    clear_screen(0x000000);

    print("Loading...", 0xFFFFFF);
    uint32_t eax, ebx, ecx, edx;

    eax = 1;

    asm volatile(
        "cpuid"
        : "+a"(eax),
        "=b"(ebx),
        "=c"(ecx),
        "=d"(edx)
    );

    if(ecx & (1 << 31))
    {
        info("Running in VM");
     
    }

    if (smbios_request.response == 0)
    {
        warning("SMBios table not loaded");
    } else {
       smbios_init(smbios_request.response, true);
       OK("SMBios table loaded");
    }

    if (memmap_request.response == 0)
    {
        panic("Memory map not loaded");
    } else {
       init_ram(memmap_request.response);
       OK("Memory map loaded");
    }

}