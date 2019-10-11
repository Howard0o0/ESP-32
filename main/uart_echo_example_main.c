/* UART Echo Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include <string.h>
#include <esp_task_wdt.h>
#include "szm301.h"
#include "fpga.h"
#include "util.h"
#include "server.h"
#include "authenticate.h"

/**
 * This is an example which echos any data it receives on UART0 back to the sender,
 * with hardware flow control turned off. It does not use UART driver event queue.
 *
 * - Port: UART0
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: off
 * - Pin assignment: see defines below
 */



uint8_t acCmdCmpFingerprint[199];
void szm301Test(void)
{
    gpio_set_direction(GPIO_NUM_16,GPIO_MODE_OUTPUT );
    gpio_set_level(GPIO_NUM_16,1);


    uart2_install();
    memset(acCmdCmpFingerprint,0,200);
    while (1)
    {
        if(acCmdCmpFingerprint[0] == 0)  // fingerprint character is empty, sample a fingerprint as standard fingerprint
        {
            printf("waiting finger... \r\n");
            uint8_t *pcFingerprint = getFingerprintCharacter();
            if(pcFingerprint)   // sample a fingerprint
            {
                printf("finger print: \r\n");
                print_hex((char *)pcFingerprint,199);
                memcpy(acCmdCmpFingerprint,pcFingerprint,199);
                printf("add to standard fingerprint \r\n");
            }
        }
        else    // standard fingerprint exist
        {
            printf("put your finger to authentication \r\n");
            int iRet = sampleFingerprintAndCmpCharacter(acCmdCmpFingerprint);
            if(iRet == 0)
            {
                printf("matched ! \r\n");
            }
            else
            {
                printf("not matched! \r\n");
            }
            
        }

        // int iMatchGrade = getMatchGrade();
        // if(iMatchGrade > 0)
        // {
        //     printf("current match grade : %d \r\n",iMatchGrade);
        // }
        
        
    }

}

void server_main(void)
{
    int iRet;
    UART2_install();

    iRet = register_server();
    if(iRet != 0)
        return;

    authenticate_server();
    if(iRet != 0)
        return;



}

void getPufResp(void)
{
    UART1_install();

    uint8_t au8Fingerprint[199] = {0xf5, 00, 00, 00, 0xd, 0x6, 0x14, 0x40, 0x21, 0x1d, 0x9e, 0x98, 0xc1, 0x3c, 0x19, 0x80, 0x41, 0x40, 0x12, 0xd5, 0xe1, 0x4b, 0x1e, 0x1, 0x21, 0x5, 0x18, 0xd8, 0x42, 0xd, 0x21, 0x42, 0x62, 0x16, 0x90, 0x40, 0x2, 0x1a, 0x1b, 00, 0xc2, 0x25, 0x25, 0xc3, 0x2, 0x2c, 0x87, 0x53, 0xe2, 0x2e, 0x1c, 0x81, 0xc2, 0x51, 0x1b, 0x2c, 0xa2, 00, 0xfe, 0xff, 0xff, 00, 00, 00, 0x60, 00, 0x80, 00, 0x2, 00, 0x32, 00, 00, 00, 0x1, 00, 00, 00, 00, 00, 00, 00, 0x1, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0xb2, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0x2d, 0xf5};
    uint8_t au8Challenge40bytes[40];
    uint8_t au8PufResp[20];
    int iRet;

    int i;
    for(i=0;i<10;i++)
    {
        finigerprintTo20bytesResponse(au8Fingerprint,au8PufResp);
        printf("pufRsp : \r\n");
        print_hex((char *)au8PufResp,20);
    }

}

void uart0_test(void)
{
    #define UART0_TXD  (GPIO_NUM_18)
    #define UART0_RXD  (GPIO_NUM_19)
    #define ECHO0_TEST_RTS  (UART_PIN_NO_CHANGE)
    #define ECHO0_TEST_CTS  (UART_PIN_NO_CHANGE)
    #define uBUF_SIZE 128

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART0_TXD, UART0_RXD, ECHO0_TEST_RTS, ECHO0_TEST_CTS);
    uart_driver_install(UART_NUM_0, uBUF_SIZE * 2, 0, 0, NULL, 0);

    int iReadBytes;
    uint8_t rcvBuf[uBUF_SIZE];

    while (1)
    {
        uart_write_bytes(UART_NUM_0, (const char *)"hahaha", 6);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void app_main()
{
    // echo_task();
    // szm301Test();
    // fpga_test();
    // getPufResp();
    // uart0_test();
    xTaskCreate(fpga_test, "uart_echo_task", 1024, NULL, 10, NULL);
}
