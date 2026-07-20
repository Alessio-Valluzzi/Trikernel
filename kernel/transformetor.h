#include "types.h"
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