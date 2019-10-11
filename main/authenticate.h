#ifndef __AUTHENTICATE_H
#define __AUTHENTICATE_H

#include <stdint.h>

void author_uart_install(void);

void sendAuthenticationRequest(uint8_t id);

int getFingerprintFromServer(uint8_t *fingerprint);

void extract40BytesChallenge(uint8_t *au8FingerprintFromServer,uint8_t *au8Challenge40Bytes);

int sendPufResponseToServer(uint8_t *au8PufResponse20Bytes);

int rcvServerResult(uint8_t *result);

void errHandle_client(int error);
int authenticate(void);

#endif