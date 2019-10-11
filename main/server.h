#ifndef __SERVER_H
#define __SERVER_H

#include <stdint.h>

void UART2_install(void);


void SendMsgToClient(uint8_t *msg,uint8_t len);
uint8_t *RcvMsgFroClient_block(uint8_t *len); 


int waitClientAuthenticationRequest(uint8_t *u8ClientId);

void errHandle(int error);

int getRespFromClient(uint8_t* ClientRsp);



uint8_t byteHammDist(uint8_t u8byte1,uint8_t u8byte2);

uint8_t calcHammingDist(uint8_t *u8str1,uint8_t *u8str2,uint8_t len);

void sendAuthenticateResult(uint8_t *result);
int authenticate_server(void);

int register_server(void);


#endif