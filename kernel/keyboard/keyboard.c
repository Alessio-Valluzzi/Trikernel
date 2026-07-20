#include "keyboard.h"
#include "screen/screen.h"

uint16_t status_p = 0x64;
uint16_t dat_p = 0x60;
uint16_t ready_S = 0xAE;

char keymap[128] =
{
    0,      // 0x00
    27,     // ESC
    '1',    // 0x02
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
    '-',
    '=',
    '\b',   // Backspace
    '\t',   // Tab
    'q',
    'w',
    'e',
    'r',
    't',
    'y',
    'u',
    'i',
    'o',
    'p',
    '[',
    ']',
    '\n',   // Enter
    0,      // Ctrl
    'a',
    's',
    'd',
    'f',
    'g',
    'h',
    'j',
    'k',
    'l',
    ';',
    '\'',
    '`',
    0,      // Shift sinistro
    '\\',
    'z',
    'x',
    'c',
    'v',
    'b',
    'n',
    'm',
    ',',
    '.',
    '/',
};


int init_keyboard(){
    if(inb(status_p) == 0xFF)
        return -1;
    outb(status_p, ready_S);
    print("\n[OK] PS/2 Keyboard initialized", 0xFFFFFF);
    return 0;
}   

char keyboard_read()
{
    // controlla se c'è un dato
    if(inb(status_p) & 1)
    {
        return keymap[inb(dat_p)];
    }

    return 0;
}


