#ifndef __UNITY_H
#define __UNITY_H

#include <stdint.h>

int pStringLen(char *pstr);
uint32_t crc32_mpeg_2(uint8_t *data, uint8_t length);

// /* simulate FPGA */
// /* Init FPGA UART(UART1) Config */
// void fpga_open(void);

// /* Return NULL if failed */
// char *getDecrypeKey(void);

// /*
//  * param    : pufChallenge must be an array, don't use dynamic memory(malloc)
//  * return   : return NULL if failed
// */
// char *getPufResponse(char *pufChallenge);

// /*
//  * param    :   dataToEncrype => must be an array, don't use dynamic memory(malloc)
//  * return   :   return NULL if failed                
// */
// char *getLblockEncrypeData(char *dataToEncrype,size_t len,int *lenAfterEncrype);

#endif