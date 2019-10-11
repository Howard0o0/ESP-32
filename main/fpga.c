#include "fpga.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include <string.h>
#include <stdio.h>
#include "freertos/task.h"
#include "util.h"

#define UART1_TXD  (GPIO_NUM_2)
#define UART1_RXD  (GPIO_NUM_15)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)
#define BUF_SIZE 128
#define WAIT_MSG_FROM_FPGA_MS 150
#define FPGA_RSP_LEN    1+8+10+4
#define KEY_LEN  
#define TRY_RCV_TIMES 10
#define CHALLENGE_OFFSET_IN_FINGERPRINT 4
#define LEN_PUF_CHALLENGE 40

enum FPGA_ACK_ERR
{
    ERR_LEN = 1,
    ERR_NOACK,
    ERR_FUNCODE,
    ERR_CHECK
};



void UART1_install(void)
{
     /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, UART1_TXD, UART1_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);

}

int sendCmdToFPGA(uint8_t *cmd,uint8_t cmdLen)
{
    int iWriteLen = uart_write_bytes(UART_NUM_1, (const char *)cmd, cmdLen);
    // vTaskDelay(150 / portTICK_RATE_MS);     // period btween 2 send has to > 150ms
    return iWriteLen;
}

int rcvRspFromFPGA(uint8_t *rcvBuf)
{
    int i;
    int iReadBytes;
    for(i = 0;i < TRY_RCV_TIMES;i++)
    {
        iReadBytes = uart_read_bytes(UART_NUM_1, rcvBuf, BUF_SIZE, WAIT_MSG_FROM_FPGA_MS / portTICK_RATE_MS);
        if(iReadBytes > 0)
            {
                // printf("rcvFromFpag: ");
                // print_hex((char *)rcvBuf,iReadBytes);
                return iReadBytes;
            }
    }

    return 0;
}

/*
 * params   :   
*/
int getPufResponse(uint8_t *challenge,uint8_t *Response)
{
    /* 1. make cmd */
    uint8_t cmd[11];
    cmd[0] = 0x0A;  //len
    cmd[1] = 0xAA;  //funcode
    memcpy(cmd+2,challenge,8);  //challenge

    uint8_t u8Check = 0;
    int i;
    for(i=2;i<10;i++)
    {
        u8Check ^= cmd[i];
    }
    cmd[10] = u8Check;

    /* 2. send cmd */
    sendCmdToFPGA(cmd,11);
    // printf("cmd : ");
    // print_hex((char *)cmd,11);
    
    /* 3. try to receive rsp */
    uint8_t u8RcvBuf[50];
    uint8_t u8RcvLen;
    u8RcvLen = rcvRspFromFPGA(u8RcvBuf);
    /* 3.1 exam len */
    if(u8RcvLen == 0)
        return ERR_NOACK;
    else if(u8RcvLen != 7)
    {
        return ERR_LEN;
    }
    /* 3.2 exam funcode */  
    if(u8RcvBuf[1] != 0xAA)
        return ERR_FUNCODE;
    /* 3.3 exam Check */
    if( (u8RcvBuf[2] ^ u8RcvBuf[3] ^ u8RcvBuf[4] ^ u8RcvBuf[5]) != u8RcvBuf[6] )
        return ERR_CHECK;
    /* 3.4 return Result */
    memcpy(Response,u8RcvBuf+2,4);
    return 0;

}

static void extract40BytesChallenge(uint8_t *au8FingerprintFromServer,uint8_t *au8Challenge40Bytes)
{
    memcpy(au8Challenge40Bytes,au8FingerprintFromServer+CHALLENGE_OFFSET_IN_FINGERPRINT,LEN_PUF_CHALLENGE);
}

int finigerprintTo20bytesResponse(uint8_t *au8Fingerprint,uint8_t *au8PufResp)
{
    // uint8_t au8Fingerprint[199] = {0xf5, 00, 00, 00, 0xd, 0x6, 0x14, 0x40, 0x21, 0x1d, 0x9e, 0x98, 0xc1, 0x3c, 0x19, 0x80, 0x41, 0x40, 0x12, 0xd5, 0xe1, 0x4b, 0x1e, 0x1, 0x21, 0x5, 0x18, 0xd8, 0x42, 0xd, 0x21, 0x42, 0x62, 0x16, 0x90, 0x40, 0x2, 0x1a, 0x1b, 00, 0xc2, 0x25, 0x25, 0xc3, 0x2, 0x2c, 0x87, 0x53, 0xe2, 0x2e, 0x1c, 0x81, 0xc2, 0x51, 0x1b, 0x2c, 0xa2, 00, 0xfe, 0xff, 0xff, 00, 00, 00, 0x60, 00, 0x80, 00, 0x2, 00, 0x32, 00, 00, 00, 0x1, 00, 00, 00, 00, 00, 00, 00, 0x1, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0xb2, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0x2d, 0xf5};
    uint8_t au8Challenge40bytes[40];
    // uint8_t au8PufResp[20];
    int iRet;


    /* 2. extract challenge and make response */
    extract40BytesChallenge(au8Fingerprint,au8Challenge40bytes);
    // print_hex((char *)au8Challenge40bytes,40);
    int i;
    uint8_t au8PufResponse4Bytes[4];
    for(i=0;i<5;i++)
    {
        uint8_t u8TryTimes = 10;
        while( (iRet = getPufResponse(au8Challenge40bytes+i*8,au8PufResponse4Bytes) != 0)  && (u8TryTimes > 0) )
        {
            printf("fpga not resp ...\r\n");
        }
        
        if(iRet != 0)
        {
            printf("fpga communication error, exit \r\n");
            return -1;
        }

        memcpy(au8PufResp+i*4,au8PufResponse4Bytes,4);
        
    }

    return 0;
}


