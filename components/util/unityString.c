#include "unityString.h"
#include <stdint.h>

int pStringLen(char *pstr)
{
    int len = 0;
    while (*pstr != 0)
    {
        len++;
        pstr++;
    }
    return len;
    
}

uint32_t crc32_mpeg_2(uint8_t *data, uint8_t length)  
{  
    uint8_t i;  
    uint32_t crc = 0xffffffff;  // Initial value  
    while(length--)  
    {  
        crc ^= (uint32_t)(*data++) << 24;// crc ^=(uint32_t)(*data)<<24; data++;  
        for (i = 0; i < 8; ++i)  
        {  
            if ( crc & 0x80000000 )  
                crc = (crc << 1) ^ 0x04C11DB7;  
            else  
                crc <<= 1;  
        }  
    }  
    return crc;  
}  




/* simulate fpga */
/* Init FPGA UART(UART1) Config */
// void fpga_open(void)
// {

// }

// /* Return NULL if failed */
// char simulateKey[] = "123456789a";
// char *getDecrypeKey(void)
// {
//     return simulateKey;
// }

// /*
//  * param    : pufChallenge must be an array, don't use dynamic memory(malloc)
//  * return   : return NULL if failed
// */
// char *getPufResponse(char *pufChallenge);

// /*
//  * param    :   dataToEncrype => must be an array, don't use dynamic memory(malloc)
//  * return   :   return NULL if failed                
// */
// char acEncrypedData[8];
// char *getLblockEncrypeData(char *dataToEncrype,size_t len,int *lenAfterEncrype)
// {
//     if (len > 8)
//     {
//         return NULL;
//     }

//     memset(acEncrypedData,0,8);
//     int i;
//     for(i = 0;i < len;i++)
//     {
//         acEncrypedData[i] = dataToEncrype[i]+1;
//     }
//     *lenAfterEncrype = 8;

//     return acEncrypedData;
// }