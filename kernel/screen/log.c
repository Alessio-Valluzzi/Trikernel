#include "log.h"

void panic(char* str){
    print("\n[KERNEL PANIC] ", 0xFF0000);
    print(str, 0xFFFFFF);
    __asm__ volatile("cli");
    while(1){
        __asm__ volatile("hlt");
    }
}

void OK(char* str){
    print("\n[OK] ", 0x00FF00);
    print(str, 0xFFFFFF);
}


void warning(char* str){
    print("\n[WARNING] ", 0xFFFF00);
    print(str, 0xFFFFFF);
}

void info(char* str){
    print("\n[INFO] ", 0x0000FF);
    print(str, 0xFFFFFF);
}