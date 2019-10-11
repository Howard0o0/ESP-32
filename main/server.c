#include <string.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "server.h"
#include <stdint.h>

#define UART2_TXD  (GPIO_NUM_2)
#define UART2_RXD  (GPIO_NUM_15)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)
#define BUF_SIZE 128
#define LEN_CHALLENGE 199
#define LEN_RSP 20
#define HAMMING_THRESHOLD 3
#define AUTHENTICATE_SUCCESS 1
#define AUTHENTICATE_FAIL 0
#define WAIT_MSG_FROM_Client_MS 200

void UART2_install(void)
{
     /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(UART_NUM_2, &uart_config);
    uart_set_pin(UART_NUM_2, UART2_TXD, UART2_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
    uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0);

}

/* globla variables */
uint8_t g_au8Uart2RcvBuf[BUF_SIZE];
uint8_t g_au8Fingerprint[LEN_CHALLENGE] = {0};
uint8_t g_au8PufResponse[LEN_RSP] = {0};

void SendMsgToClient(uint8_t *msg,uint8_t len)
{
    uart_write_bytes(UART_NUM_2, (const char *)msg, len);
}

uint8_t *RcvMsgFroClient_block(uint8_t *len)
{
    while (1)
    {
        *len = uart_read_bytes(UART_NUM_2, g_au8Uart2RcvBuf, BUF_SIZE, WAIT_MSG_FROM_Client_MS / portTICK_RATE_MS);
        if(*len > 0)
            return g_au8Uart2RcvBuf;
    }
    
}

int waitClientAuthenticationRequest(uint8_t *u8ClientId)
{
    uint8_t u8Len;
    uint8_t *pu8MsgFromClient = RcvMsgFroClient_block(&u8Len);
    if(u8Len == 1)
    {
        *u8ClientId = pu8MsgFromClient[0];
        return 0;
    }
    else
    {
        return -1;
    }
    
    
}

void errHandle(int error)
{
    printf("error ret : %d \r\n",error);
}



int getRespFromClient(uint8_t* ClientRsp)
{
    uint8_t u8Len;
    ClientRsp = RcvMsgFroClient_block(&u8Len);
    if((u8Len != LEN_RSP) || (ClientRsp == NULL) )
        return -1;
    
    return 0;
}

uint8_t byteHammDist(uint8_t u8byte1,uint8_t u8byte2)
{
    int i;
    uint8_t u8Dist = 0;
    for(i=0;i<8;i++)
    {
        if( ((u8byte1>>i)&&1) && ((u8byte2>>i)&&1) )
            u8Dist++;
    }

    return u8Dist;
}

uint8_t calcHammingDist(uint8_t *u8str1,uint8_t *u8str2,uint8_t len)
{
    int i;
    uint8_t u8ByteHammDist;
    uint8_t u8Dist = 0;
    for(i=0;i<len;i++)
    {
        u8ByteHammDist = byteHammDist(*(u8str1+i),*(u8str2+i));
        u8Dist += u8ByteHammDist;
    }

    return u8Dist;
}

void sendAuthenticateResult(uint8_t *result)
{
    uart_write_bytes(UART_NUM_2, (const char *)result, 1);
}

int register_server(void)
{
    uint8_t u8Len;
    uint8_t *pu8RcvBuf;

    /* 1. receive challenge */
    pu8RcvBuf = RcvMsgFroClient_block(&u8Len);
    if(u8Len != 199)
    {
        printf("register_server challenge len error : %u ",u8Len);
        return -1;
    }
    memcpy(g_au8Fingerprint,pu8RcvBuf,199);
    printf("challenge registered \r\n");

    /* 2. receive pufResponse */
    pu8RcvBuf = RcvMsgFroClient_block(&u8Len);
    if(u8Len != 20)
    {
        printf("register_server pufResponse len error : %u ",u8Len);
        return -1;
    }
    memcpy(g_au8PufResponse,pu8RcvBuf,20);
    printf("pufResponse registered \r\n");

    return 0;
}

int authenticate_server(void)
{
    int iRet;
    uint8_t u8ClientId;



    /* 1. wait client's authentication request and get client's id */
    printf("waiting client's authenticate request \r\n");
    iRet = waitClientAuthenticationRequest(&u8ClientId);
    if(iRet != 0 )
    {
        errHandle(iRet);
        return -1;
    }
    printf("recerived client[%u] authenticate request \r\n",u8ClientId);
    

    /* 2. pick a challenge according to id and send to client, wait response */
    SendMsgToClient(g_au8Fingerprint,LEN_CHALLENGE);  // send challenge to client
    printf("sent challenge to client[%u] \r\n",u8ClientId);

    /* 3. receive response, calc hamming_dist(respServer,resp), and send result to client */
    uint8_t au8ClientRsp[LEN_RSP];
    uint8_t u8HammingDist;
    iRet = getRespFromClient(au8ClientRsp);
    if(iRet != 0)
    {
        printf("error len of client's response \r\n");
        return -1;
    }
    printf("received response, starting to calc Hamming-Dist \r\n");
    // calc hamming-dist
    u8HammingDist = calcHammingDist(au8ClientRsp,g_au8PufResponse,20);
    if(u8HammingDist > HAMMING_THRESHOLD)   //fail
    {
        printf(" %u > %u , failed \r\n",u8HammingDist,HAMMING_THRESHOLD);
        uint8_t u8Result[] = {AUTHENTICATE_FAIL};
        uart_write_bytes(UART_NUM_2, (const char *)u8Result, 1);
    }
    else
    {
        printf(" %u < %u , success \r\n",u8HammingDist,HAMMING_THRESHOLD);
        uint8_t u8Result[] = {AUTHENTICATE_SUCCESS};
        uart_write_bytes(UART_NUM_2, (const char *)u8Result, 1); 
    }
    


    return 0;
}