#ifndef __FPGA_H
#define __FPGA_H

#include <stdint.h>


void UART1_install(void);


int sendCmdToFPGA(uint8_t *cmd,uint8_t cmdLen);


int rcvRspFromFPGA(uint8_t *rcvBuf);

/*!
 * @brief : get Puf Response
 *
 * @param[in] challenge  : only accept 8 bytes challenge
 * @param[out] Response  : 4bytes Response
 *
 * @retval  0 -> success 
 *          otherwise -> fail
 */
int getPufResponse(uint8_t *challenge,uint8_t *Response);


/*!
 * @brief :get 80bit origin Puf
 *
 * @param[out] pu8Puf80Bit  : 10bytes origin Puf
 *
 * @retval  0 -> success 
 *          1 -> fail
 */
int get80bitPuf(uint8_t *pu8Puf80Bit);


int get80bitPuf(uint8_t *pu8Puf80Bit);

int finigerprintTo20bytesResponse(uint8_t *au8Fingerprint,uint8_t *au8PufResp);

void fpga_test(void);


#endif