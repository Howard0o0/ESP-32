/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "sdkconfig.h"

#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"


#include "ic_driver/max30102.c"
#include "ic_driver/max30102.h"
#include "util/algorithm.h"
#include "util/algorithm.c"




void app_main()
{
	xTaskCreate(MAX30102_test,"max30102_test_task",2048,NULL,1,NULL);
//	xTaskCreate(test,"test_task",2048,NULL,1,NULL);
//	test();
}
