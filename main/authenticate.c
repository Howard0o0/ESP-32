#include "authenticate.h"
#include "fpga.h"
#include "szm301.h"
#include "string.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define UART2_TXD  (GPIO_NUM_2)
#define UART2_RXD  (GPIO_NUM_15)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)
#define BUF_SIZE 128
#define WAIT_MSG_FROM_SERVER_MS 150
#define CHALLENGE_OFFSET_IN_FINGERPRINT 4
#define LEN_PUF_RESPONSE 20
#define LEN_PUF_CHALLENGE 40
#define LEN_FINGERPRINT 199

void author_uart_install(void)
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

void sendAuthenticationRequest(uint8_t id)
{
    uart_write_bytes(UART_NUM_2, (const char *)&id, 1);
}

int getFingerprintFromServer(uint8_t *fingerprint)
{
    int i;
    int iUart2RcvLen;
    for(i=0;i<10;i++)
    {
        iUart2RcvLen = uart_read_bytes(UART_NUM_2, fingerprint, BUF_SIZE, WAIT_MSG_FROM_SERVER_MS / portTICK_RATE_MS);
        if(iUart2RcvLen > 0)
            return iUart2RcvLen;
    }

    return 0;
}

void extract40BytesChallenge(uint8_t *au8FingerprintFromServer,uint8_t *au8Challenge40Bytes)
{
    memcpy(au8Challenge40Bytes,au8FingerprintFromServer+CHALLENGE_OFFSET_IN_FINGERPRINT,LEN_PUF_CHALLENGE);
}

int sendPufResponseToServer(uint8_t *au8PufResponse20Bytes)
{
    uart_write_bytes(UART_NUM_2, (const char *)au8PufResponse20Bytes, LEN_PUF_RESPONSE);

    return 0;
}

int rcvServerResult(uint8_t *result)
{
    int i;
    int iUart2RcvLen;
    uint8_t au8Uart2RcvBuf[50];

    for(i=0;i<10;i++)
    {
        iUart2RcvLen = uart_read_bytes(UART_NUM_2, au8Uart2RcvBuf, 50, WAIT_MSG_FROM_SERVER_MS / portTICK_RATE_MS);
        if(iUart2RcvLen > 0)
        {
            *result = au8Uart2RcvBuf[0];
            return iUart2RcvLen;
        }
    }

    return 0;
}

void errHandle(int error)
{
    printf("error ret : %d \r\n",error);
}

int CRpRegister(void)
{
    uint8_t au8Fingerprint[199];
    uint8_t au8Challenge40bytes[40];
    uint8_t au8PufResp[20];
    int iRet;

    /* 1. sample fingerprint */
    uint8_t *pu8Fingerprint = getFingerprintCharacter();
    memcpy(au8Fingerprint,pu8Fingerprint,199);

    /* 2. extract challenge and make response */
    extract40BytesChallenge(au8Fingerprint,au8Challenge40bytes);
    int i;
    uint8_t au8PufResponse4Bytes[4];
    for(i=0;i<5;i++)
    {
        iRet = getPufResponse(au8Challenge40bytes+i*8,au8PufResponse4Bytes);
        if (iRet != 0)
        {
            errHandle(iRet);
        }
        memcpy(au8PufResp+i*4,au8PufResponse4Bytes,4);
        
    }

    /* 3. send fingerprint and response */
    uart_write_bytes(UART_NUM_2, (const char *)au8Fingerprint, 199);
    uart_write_bytes(UART_NUM_2, (const char *)au8PufResp, 20);

    return 0;
}

int authenticate(void)
{
    int iRet;
    uint8_t iId = 0;
    uint8_t au8FingerprintFromServer[LEN_FINGERPRINT];
    uint8_t au8Challenge40Bytes[LEN_PUF_CHALLENGE];
    uint8_t au8PufResponse20Bytes[LEN_PUF_RESPONSE];
    uint8_t au8PufResponse4Bytes[4];




    /* 1. send authentication request */
    sendAuthenticationRequest(iId);
    printf("send authentication to server \r\n");

    /* 2. get fingerprint contained challenge from server*/
    iRet = getFingerprintFromServer(au8FingerprintFromServer);
    printf("got stdFingerprint from server \r\n");

    /* 3. sample fingerprintTmp and match */
    printf("waiting for finger \r\n");
    iRet = sampleFingerprintAndCmpCharacter(au8FingerprintFromServer);
    if (iRet != 0)
    {
        errHandle(iRet);
    }
    printf("fingerprint matched \r\n");
    
    
    /* 4. extract challenge(40bytes) from fingerprint */
    extract40BytesChallenge(au8FingerprintFromServer,au8Challenge40Bytes);
    printf("extracted challenge from stdFingerprint\r\n");

    /* 5. get pufResponse using challenge */
    int i;
    for(i=0;i<5;i++)
    {
        iRet = getPufResponse(au8Challenge40Bytes+i*8,au8PufResponse4Bytes);
        if (iRet != 0)
        {
            errHandle(iRet);
        }
        memcpy(au8PufResponse20Bytes+i*4,au8PufResponse4Bytes,4);
        
    }

    /* 6. send pufResponse to server */
    sendPufResponseToServer(au8PufResponse20Bytes);
    printf("sent puf response to server \r\n");

    /* 7. Receive server's ACK and judge authentication result */
    uint8_t u8Result;
    iRet = rcvServerResult(&u8Result);
    if(iRet <= 0)
    {
        printf("SERVER not ACK\r\n");
        return 0;
    }
    if(u8Result == 1)
    {
        printf("Server authenticated! \r\n");
        return 1;
    }
    else
    {
        printf(" authenticate failed \r\n ");
        return 0;
    }
    

}