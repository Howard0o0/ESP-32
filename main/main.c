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



void app_main()
{

	/* test C-R and R json response */
	// registerAndAuth_test();
	/* End of test C-R and R json response */

	/* test C-R and R json response using taskNotify */
	
	// xTaskCreate(	(TaskFunction_t)mainTaskAuthenticate,		/* Pointer to the function that implements the task. */
	// 				"mainTaskAuthenticate",	/* Text name for the task.  This is to facilitate debugging only. */
	// 				1024*16,		/* Stack depth in words. */
	// 				NULL,		/* We are not using the task parameter. */
	// 				tskIDLE_PRIORITY,			/* This task will run at priority 1. */
	// 				&TH_mainTaskAuthenticate );		
	/* End of test C-R and R json response using taskNotify */
	
	/* test fpga Puf*/
	// I2c_Master_Init();
	// MAX77752_Init();


	// while (1)
	// {
	// 	MAX77752_printAllVolt();
	// 	rstFpga();
	// 	fpga_test();
	// 	vTaskDelay(1000 / portTICK_RATE_MS);
	// }
	
	/* End of test fpga Puf */

	/* test fpga led blink */

	// gpio_set_direction(GPIO_NUM_21,GPIO_MODE_INPUT);
	// gpio_set_direction(GPIO_NUM_22,GPIO_MODE_INPUT);
	// gpio_set_direction(GPIO_NUM_23,GPIO_MODE_INPUT);

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
	xTaskCreate(	(TaskFunction_t)mainTaskAuthenticate,		/* Pointer to the function that implements the task. */
					"defaultTask",	/* Text name for the task.  This is to facilitate debugging only. */
					1024*16,		/* Stack depth in words. */
					NULL,		/* We are not using the task parameter. */
					configMAX_PRIORITIES - 4,			/* This task will run at priority 1. */
					NULL );	
	/* End of test freertos */
	

}



