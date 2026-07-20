#include "math.h"

#define SCALE 1000

int abs(int x){
    if(x < 0)
        return -x;
    return x;
}

int min(int x, int y){
    if(x > y)
        return x;
    return y;
}

int max(int x, int y){
    if(x < y)
        return x;

    return y;
}

int float_to_fixed(float value)
{
    return (int)(value * SCALE);
}

float fixed_to_float(int value)
{
    return (float)value / SCALE;
}