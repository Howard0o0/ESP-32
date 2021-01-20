#include "fpga.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdio.h>
#include "freertos/task.h"
#include "util.h"
#include "max77752.h"
#include <stdlib.h>
#include <string.h>

// #define UART1_TXD  (GPIO_NUM_2)
// #define UART1_RXD  (GPIO_NUM_15)
#define UART1_TXD (GPIO_NUM_22)
#define UART1_RXD (GPIO_NUM_23)
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)
#define PIN_FPGA_nRST (GPIO_NUM_21)
#define BUF_SIZE 128
#define WAIT_MSG_FROM_FPGA_MS 150
#define FPGA_RSP_LEN 1 + 8 + 10 + 4
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

void fpgaDriverInstall(void)
{
        static bool driver_installed = false;
        if (driver_installed)
        {
                printf("fpgaDriverInstall repeat exec ! no need to init again \r\n\r\n");
                return;
        }

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

        /* init nRst Pin */
        gpio_set_direction(PIN_FPGA_nRST, GPIO_MODE_OUTPUT);
        gpio_set_level(PIN_FPGA_nRST, 1);

        driver_installed = true;
}

void rstFpga(void)
{
        gpio_set_level(PIN_FPGA_nRST, 1);
        vTaskDelay(50 / portTICK_RATE_MS);
        gpio_set_level(PIN_FPGA_nRST, 0);
        vTaskDelay(50 / portTICK_RATE_MS);
        gpio_set_level(PIN_FPGA_nRST, 1);
}

int sendCmdToFPGA(uint8_t *cmd, uint8_t cmdLen)
{
        rstFpga();
        int iWriteLen = uart_write_bytes(UART_NUM_1, (const char *)cmd, cmdLen);
        // vTaskDelay(150 / portTICK_RATE_MS);     // period btween 2 send has to > 150ms
        return iWriteLen;
}

int rcvRspFromFPGA(uint8_t *rcvBuf)
{
        int i;
        int iReadBytes;
        for (i = 0; i < TRY_RCV_TIMES; i++)
        {
                iReadBytes = uart_read_bytes(UART_NUM_1, rcvBuf, BUF_SIZE, WAIT_MSG_FROM_FPGA_MS / portTICK_RATE_MS);
                if (iReadBytes > 0)
                {
                        // printf("rcvFromFpag: ");
                        // print_hex((char *)rcvBuf, iReadBytes);
                        return iReadBytes;
                }
                printf("waiting fpga response...\r\n");
        }

        return 0;
}

/*
 * params   :   
*/
int getPufResponse(uint8_t *challenge, uint8_t *Response)
{
        /* 1. make cmd */
        uint8_t cmd[11];
        cmd[0] = 0x0A;                 //len
        cmd[1] = 0xAA;                 //funcode
        memcpy(cmd + 2, challenge, 8); //challenge

        uint8_t u8Check = 0;
        int i;
        for (i = 2; i < 10; i++)
        {
                u8Check ^= cmd[i];
        }
        cmd[10] = u8Check;

        /* 2. send cmd */
        sendCmdToFPGA(cmd, 11);
        // printf("cmd : ");
        // print_hex((char *)cmd,11);

        /* 3. try to receive rsp */
        uint8_t u8RcvBuf[50] = {0};
        uint8_t u8RcvLen;
        u8RcvLen = rcvRspFromFPGA(u8RcvBuf);
        /* 3.1 exam len */
        if (u8RcvLen == 0)
                return ERR_NOACK;
        else if (u8RcvLen != 7)
        {
                return ERR_LEN;
        }
        /* 3.2 exam funcode */
        if (u8RcvBuf[1] != 0xAA)
                return ERR_FUNCODE;
        /* 3.3 exam Check */
        if ((u8RcvBuf[2] ^ u8RcvBuf[3] ^ u8RcvBuf[4] ^ u8RcvBuf[5]) != u8RcvBuf[6])
                return ERR_CHECK;
        /* 3.4 return Result */
        memcpy(Response, u8RcvBuf + 2, 4);
        return 0;
}

static void extract40BytesChallenge(uint8_t *au8FingerprintFromServer, uint8_t *au8Challenge40Bytes)
{
        memcpy(au8Challenge40Bytes, au8FingerprintFromServer + CHALLENGE_OFFSET_IN_FINGERPRINT, LEN_PUF_CHALLENGE);
}

