#include "util.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

void print_hex(char *str, uint8_t len)
{
    while (len > 0)
    {
        printf(" %#02x,", *str);
        str++;
        len--;
    }
    printf("\r\n");
}

uint8_t makeCheck(uint8_t *data, uint8_t len)
{
    uint8_t u8Result = 0;
    uint8_t i;
    for (i = 0; i < len; i++)
    {
        u8Result ^= *data;
        data++;
    }

    return u8Result;
}

void hex2str(uint8_t *pbDest, uint8_t *pbSrc, int nLen)
{
    char ddl, ddh;
    int i;

    for (i = 0; i < nLen; i++)
    {
        ddh = 48 + pbSrc[i] / 16;
        ddl = 48 + pbSrc[i] % 16;
        if (ddh > 57)
            ddh = ddh + 7;
        if (ddl > 57)
            ddl = ddl + 7;
        pbDest[i * 2] = ddh;
        pbDest[i * 2 + 1] = ddl;
    }

    pbDest[nLen * 2] = '\0';
}

//十六进制字符串转换为字节流
void HexStrToByte(const char* source, unsigned char* dest, int sourceLen)
{
    short i;
    unsigned char highByte, lowByte;

    for (i = 0; i < sourceLen; i += 2)
    {
        highByte = toupper(source[i]);
        lowByte  = toupper(source[i + 1]);

        if (highByte > 0x39)
            highByte -= 0x37;
        else
            highByte -= 0x30;

        if (lowByte > 0x39)
            lowByte -= 0x37;
        else
            lowByte -= 0x30;

        dest[i / 2] = (highByte << 4) | lowByte;
    }
    return ;
}
