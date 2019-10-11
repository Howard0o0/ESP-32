#include "util.h"
#include <stdio.h>

void print_hex(char *str,uint8_t len)
{
    while(len > 0)
    {
        printf(" %#02x,",*str);
        str++;
        len--;
    }
    printf("\r\n");
    
}