int finigerprintTo20bytesResponse(uint8_t *au8Fingerprint, uint8_t *au8PufResp)
{
        // uint8_t au8Fingerprint[199] = {0xf5, 00, 00, 00, 0xd, 0x6, 0x14, 0x40, 0x21, 0x1d, 0x9e, 0x98, 0xc1, 0x3c, 0x19, 0x80, 0x41, 0x40, 0x12, 0xd5, 0xe1, 0x4b, 0x1e, 0x1, 0x21, 0x5, 0x18, 0xd8, 0x42, 0xd, 0x21, 0x42, 0x62, 0x16, 0x90, 0x40, 0x2, 0x1a, 0x1b, 00, 0xc2, 0x25, 0x25, 0xc3, 0x2, 0x2c, 0x87, 0x53, 0xe2, 0x2e, 0x1c, 0x81, 0xc2, 0x51, 0x1b, 0x2c, 0xa2, 00, 0xfe, 0xff, 0xff, 00, 00, 00, 0x60, 00, 0x80, 00, 0x2, 00, 0x32, 00, 00, 00, 0x1, 00, 00, 00, 00, 00, 00, 00, 0x1, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0xb2, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0x2d, 0xf5};
        uint8_t au8Challenge40bytes[40];
        // uint8_t au8PufResp[20];
        int iRet;

        /* 2. extract challenge and make response */
        extract40BytesChallenge(au8Fingerprint, au8Challenge40bytes);
        // print_hex((char *)au8Challenge40bytes,40);
        int i;
        uint8_t au8PufResponse4Bytes[4];
        for (i = 0; i < 5; i++)
        {
                uint8_t u8TryTimes = 10;
                while ((iRet = getPufResponse(au8Challenge40bytes + i * 8, au8PufResponse4Bytes) != 0) && (u8TryTimes > 0))
                {
                        rstFpga();
                        printf("fpga not resp ...\r\n");
                        u8TryTimes--;
                }

                if (iRet != 0)
                {
                        printf("fpga communication error, exit \r\n");
                        return -1;
                }

                memcpy(au8PufResp + i * 4, au8PufResponse4Bytes, 4);
        }

        return 0;
}

void PrintHex(uint8_t *data, int len)
{
        for (int i = 0; i < len; i++)
                printf("%02X ", data[i]);
        printf("\r\n");
}

bool IsSame(uint8_t *data1, uint8_t *data2, int len)
{
        if (!data1 || !data2)
                return false;

        for (int i = 0; i < len; i++)
        {
                if (data1[i] != data2[i])
                        return false;
        }

        return true;
}

uint8_t *FindMajorElementIndex(uint8_t **data, int first_dim_len,
                               int second_dim_len)
{
        uint8_t *candidata = NULL;
        int count = 0;

        for (int i = 0; i < first_dim_len; i++)
        {

                uint8_t *curr_tgt = (uint8_t *)data + i * second_dim_len;
                if (IsSame(curr_tgt, candidata, second_dim_len))
                        ++count;

                else if (--count < 0)
                {
                        candidata = curr_tgt;
                        count = 1;
                }
        }

        count = 0;
        for (int i = 0; i < first_dim_len; i++)
        {
                uint8_t *curr_tgt = (uint8_t *)data + i * second_dim_len;
                if (IsSame(curr_tgt, candidata, second_dim_len))
                        ++count;
        }

        if (count > first_dim_len / 2)
                return candidata;

        return NULL;
}

