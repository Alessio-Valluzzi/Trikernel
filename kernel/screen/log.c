#include "log.h"

void panic(char* str){
    print("\n[PANIC] ", 0xFF0000);
    print(str, 0xFFFFFF);
    while(1){
        __asm__ volatile("hlt");
    }
}

void OK(char* str){
    print("\n[OK] ", 0x00FF00);
    print(str, 0xFFFFFF);
}
