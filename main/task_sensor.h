#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "sys/unistd.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "sdkconfig.h"

#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

#include "iic.h"
#include "uart.h"

#include "ls013b7dh03.h"
#include "gatts_table.h"
#include "bmi160.h"
#include "max30102.h"
#include "max30205.h"

void sensor_data_update();
void ble_data_update();
void lcd_data_update();
void lcd_show_main();
void lcd_show_sensor();