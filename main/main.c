/* Blink Example

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
#include "authenticate.h"
#include "gatts_table.h"
#include "max77752.h"
#include "iic.h"
#include "cJSON.h"
#include "freertosTest.h"
#include "taskDoubleFactorAuthenticate.h"
#include "rgb_led.h"
#include "task_sensor.h"
#include "max30102.h"
#include "bmi160_defs.h"

void allDriverInstall(void);

void app_main()
{
        /* ==========MAX30102 TEST ==================*/
        // MAX30102_test();

        /* ==========End of MAX30102 TEST ==================*/

        // /* BMI160 TEST */
        // I2c_Master_Init();
        // while(BMI160_init() != BMI160_OK){
        //         printf("main BMI160 init error\r\n");
        // }
        // while(1){
        //         uint16_t step = 0;
        //         step = BMI160_read_step_count();
        //         printf("cur step : %u\r\n", step);
        //         vTaskDelay(1000 / portTICK_RATE_MS);
        // }
        // /* BMI160 TEST */


        /* MAX30205 TEST */
        // I2c_Master_Init();
        // while(1){
        //         float temp = 0;
        //         temp = MAX30205_ReadTemperature();
        //         printf("cur temp : %f\r\n", temp);
        //         vTaskDelay(1000 / portTICK_RATE_MS);
        // }
        /* MAX30205 TEST */

        /* test fingerprint */
        // szm301DriverInstall();
        // while (1)
        // {
        // 	gpio_pad_select_gpio(GPIO_NUM_26);
        // 	gpio_set_direction(GPIO_NUM_26,GPIO_MODE_OUTPUT);
        // 	gpio_set_level(GPIO_NUM_26,1);
        // 	uint8_t *pu8Challenge = getFingerprintCharacter();
        // 	if(pu8Challenge != NULL)
        // 	{
        // 		printf("fingerprint : ");
        // 		print_hex((char *)pu8Challenge,199);
        // 	}
        // }

        /* End of test fingerprint */

        /* test RGB pins as uart pins */

        // #define ECHO_TEST_TXD  (GPIO_NUM_14)
        // #define ECHO_TEST_RXD  (GPIO_NUM_12)
        // #define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
        // #define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

        // uart_config_t uart_config = {
        //     .baud_rate = 115200,
        //     .data_bits = UART_DATA_8_BITS,
        //     .parity = UART_PARITY_DISABLE,
        //     .stop_bits = UART_STOP_BITS_1,
        //     .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
        // uart_param_config(UART_NUM_2, &uart_config);
        // uart_set_pin(UART_NUM_2, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
        // uart_driver_install(UART_NUM_2, 1024 * 2, 0, 0, NULL, 0);

        // uint8_t au8UartRcvBuf[512];

        // while (1)
        // {
        // 	uart_write_bytes(UART_NUM_2, (const char *)"hello \n", sizeof("hello \n"));
        // 	int uartRcvLen = uart_read_bytes(UART_NUM_2, au8UartRcvBuf, 512, 100 / portTICK_RATE_MS);
        //     if(uartRcvLen > 0)
        // 	{
        // 		printf("rcv : %s \n",(char *)au8UartRcvBuf);
        // 	}
        // vTaskDelay(1000 / portTICK_RATE_MS);
        // }

        /* End of test RGB pins as uart pins */

        /* RGB led Test */
        // #define LED_R GPIO_NUM_26
        // #define LED_G GPIO_NUM_14
        // #define LED_B GPIO_NUM_12

        // gpio_pad_select_gpio(LED_R);
        // gpio_pad_select_gpio(LED_G);
        // gpio_pad_select_gpio(LED_B);
        // gpio_set_direction(GPIO_NUM_12,GPIO_MODE_OUTPUT);
        // gpio_set_level(GPIO_NUM_12,1);
        // gpio_set_direction(GPIO_NUM_14,GPIO_MODE_OUTPUT);
        // gpio_set_level(GPIO_NUM_14,1);
        // gpio_set_direction(GPIO_NUM_26,GPIO_MODE_OUTPUT);
        // gpio_set_level(GPIO_NUM_26,1);

        // while (1)
        // {
        // 	printf("running ...\r\n");
        // 	vTaskDelay(1000 / portTICK_RATE_MS);
        // }

        /* End of RGB led Test */

        /* ============read ble id =========================*/
        // uint8_t *pu8DeviceId=calloc(50,sizeof(uint8_t));
        // uint16_t u16LenDeviceId;
        // blue_init();
        // vTaskDelay(1000 / portTICK_RATE_MS);
        // while (1)
        // {
        // 	if ( get_char_value(&pu8DeviceId, &u16LenDeviceId,4) == 0)
        // 	{
        // 		// getDeviceId(pu8DeviceId, &u16LenDeviceId);
        // 		// printf("pu8DeviceId = %s \r\n",(char *)pu8DeviceId);
        // 		printf("num:%d\r\n",u16LenDeviceId);
        // 		printf("rec: %s \r\n",(char *)pu8DeviceId);
        // 		vTaskDelay(1000 / portTICK_RATE_MS);

        // 	}
        // }

        /*============= End of read ble id ===============*/

        /* =======================Unit Test =====================*/
        allDriverInstall();

        xTaskCreate(sensor_data_update, "sensor_test", 2048, NULL, 5, NULL);

        xTaskCreate(ble_data_update, "ble_test", 2048, NULL, 3, NULL);


        xTaskCreate((TaskFunction_t)mainTaskAuthenticate, /* Pointer to the function that implements the task. */
                    "defaultTask",                        /* Text name for the task.  This is to facilitate debugging only. */
                    1024 * 16,                            /* Stack depth in words. */
                    NULL,                                 /* We are not using the task parameter. */
                    configMAX_PRIORITIES - 4,             /* This task will run at priority 1. */
                    NULL);

        // xTaskCreate(lcd_data_update,"lcd_test",2048,NULL,3,NULL);
        // // // xTaskCreate(lcd_show_main,"lcd_main_test",2048,NULL,1,NULL);
        // // // xTaskCreate(lcd_show_sensor,"lcd_sensor_test",2048,NULL,2,NULL);
        /*=====================End of Unit Test =====================*/

        /* =====================test C-R and R json response =====================*/
        // registerAndAuth_test();
        /* =====================End of test C-R and R json response =====================*/

        /* =====================test C-R and R json response using taskNotify===================== */

        // xTaskCreate(	(TaskFunction_t)mainTaskAuthenticate,		/* Pointer to the function that implements the task. */
        // 				"mainTaskAuthenticate",	/* Text name for the task.  This is to facilitate debugging only. */
        // 				1024*16,		/* Stack depth in words. */
        // 				NULL,		/* We are not using the task parameter. */
        // 				tskIDLE_PRIORITY,			/* This task will run at priority 1. */
        // 				&TH_mainTaskAuthenticate );
        /* =====================End of test C-R and R json response using taskNotify =====================*/

        /* test fpga Puf*/
        // I2c_Master_Init();
        // MAX77752_Init();

        // while (1)
        // {
        //         // MAX77752_printAllVolt();
        //         rstFpga();
        //         fpga_test();
        //         vTaskDelay(1000 / portTICK_RATE_MS);
        // }

        /* End of test fpga Puf */

        /*+++++++++++++++test lblock key stability++++++++++++++*/
        // TestLblockKeyStability();
        // PrintStableLblockKey();
        /*---------------test lblock key stability--------------*/

        /* test fpga led blink */
        // I2c_Master_Init();
        // MAX77752_Init();
        // gpio_set_direction(GPIO_NUM_21,GPIO_MODE_OUTPUT);
        // gpio_set_direction(GPIO_NUM_22,GPIO_MODE_INPUT);
        // gpio_set_direction(GPIO_NUM_23,GPIO_MODE_INPUT);
        // rstFpga();

        //GPIO get
        // printf("gpio21:%d\r\n",gpio_get_level(GPIO_NUM_21));
        // while(1)
        // {
        // 	printf("gpio22:%d\r\n",gpio_get_level(GPIO_NUM_22));
        // 	printf("gpio23:%d\r\n",gpio_get_level(GPIO_NUM_23));
        // 	vTaskDelay(1000 / portTICK_RATE_MS);
        // }

        /* End of test fpga led blink */

        /* keep sending C-R json */
        // blue_init();
        // while (1)
        // {
        // 	makeJson_test_CR();
        // 	vTaskDelay( 1000 / portTICK_RATE_MS) ;
        // }
        /* End of keep sending C-R json */

        // /* test freertos */
        // xTaskCreate(	(TaskFunction_t)mainTaskAuthenticate,		/* Pointer to the function that implements the task. */
        // 				"defaultTask",	/* Text name for the task.  This is to facilitate debugging only. */
        // 				1024*16,		/* Stack depth in words. */
        // 				NULL,		/* We are not using the task parameter. */
        // 				configMAX_PRIORITIES - 4,			/* This task will run at priority 1. */
        // 				NULL );

        /* End of test freertos */
}

void allDriverInstall(void)
{
        I2c_Master_Init();
        max30102_init();
        blue_init();
        fpgaDriverInstall();
        szm301DriverInstall();
        BMI160_init();

        TryToGetLblockKey();

        // MAX77752_Init();
        // lcd_init();
}
