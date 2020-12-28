#ifndef __UTIL_H
#define __UTIL_H

#include <stdint.h>

void	print_hex(char* str, uint8_t len);
void	PrintHex(uint8_t* data, int len);
uint8_t makeCheck(uint8_t* data, uint8_t len);
void	HexStrToByte(const char* source, unsigned char* dest, int sourceLen);
void	hex2str(uint8_t* pbDest, uint8_t* pbSrc, int nLen);

#endif