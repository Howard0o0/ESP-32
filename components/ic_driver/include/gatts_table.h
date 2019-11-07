/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#ifndef MAIN_BLE_GATTS_TABLE_H_
#define MAIN_BLE_GATTS_TABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


/**/
//charnum:1,4
void change_char_value(uint8_t * value,int len,int charnum);
int get_char_value(uint8_t **value, uint16_t *len,int charnum);
void server_notify_client(void);
void server_notify_client_test(uint8_t * value,int len);
void blue_init();
/**/

/************spp*************/
#define spp_sprintf(s,...)         sprintf((char*)(s), ##__VA_ARGS__)
#define SPP_DATA_MAX_LEN           (512)
#define SPP_CMD_MAX_LEN            (20)
#define SPP_STATUS_MAX_LEN         (20)
#define SPP_DATA_BUFF_MAX_LEN      (2*1024)
///Attributes State Machine
enum{
    SPP_IDX_SVC,

    SPP_IDX_SPP_DATA_RECV_CHAR,
    SPP_IDX_SPP_DATA_RECV_VAL,

    SPP_IDX_SPP_DATA_NOTIFY_CHAR,
    SPP_IDX_SPP_DATA_NTY_VAL,
    SPP_IDX_SPP_DATA_NTF_CFG,

    // SPP_IDX_SPP_COMMAND_CHAR,
    // SPP_IDX_SPP_COMMAND_VAL,

    // SPP_IDX_SPP_STATUS_CHAR,
    // SPP_IDX_SPP_STATUS_VAL,
    // SPP_IDX_SPP_STATUS_CFG,

    SPP_IDX_NB,
};
/**************************/



/* Attributes State Machine */
enum
{
    IDX_SVC_A,
    IDX_CHAR_A,
    IDX_CHAR_VAL_A,
    IDX_CHAR_CFG_A,

    A,
};

enum
{

    IDX_SVC_B,
    IDX_CHAR_B,
    IDX_CHAR_VAL_B,

    B,
};

enum
{

    IDX_SVC_C,
    IDX_CHAR_C,
    IDX_CHAR_VAL_C,

    C,

};

enum
{
    IDX_SVC_D,
    IDX_CHAR_D,
    IDX_CHAR_VAL_D,

    D,
};

enum
{

    IDX_SVC_E,
    IDX_CHAR_E,
    IDX_CHAR_VAL_E,

    E,
    
};

enum
{

    IDX_SVC_ID,
    IDX_CHAR_ID,
    IDX_CHAR_VAL_ID,

    ID,
    
};

#endif


