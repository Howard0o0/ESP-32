/*
 * author   :   howard
 * date     :   2019/10/01
*/

#ifndef __FPGA_H
#define __FPGA_H

#include <stdint.h>


/*!
 * @brief : fpga connect mcu via uart1 , need to init uart1 before drive fpga
 */
void UART1_install(void);

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


/*!
 * @brief :PUF(au8Fingerprint)=au8PufResp
 *
 * @param[in] au8Fingerprint  : 199 bytes fingerprint
 * @param[out] au8PufResp  : 4bytes Response
 *
 * @retval  0 -> success 
 *          otherwise -> fail
 */
int finigerprintTo20bytesResponse(uint8_t *au8Fingerprint,uint8_t *au8PufResp);

int rcvRspFromFPGA(uint8_t *rcvBuf);
int sendCmdToFPGA(uint8_t *cmd,uint8_t cmdLen);
void fpga_test(void);


#endif