int get80bitPuf(uint8_t *pu8Puf80Bit)
{
#define PUF_SAMPLE_TIMES 5

        int iRet;
        int iStableFlag = 0;
        int iErrorTimes = 0;
        uint8_t pu8Challenge8bytes[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
        uint8_t pu8Challenge8bytes2[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        uint8_t pu8TmpPufResp4Bytes[PUF_SAMPLE_TIMES][4];

        int i;
        int iPufIndex = 0;
        /* get head 4 bytes puf */
        for (i = 0; i < PUF_SAMPLE_TIMES; i++)
        {
                if (iErrorTimes >= 3)
                {
                        return -1;
                }
                rstFpga();
                iRet = getPufResponse(pu8Challenge8bytes, pu8TmpPufResp4Bytes[iPufIndex++]);
                if (iRet != 0)
                {
                        iErrorTimes++;
                        iPufIndex--;
                }
        }

        uint8_t *stable_puf_a = FindMajorElementIndex((uint8_t **)pu8TmpPufResp4Bytes, PUF_SAMPLE_TIMES, 4);
        if (!stable_puf_a)
        {
                printf("can't get stable puf ! \r\n");
                return -1;
        }
        memcpy(pu8Puf80Bit, stable_puf_a, 4);

        /* get tail 4 bytes puf */
        iErrorTimes = 0;
        iPufIndex = 0;
        for (i = 0; i < PUF_SAMPLE_TIMES; i++)
        {
                if (iErrorTimes >= 3)
                {
                        return -1;
                }
                rstFpga();
                iRet = getPufResponse(pu8Challenge8bytes2, pu8TmpPufResp4Bytes[iPufIndex++]);
                if (iRet != 0)
                {
                        iErrorTimes++;
                        iPufIndex--;
                }
        }

        uint8_t *stable_puf_b = FindMajorElementIndex((uint8_t **)pu8TmpPufResp4Bytes, PUF_SAMPLE_TIMES, 4);
        if (!stable_puf_b)
        {
                printf("can't get stable puf ! \r\n");
                return -1;
        }
        memcpy(pu8Puf80Bit + 4, stable_puf_b, 4);

        /* fill the last 2 bytes of au8Puf80bit */
        pu8Puf80Bit[8] = 0xF0;
        pu8Puf80Bit[9] = 0x51;

        return 0;
}

/* len : 10Byte */
bool GetLblockKey(uint8_t *lblock_key)
{
        if (get80bitPuf(lblock_key) == 0)
                return true;
        return false;
}

static uint8_t *getLblockResponseAccordingToId(char *id)
{
        static uint8_t lblockResponse[5][10] = {
            {0xE6,0x3B ,0x09 ,0xD6 ,0x24 ,0x80 ,0x35 ,0xFA}, // ID : "0D"
            {0xE8, 0xED, 0x93, 0x53, 0x2C, 0xAE, 0x5A, 0x59}, // ID : "0B"
            {0x1B,0x00 ,0x0A ,0xBB ,0x35 ,0x8A ,0x9A ,0xAB }, // ID : "0F"
            {},
            {}};

        if (strstr(id, "0D") != NULL)
                return lblockResponse[0];
        if (strstr(id, "0B") != NULL)
                return lblockResponse[1];
        if (strstr(id, "0F") != NULL)
                return lblockResponse[2];

        printf("unrecord id : %s \r\n", id);
        return NULL;
}

bool GetStableLblockKey(uint8_t *stable_lblock_key, char *id)
{
        uint8_t dirty_key[11];
        int errCnt = 0;
        uint8_t *standard_enc_data = getLblockResponseAccordingToId(id);
        uint8_t dirty_enc_data[8];

        while (errCnt < 10)
        {
                printf("trying to get stable lblock key , round %d \r\n", errCnt);
                if (GetLblockKey(dirty_key) != true)
                {
                        errCnt++;
                        printf("[ERROR] cannot get first-step stable key from fpga \r\n");
                        continue;
                }
                printf("key : ");
                PrintHex(dirty_key, 10);
                if (lblock_encrype_8bytes((uint8_t *)"12345678", dirty_key, dirty_enc_data) != 0)
                {
                        errCnt++;
                        printf("[ERROR] get wrong lblock key \r\n");
                        continue;
                }
                printf("enc : ");
                PrintHex(dirty_enc_data, 8);
                if (IsSame(dirty_enc_data, standard_enc_data, 8) == true)
                {
                        memcpy(stable_lblock_key, dirty_key, 10);
                        printf("[INFO] got stable lblock key: ");
                        PrintHex(dirty_key, 10);
                        return true;
                }
                errCnt++;
        }

        return false;
}

uint8_t *GetLblockKeyForId(char *id)
{
        static uint8_t lblockKey[11] = {0};
        if (lblockKey[8] == 0xF0 && lblockKey[9] == 0x51)
                return lblockKey;

        if (GetStableLblockKey(lblockKey, id) == true)
                return lblockKey;
        return NULL;
}

int lblock_encrype_8bytes(uint8_t *pu8PlainText, uint8_t *pu8Key, uint8_t *pu8EncrypedData)
{
        int iRet;
        uint8_t au8RcvBuf[15] = {0};

        /* 1.early terminate */
        if (pu8PlainText == NULL || pu8EncrypedData + 7 == NULL || pu8Key == NULL || pu8Key + 9 == NULL)
        {
                return -1;
        }

        /* 2. make cmd */
        uint8_t au8Cmd[1 + 1 + 8 + 10 + 1];
        au8Cmd[0] = 0x14;
        au8Cmd[1] = 0x55;
        memcpy(au8Cmd + 2, pu8PlainText, 8);
        memcpy(au8Cmd + 10, pu8Key, 10);
        // uint8_t u8Check1 = makeCheck(pu8PlainText,8);
        // uint8_t u8Check2 = makeCheck(pu8Key,10);
        uint8_t u8Check = makeCheck(au8Cmd + 2, 18);
        au8Cmd[1 + 1 + 8 + 10] = u8Check;

        /* 3. send cmd, receive result */
        sendCmdToFPGA(au8Cmd, 1 + 1 + 8 + 10 + 1);
        // printf("cmd :");
        // print_hex((char *)au8Cmd, 1 + 1 + 8 + 10 + 1);
        iRet = rcvRspFromFPGA(au8RcvBuf);
        if (iRet <= 0)
        {
                printf("no rcv from fpga \r\n");
                return -1;
        }

        /* 4. check */
        uint8_t u8CheckRcv = makeCheck(au8RcvBuf + 2, 8);
        if (u8CheckRcv != au8RcvBuf[10])
        {
                printf("check failed  \r\n");
                return -1;
        }

        /* 5. extract result and return  */
        memcpy(pu8EncrypedData, au8RcvBuf + 2, 8);

        return 0;
}

int lblock_encrype(uint8_t *pu8PlainText, uint8_t *pu8Key, uint8_t *pu8EncrypedData, uint8_t pu8LenbeforEncrype, uint8_t *pu8LenAfterEncrype)
{
        /* 1. exam if pu8PlainText and pu8Key legal */
        if (pu8PlainText == NULL || pu8EncrypedData + pu8LenbeforEncrype - 1 == NULL || pu8Key == NULL || pu8Key + 9 == NULL)
        {
                return -1;
        }

        /* 2. calc pu8LenAfterEncrype and encrype times , if need supplement */
        /* 2.1 if need supplement */
        uint8_t u8needSupplementFlag = 0;
        uint8_t u8RemainLen = pu8LenbeforEncrype % 8;
        if (u8RemainLen != 0)
        {
                u8needSupplementFlag = 1;
        }
        /* 2.2 calc encrype times */
        uint8_t u8EncrypeTimes = (pu8LenbeforEncrype / 8) + u8needSupplementFlag;
        /* 2.3 calc pu8LenAfterEncrype */
        *pu8LenAfterEncrype = u8EncrypeTimes * 8;

        /* 3. start encrype each 8 bytes except the last 8 bytes */
        int i;
        int iRet;
        uint8_t au8Encryped8btes[8];
        for (i = 0; i < u8EncrypeTimes - 1; i++)
        {
                rstFpga();
                iRet = lblock_encrype_8bytes(pu8PlainText + i * 8, pu8Key, au8Encryped8btes);
                if (iRet != 0)
                {
                        return -1;
                }
                memcpy(pu8EncrypedData + i * 8, au8Encryped8btes, 8);
        }

        /* 4. encrype the last 8 byte */
        uint8_t au8dataLeft[8] = {0};
        if (u8needSupplementFlag)
        {
                memcpy(au8dataLeft, pu8PlainText + i * 8, u8RemainLen);
        }
        else
        {
                memcpy(au8dataLeft, pu8PlainText + i * 8, 8);
        }
        iRet = lblock_encrype_8bytes(au8dataLeft, pu8Key, au8Encryped8btes);
        if (iRet != 0)
        {
                return -1;
        }
        memcpy(pu8EncrypedData + i * 8, au8Encryped8btes, 8);

        return 0;
}

uint8_t au8ChallengeT[199 + 1] = {0xf5, 00, 00, 00, 0xe, 0x2c, 0x27, 0xdb, 0x81, 0x30, 0x1e, 0x45, 0xe1, 0x36, 0x8f, 0xdc, 0x61, 0x41, 0xa8, 0x9e, 0x21, 0x46, 0x16, 0xdf, 0x1, 0x4a, 0x9d, 0x9f, 0x61, 0x4f, 0x8a, 0x1f, 0x81, 0x10, 0x94, 0x1c, 0x2, 0x27, 0x6, 0x9c, 0x62, 0x2d, 0x16, 0x1c, 0x2, 0x3d, 0x93, 0xc6, 0x22, 0x3d, 0xa1, 0xdd, 0x42, 0x41, 0x89, 0x1e, 0x2, 0x4d, 0x93, 0xc8, 0x42, 00, 00, 00, 0x60, 00, 0x80, 00, 0x2, 00, 0x32, 00, 00, 00, 0x1, 00, 00, 00, 00, 00, 00, 00, 0x1, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0xb2, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0xb0, 0xf5};
void fpga_test(void)
{
        fpgaDriverInstall();
        printf("start lblock test \r\n");

        /* test 80bit puf */
        uint8_t au8Puf80bit[10];
        int iRet;

        while (1)
        {
                iRet = GetLblockKey(au8Puf80bit);
                if (iRet == true) //success
                {
                        printf("lblock key : \r\n");
                        print_hex((char *)au8Puf80bit, 10);
                }
                else
                {
                        printf("get lblock key failed \r\n");
                }

                // iRet = get80bitPuf(au8Puf80bit);
                // if (iRet == 0) //success
                // {
                //         printf("80bit puf : \r\n");
                //         print_hex((char *)au8Puf80bit, 10);
                // }
                // else
                // {
                //         printf("get 80bit puf failed \r\n");
                // }

                vTaskDelay(1000 / portTICK_RATE_MS);
        }
        /* end test 80bit puf */

        /* test lblcok */
        // uint8_t au8data[] = {  0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef };
        // uint8_t au8data[20] = {  0};
        // uint8_t au8Key[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc};
        // int iRet = 0;

        // uint8_t au8Encryped[24];
        // while (1)
        // {
        //     uint8_t u8LenAfterEnc;
        //     iRet = lblock_encrype(au8data,au8Key,au8Encryped, 20, &u8LenAfterEnc);
        //     if(iRet != 0)
        //     {
        //         printf("lblock encrype 8  bytes failed ! \r\n");
        //         continue;
        //     }
        //     printf("encryped data : ");
        //     print_hex((char *)au8Encryped,24);
        // }
        /* end of test lblcok */

        /* test C-R */
        // int iRet;
        // uint8_t au8Resp[21];
        // while (1)
        // {
        //     rstFpga();
        //     MAX77752_printAllVolt();
        //     iRet = finigerprintTo20bytesResponse(au8ChallengeT,au8Resp);
        //     if(iRet != 0)
        //     {
        //         printf(" get R failed \r\n");

        //     }
        //     else
        //     {
        //         printf("R: \r\n");
        //         print_hex((char *)au8Resp,20);
        //     }
        // }

        /* test C-R */
}

void TestLblockKeyStability()
{
        fpgaDriverInstall();
        /* test 80bit puf */
        uint8_t standard_key[10];
        uint8_t curr_key[10];
        int iRet;
        uint8_t standard_key_enc[8];

        while (GetLblockKey(standard_key) != true)
                vTaskDelay(1000 / portTICK_RATE_MS);

        printf("standard key : ");
        print_hex((char *)standard_key, 10);

        while (1)
        {

                iRet = GetLblockKey(curr_key);
                if (iRet != true) //success
                {
                        printf("get lblock key failed \r\n");
                        vTaskDelay(1000 / portTICK_RATE_MS);
                        continue;
                }

                printf("lblock key : ");
                print_hex((char *)curr_key, 10);
                if (lblock_encrype_8bytes((uint8_t *)"12345678", curr_key, standard_key_enc) == 0)
                {
                        printf("after enc : ");
                        PrintHex(standard_key_enc, 8);
                }
        }
}

void PrintStableLblockKey()
{
        fpgaDriverInstall();
        uint8_t *lblockKey = NULL;
        while (1)
        {
                lblockKey = GetLblockKeyForId("0F");
                if (!lblockKey)
                {
                        printf("555 got lblock key failed \r\n");
                        vTaskDelay(1000 / portTICK_RATE_MS);
                        continue;
                }
                printf("Got Stable Lblock Key : ");
                PrintHex(lblockKey, 10);
                vTaskDelay(1000 / portTICK_RATE_MS);
        }
}