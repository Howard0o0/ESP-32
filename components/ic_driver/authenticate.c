#include "authenticate.h"
#include "fpga.h"
#include "szm301.h"
#include "string.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "util.h"
#include "gatts_table.h"
#include "cJSON.h"


/* global variables */
uint8_t au8EncResp_[24+1] = {0x59, 0x72, 0x3f, 0x3d, 0x16, 0x20, 0x22, 0xb7, 0x44, 0xf4, 0x2f, 0xa, 0x22, 0xb3, 0x9, 0x3a, 0x15, 0xda, 0x79, 0x7e, 0xfe, 0xab, 0xdd, 0x82};
uint8_t au8Resp[20+1] = {0x7f, 0x2e, 0x84, 0x6f, 0xad, 0x2e, 0x5, 0x11, 0x29, 0x3e, 0x84, 0x71, 0xaa, 0x3e, 0x84, 0x59, 0x7c, 0x36, 0x15, 0x49} ;
char acChallengeStr_[199*2+1];
char qq[2];
char acEncRespStr_[24*2+1];
char q[2];
char pc8Resp[20+1];
uint8_t au8Challenge_[199+1] = {0xf5, 0, 0, 0, 0xe, 0x2c, 0x27, 0xdb, 0x81, 0x30, 0x1e, 0x45, 0xe1, 0x36, 0x8f, 0xdc, 0x61, 0x41, 0xa8, 0x9e, 0x21, 0x46, 0x16, 0xdf, 0x1, 0x4a, 0x9d, 0x9f, 0x61, 0x4f, 0x8a, 0x1f, 0x81, 0x10, 0x94, 0x1c, 0x2, 0x27, 0x6, 0x9c, 0x62, 0x2d, 0x16, 0x1c, 0x2, 0x3d, 0x93, 0xc6, 0x22, 0x3d, 0xa1, 0xdd, 0x42, 0x41, 0x89, 0x1e, 0x2, 0x4d, 0x93, 0xc8, 0x42, 0, 0, 0, 0x60, 0, 0x80, 0, 0x2, 0, 0x32, 0, 0, 0, 0x1, 0, 0, 0, 0, 0, 0, 0, 0x1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xb2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xb0, 0xf5};
uint8_t bleRcvBuf[512+1];
uint8_t bleSendBuf[512+1];
/* End of global variables */

cJSON *makeJson_CR(void)
{
    hex2str((uint8_t *)acEncRespStr_,au8EncResp_,24);
    hex2str((uint8_t *)acChallengeStr_,au8Challenge_,199);
    // Hex2Str((const char *)au8EncResp_,acEncRespStr_,24);
    // Hex2Str((const char *)au8Challenge_,acChallengeStr_,199);
    cJSON *cjMsg = cJSON_CreateObject();
    cJSON_AddStringToObject(cjMsg,"id","0B");
    cJSON_AddStringToObject(cjMsg,"state","ok");
    // cJSON_AddStringToObject(cjMsg,"status","ok");
    cJSON_AddStringToObject(cjMsg,"challenge",acChallengeStr_);
    cJSON_AddStringToObject(cjMsg,"ER",acEncRespStr_);

    return cjMsg;
}

void makeJson_test_CR(void)
{
    hex2str((uint8_t *)acEncRespStr_,au8EncResp_,24);
    hex2str((uint8_t *)acChallengeStr_,au8Challenge_,199);
    // Hex2Str((const char *)au8EncResp_,acEncRespStr_,24);
    // Hex2Str((const char *)au8Challenge_,acChallengeStr_,199);
    cJSON *cjMsg = cJSON_CreateObject();
    cJSON_AddStringToObject(cjMsg,"id","0a");
    cJSON_AddStringToObject(cjMsg,"state","ok");
    // cJSON_AddStringToObject(cjMsg,"status","ok");
    cJSON_AddStringToObject(cjMsg,"challenge",acChallengeStr_);
    cJSON_AddStringToObject(cjMsg,"ER",acEncRespStr_);
    // cJSON_AddNumberToObject(cjMsg,"Rlen",20);
    char *pcjson = cJSON_Print(cjMsg);

	/* send json to phone */
    server_notify_client_test((uint8_t *)pcjson,strlen(pcjson));
    printf("len(%d) , sent json : \r\n %s \r\n",(int)strlen(pcjson),pcjson);
    
}

cJSON *makeJson_R(void)
{
    hex2str((uint8_t *)pc8Resp,au8Resp,20);
    cJSON *cjMsg = cJSON_CreateObject();
    cJSON_AddStringToObject(cjMsg,"id","0B");
    cJSON_AddStringToObject(cjMsg,"state","ok");
    cJSON_AddStringToObject(cjMsg,"response",pc8Resp);

    return cjMsg;
}

void makeJson_test_R(void)
{
    hex2str((uint8_t *)pc8Resp,au8Resp,20);
    cJSON *cjMsg = cJSON_CreateObject();
    cJSON_AddStringToObject(cjMsg,"id","0A");
    cJSON_AddStringToObject(cjMsg,"state","ok");
    cJSON_AddStringToObject(cjMsg,"response",pc8Resp);
    char *pcjson = cJSON_Print(cjMsg);
	/* send json to phone */
    server_notify_client_test((uint8_t *)pcjson,strlen(pcjson));
    printf("len(%d) , sent json : \r\n %s \r\n",(int)strlen(pcjson),pcjson);
    
}




void registerAndAuth_test(void)
{
    blue_init();
    while (1)
    {
        uint16_t bleRcvLen;
        if(get_char_value(bleRcvBuf,&bleRcvLen,4) != 0) //if rcv nothing , no response
        {
            vTaskDelay(10 / portTICK_RATE_MS);
            continue;
        }

        /* get cmd */
        printf("BufLen:%d\r\n",bleRcvLen);
        printf("bleRcvBuf: \n %s \n",(char *)bleRcvBuf);
        cJSON *jsCmd;
        jsCmd = cJSON_Parse((const char *)bleRcvBuf);
        if(  cJSON_GetObjectItem( jsCmd , "cmd" ) == NULL ) //no "cmd" in jsCmd
        {
            printf("no key[cmd] in json received\r\n");
            continue;
        }

        char *pcCmd = cJSON_GetObjectItem( jsCmd , "cmd" )->valuestring;
        if(strstr(pcCmd,"register") != 0) //receive cmd:register
        {
            makeJson_test_CR();
        }

        if(strstr(pcCmd,"authenticate") != 0)
        {
            makeJson_test_R();
        }

    }
    
}