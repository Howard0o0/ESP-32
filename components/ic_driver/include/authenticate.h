#ifndef __AUTHENTICATE_H
#define __AUTHENTICATE_H

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "cJSON.h"


void getDeviceId(uint8_t *value, uint16_t *len);
cJSON *makeJson_CR(void);
cJSON *makeJson_R(void);
void makeJson_test_CR(void);
void makeJson_test_R(void);
void registerAndAuth_test(void);
cJSON *makeJsonForRegister(void);
cJSON *makeJsonRegisterErr(void);
cJSON *makeJsonForAuthenticate(uint8_t *pu8Challenge);
cJSON *makeJsonAuthenticateErr(void);


#endif