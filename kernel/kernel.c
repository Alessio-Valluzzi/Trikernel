#include "limine.h"
#include "types.h"
#include "screen.h"

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

    print("Hello, World!", 0xFFFFFF);

    while(1)
    {
        asm volatile("hlt");
    }

}