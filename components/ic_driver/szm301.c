#include "szm301.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include <string.h>
#include "freertos/FreeRTOS.h"

#define WAIT_RSP_FROM_FINGERMODULE_MS 500
#define BUF_SIZE 1024
#define ECHO_TEST_TXD  (GPIO_NUM_26)
#define ECHO_TEST_RXD  (GPIO_NUM_27)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

void uart2_install(void)
{
     /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 19200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(UART_NUM_2, &uart_config);
    uart_set_pin(UART_NUM_2, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
    uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0);

}

uint8_t au8UartRcvBuf[BUF_SIZE];
uint8_t au8FingerCharacter[199];
uint8_t *getFingerprintCharacter(void)
{
    /* send Cmd */
    char acCmdGetFingerCharacter[] = {0xF5,0x23,0x0,0x0,0x0,0x0,0x23,0xF5};
    uart_write_bytes(UART_NUM_2, (const char *)acCmdGetFingerCharacter, sizeof(acCmdGetFingerCharacter));

    /* wait Resp */ 
    int i = 0;;
    int uartRcvLen;
    while(i<10)
    {
        uartRcvLen = uart_read_bytes(UART_NUM_2, au8UartRcvBuf, BUF_SIZE, WAIT_RSP_FROM_FINGERMODULE_MS / portTICK_RATE_MS);
        if(uartRcvLen > 0)
            break;
        i++;
    }
    if(uartRcvLen == 0) //time out when waiting Resp
    {
        printf("time out when waiting Resp in getFingerCharacter()\r\n");
        return NULL;
    }
    if(uartRcvLen != 207) // finger module will return msgHead(8bytes)+msgBody(199bytes)
    {
        printf("rcv len error in getFingerCharacter(): %d bytes \r\n",uartRcvLen);
        return NULL;
    }

    /* extract character au8UartRcvBuf[8:206] and return */
    memcpy(au8FingerCharacter,au8UartRcvBuf+8,199);

    return au8FingerCharacter;
}

int setSampleFingerprintTimeOut(uint8_t u8MaxTimeS)
{
    uint8_t u8Time = 5 * u8MaxTimeS;
    uint8_t u8Check = 0x2E ^ 0x0 ^ u8Time ^ 0x0 ^ 0x0;
    char pcSetTimeCmd[] = {0xF5,0x2E,0x0,u8Time,0x0,0x0,u8Check,0xF5};
    
    uart_write_bytes(UART_NUM_2, (const char *)pcSetTimeCmd, sizeof(pcSetTimeCmd));

    int i = 0;;
    int uartRcvLen;
    while(i<10)
    {
        uartRcvLen = uart_read_bytes(UART_NUM_2, au8UartRcvBuf, BUF_SIZE, WAIT_RSP_FROM_FINGERMODULE_MS / portTICK_RATE_MS);
        if(uartRcvLen > 0)
            break;
        i++;
    }
    if(uartRcvLen == 0) //time out when waiting Resp
    {
        printf("time out when waiting Resp in setSampleFingerTimeOut() \r\n");
        return -1;
    }
    if(uartRcvLen != 8) // finger module will return msgHead(8bytes)+msgBody(199bytes)
    {
        printf("rcv len error in setSampleFingerTimeOut() : %d bytes \r\n",uartRcvLen);
        return -2;
    }

    /* judge if Success */
    if(au8UartRcvBuf[4] == 0x0)
    {
        return 0;
    }
    else
    {
        return -3;
    }
    
}

int sampleFingerprintAndCmpCharacter(uint8_t *pu8Character)
{
    char acCmd[207] = { 0xF5 ,0x44 ,0x00 ,0xC4 ,0x00 ,0x00 ,0x80 ,0xF5 };
    memcpy(acCmd+8,pu8Character,199);

    uart_write_bytes(UART_NUM_2, (const char *)acCmd, sizeof(acCmd));

    int i = 0;;
    int uartRcvLen;
    while(i<10)
    {
        uartRcvLen = uart_read_bytes(UART_NUM_2, au8UartRcvBuf, BUF_SIZE, WAIT_RSP_FROM_FINGERMODULE_MS / portTICK_RATE_MS);
        if(uartRcvLen > 0)
            break;
        i++;
    }
    if(uartRcvLen == 0) //time out when waiting Resp
    {
        printf("time out when waiting Resp in setSampleFingerTimeOut() \r\n");
        return -2;
    }
    if(uartRcvLen != 8) // finger module will return msgHead(8bytes)+msgBody(199bytes)
    {
        printf("rcv len error in setSampleFingerTimeOut() : %d bytes \r\n",uartRcvLen);
        return -3;
    }

    /* judge if Success */
    if(au8UartRcvBuf[4] == 0x0)
    {
        return 0;
    }
    else if(au8UartRcvBuf[4] == 0x1) //fail
    {
        return -1;
    }
    else    //wait finger time out
    {
        return -4;
    }
    
}

int setMatchGrade(uint8_t grade)
{
    uint8_t u8Check = 0x28 ^ 0x0 ^ 0x0 ^grade  ^ 0x0;
    uint8_t pu8Cmd[] = {0xF5,0x28,0x0,0x0,grade,0x0,u8Check,0xF5};
    uart_write_bytes(UART_NUM_2, (const char *)pu8Cmd, sizeof(pu8Cmd));

    int i = 0;;
    int uartRcvLen;
    while(i<10)
    {
        uartRcvLen = uart_read_bytes(UART_NUM_2, au8UartRcvBuf, BUF_SIZE, WAIT_RSP_FROM_FINGERMODULE_MS / portTICK_RATE_MS);
        if(uartRcvLen > 0)
            break;
        i++;
    }
    if(uartRcvLen == 0) //time out when waiting Resp
    {
        printf("time out when waiting AT-Resp in setMatchGrade() \r\n");
        return -2;
    }
    if(uartRcvLen != 8) // finger module will return msgHead(8bytes)+msgBody(199bytes)
    {
        printf("rcv len error in setMatchGrade() : %d bytes \r\n",uartRcvLen);
        return -3;
    }

    /* judge if Success */
    if(au8UartRcvBuf[4] == 0x0)
    {
        return 0;
    }
    else     //fail
    {
        return -1;
    }

}

int getMatchGrade(void)
{
    uint8_t u8Check = 0x28 ^ 0x0 ^ 0x1 ^ 0x1  ^ 0x0;
    uint8_t pu8Cmd[] = {0xF5,0x28,0x0,0x1,0x1,0x0,u8Check,0xF5};
    uart_write_bytes(UART_NUM_2, (const char *)pu8Cmd, sizeof(pu8Cmd));

    int i = 0;;
    int uartRcvLen;
    while(i<10)
    {
        uartRcvLen = uart_read_bytes(UART_NUM_2, au8UartRcvBuf, BUF_SIZE, WAIT_RSP_FROM_FINGERMODULE_MS / portTICK_RATE_MS);
        if(uartRcvLen > 0)
            break;
        i++;
    }
    if(uartRcvLen == 0) //time out when waiting Resp
    {
        printf("time out when waiting AT-Resp in setMatchGrade() \r\n");
        return -2;
    }
    if(uartRcvLen != 8) // finger module will return msgHead(8bytes)+msgBody(199bytes)
    {
        printf("rcv len error in setMatchGrade() : %d bytes \r\n",uartRcvLen);
        return -3;
    }

    /* judge if Success */
    if(au8UartRcvBuf[4] == 0x0)
    {
        return (int)au8UartRcvBuf[3];
    }
    else     //fail
    {
        return -1;
    }
}

