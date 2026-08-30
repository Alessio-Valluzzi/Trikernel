#include "string.h"

int strcmp(const char *a, const char *b)
{
    while (*a && (*a == *b))
    {
        a++;
        b++;
    }

    return (unsigned char)*a - (unsigned char)*b;
}

void uint8_to_string(uint8_t num, char *str)
{
    int i = 0;

    if(num == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    char temp[4];

    while(num > 0)
    {
        temp[i++] = (num % 10) + '0';
        num /= 10;
    }

    int j = 0;

    while(i > 0)
    {
        str[j++] = temp[--i];
    }

    str[j] = '\0';
}

void uint64_to_string(uint64_t num, char *str)
{
    int i = 0;

    if(num == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    char temp[60];

    while(num > 0)
    {
        temp[i++] = (num % 10) + '0';
        num /= 10;
    }

    int j = 0;

    while(i > 0)
    {
        str[j++] = temp[--i];
    }

    str[j] = '\0';
}

void string_concat(char *dest, const char *src)
{
    while(*dest)
    {
        dest++;
    }

    while(*src)
    {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = '\0';
}

void uint16_to_string(uint16_t num, char *str)
{
    int i = 0;

    if(num == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    char temp[8];

    while(num > 0)
    {
        temp[i++] = (num % 10) + '0';
        num /= 10;
    }

    int j = 0;

    while(i > 0)
    {
        str[j++] = temp[--i];
    }

    str[j] = '\0';
}