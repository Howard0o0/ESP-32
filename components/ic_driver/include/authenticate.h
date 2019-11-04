#ifndef __AUTHENTICATE_H
#define __AUTHENTICATE_H

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "cJSON.h"


cJSON *makeJson_CR(void);
cJSON *makeJson_R(void);
void makeJson_test_CR(void);
void makeJson_test_R(void);
void registerAndAuth_test(void);


#endif