int get80bitPuf(uint8_t *pu8Puf80Bit)
{
    int iRet;
    int iStableFlag = 0;
    int iErrorTimes = 0;
    uint8_t pu8Challenge8bytes[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };
    uint8_t pu8Challenge8bytes2[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t pu8TmpPufResp4Bytes[10][4];

    int i;
    int iPufIndex = 0;
    /* get head 4 bytes puf */
    for(i = 0;i < 10;i++)
    {
        if(iErrorTimes >= 3)
        {
            return -1;
        }
        iRet = getPufResponse(pu8Challenge8bytes,pu8TmpPufResp4Bytes[iPufIndex++]);
        if(iRet != 0)
        {
            iErrorTimes++;
            iPufIndex--;
        }

    }

    i=0;
    while (pu8TmpPufResp4Bytes[i+5] != NULL)
    {
        if( (strstr( (char *)pu8TmpPufResp4Bytes[i],(char *)pu8TmpPufResp4Bytes[i+1]) != 0) && \
        (strstr( (char *)pu8TmpPufResp4Bytes[i],(char *)pu8TmpPufResp4Bytes[i+2]) != 0) && \
        (strstr( (char *)pu8TmpPufResp4Bytes[i],(char *)pu8TmpPufResp4Bytes[i+3]) != 0) && \
        (strstr( (char *)pu8TmpPufResp4Bytes[i],(char *)pu8TmpPufResp4Bytes[i+4]) != 0) && \
        (strstr( (char *)pu8TmpPufResp4Bytes[i],(char *)pu8TmpPufResp4Bytes[i+5]) != 0)    )
        {
            iStableFlag = 1;
            memcpy(pu8Puf80Bit,pu8TmpPufResp4Bytes[i],4);
            break;
        }
        i++;
    }
    if(!iStableFlag)
    {
        printf("can't get stable puf ! \r\n");
        return -1;
    }

     /* get tail 4 bytes puf */
    iErrorTimes = 0;
    iPufIndex = 0;
    for(i = 0;i < 10;i++)
    {
        if(iErrorTimes >= 3)
        {
            return -1;
        }
        iRet = getPufResponse(pu8Challenge8bytes2,pu8TmpPufResp4Bytes[iPufIndex++]);
        if(iRet != 0)
        {
            iErrorTimes++;
            iPufIndex--;
        }

    }

    i=0;
    iStableFlag = 0;
    while (pu8TmpPufResp4Bytes[i+5] != NULL)
    {
        if( (strstr( (char *)pu8TmpPufResp4Bytes[i],(char *)pu8TmpPufResp4Bytes[i+1]) != 0) && \
            (strstr( (char *)pu8TmpPufResp4Bytes[i],(char *)pu8TmpPufResp4Bytes[i+2]) != 0) && \
            (strstr( (char *)pu8TmpPufResp4Bytes[i],(char *)pu8TmpPufResp4Bytes[i+3]) != 0) && \
            (strstr( (char *)pu8TmpPufResp4Bytes[i],(char *)pu8TmpPufResp4Bytes[i+4]) != 0) && \
            (strstr( (char *)pu8TmpPufResp4Bytes[i],(char *)pu8TmpPufResp4Bytes[i+5]) != 0)    )
        {
            iStableFlag = 1;
            memcpy(pu8Puf80Bit+4,pu8TmpPufResp4Bytes[i],4);
            break;
        }
        i++;
    }
    if(!iStableFlag)
    {
        printf("can't get stable puf ! \r\n");
        return -1;
    }

    /* fill the last 2 bytes of au8Puf80bit */
    pu8Puf80Bit[8] = 0xF0;
    pu8Puf80Bit[9] = 0x51;

    return 0;
    

}

// void fpga_test(void)
// {
//     UART1_install();

//     uint8_t au8Challenge[] = {0x11 ,0x22 ,0x33 ,0x44 ,0x55 ,0x66 ,0x77 ,0x88};
//     uint8_t au8Resp[4];
//     int iRet;

//     while (1)
//     {
//         iRet = getPufResponse(au8Challenge,au8Resp);
//         if(iRet == 0)
//         {
//             print_hex((char *)au8Resp,4);
//         }
//         else
//         {
//             printf("iRet : %d \r\n",iRet);
//         }

//         vTaskDelay(1000 / portTICK_RATE_MS);
//     }

// }

void fpga_test(void)
{
    UART1_install();

    uint8_t au8Puf80bit[10];
    int iRet;

    while (1)
    {
        iRet = get80bitPuf(au8Puf80bit);
        if (iRet == 0) //success
        {
            printf("80bit puf : \r\n");
            print_hex((char *)au8Puf80bit,10);
        }
        else
        {
            printf("get 80bit puf failed \r\n");
        }
        

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}