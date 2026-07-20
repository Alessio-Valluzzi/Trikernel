#include "keyboard.h"
#include "screen/screen.h"

uint16_t status_p = 0x64;
uint16_t dat_p = 0x60;
uint16_t ready_S = 0xAE;

int init_keyboard(){
    if(inb(status_p) == 0xFF)
        return -1;
    outb(status_p, ready_S);
    print("\n[OK] PS/2 Keyboard initialized", 0xFFFFFF);
    return 0;
}   

uint8_t keyboard_read()
{
    // controlla se c'è un dato
    if(inb(status_p) & 1)
    {
        return inb(dat_p);
    }

    return 0;
}
