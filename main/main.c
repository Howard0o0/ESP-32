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

#include "emd_proctol/iic.h"
#include "emd_proctol/iic.c"
#include "emd_proctol/uart.h"
#include "emd_proctol/uart.c"


#include "ic_driver/max17048.h"
#include "ic_driver/max17048.c"
#include "ic_driver/max30205.h"
#include "ic_driver/max30205.c"
#include "ic_driver/max77752.h"
#include "ic_driver/max77752.c"
#include "ic_driver/rgb_led.h"
#include "ic_driver/rgb_led.c"
#include "ic_driver/max30102.c"

#include "util/algorithm.h"

#define MAX_BRIGHTNESS 255

void MAX30102_test(void *parameter){
	uint8_t reg_data;
	I2c_Master_Init();
//	MAX77752_Init();
//	RGB_LedInit();

	uint8_t u8_temp;
	uint32_t red_led;
	uint32_t ir_led;

	uint8_t temp_int;
	uint8_t temp_frac;
	float temp_value;

	//MAX30102 RESET
	u8_temp = 0x40;
	I2c_Master_Write_Slave(0x57,REG_MODE_CONFIG,&u8_temp,1);
	//read and clear status register
	I2c_Master_Read_Slave(0x57,REG_INTR_STATUS_1,&reg_data,1);
	//MAX30102 INIT
	max30102_INT_GPIO_init();

	//max30102 init
	u8_temp = 0xc0;
	I2c_Master_Write_Slave(0x57,REG_INTR_ENABLE_1,&u8_temp,1);
	u8_temp = 0x00;
	I2c_Master_Write_Slave(0x57,REG_INTR_ENABLE_2,&u8_temp,1);
	u8_temp = 0x00;
	I2c_Master_Write_Slave(0x57,REG_FIFO_WR_PTR,&u8_temp,1);
	u8_temp = 0x00;
	I2c_Master_Write_Slave(0x57,REG_OVF_COUNTER,&u8_temp,1);
	u8_temp = 0x00;
	I2c_Master_Write_Slave(0x57,REG_FIFO_RD_PTR,&u8_temp,1);
	u8_temp = 0x0f;
	I2c_Master_Write_Slave(0x57,REG_FIFO_CONFIG,&u8_temp,1);
	u8_temp = 0x03;
	I2c_Master_Write_Slave(0x57,REG_MODE_CONFIG,&u8_temp,1);
	u8_temp = 0x27;
	I2c_Master_Write_Slave(0x57,REG_SPO2_CONFIG,&u8_temp,1);
	u8_temp = 0x24;
	I2c_Master_Write_Slave(0x57,REG_LED1_PA,&u8_temp,1);
	u8_temp = 0x24;
	I2c_Master_Write_Slave(0x57,REG_LED2_PA,&u8_temp,1);
	u8_temp = 0x7f;
	I2c_Master_Write_Slave(0x57,REG_PILOT_PA,&u8_temp,1);

	//clear temperature value reg
//	I2c_Master_Read_Slave(0x57,REG_TEMP_INTR,&reg_data,1);
//	I2c_Master_Read_Slave(0x57,REG_TEMP_FRAC,&reg_data,1);


	int i = 0;
	while(1)
	{
		u8_temp = 0x01;
//		I2c_Master_Write_Slave(0x57,REG_TEMP_CONFIG,&u8_temp,1);
		//wait until max30102_INT comes
		while(max30102_INT_Capture() == FALSE);
		i++;
		uint8_t u8_tmp;
//		I2c_Master_Read_Slave(0x57,REG_INTR_STATUS_1,&u8_tmp,1);
//		printf("sPO2_INT_STATUS:%i\r\n",u8_tmp);

		printf("start sampling! %d \r\n",i++);

//		I2c_Master_Read_Slave(0x57,REG_TEMP_INTR,&temp_int,1);
//		I2c_Master_Read_Slave(0x57,REG_TEMP_FRAC,&temp_frac,1);
//		temp_value = temp_int+ temp_frac*0.0625;
//		printf("temperatrue is %6.4f \r\n",temp_value);

		max30102_read_redLed_and_irLed(&red_led,&ir_led);
		printf("red_led:%i,ir_led:%i\r\n",red_led,ir_led);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}

//	while(1)
//	{
//		printf("\r\nTemperature is %f℃\r\n",MAX30205_ReadTemperature());
//		vTaskDelay(1000 / portTICK_RATE_MS);
//		RGB_ColorTest();
////		MAX77752_Init();
//		MAX77752_ReadBUCKVoltage(MAX77752_BUCK1);
//		MAX77752_ReadBUCKVoltage(MAX77752_BUCK2);
//		MAX77752_ReadBUCKVoltage(MAX77752_BUCK3);
//		MAX77752_ReadBUCKVoltage(MAX77752_LDO);
//		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_OPMD1, &reg_data, 1);
//		disp_buf(&reg_data, 1);
//	}
}

void MAX30102_test_2(void *parameter){

	/*max30102_init_process */
	uint8_t reg_data = 0x00;
	I2c_Master_Init();
//	MAX77752_Init();
//	RGB_LedInit();

	uint8_t u8_temp;
	uint32_t red_led;
	uint32_t ir_led;

	uint8_t temp_int;
	uint8_t temp_frac;
	float temp_value;

	//MAX30102 RESET
	u8_temp = 0x40;
	I2c_Master_Write_Slave(0x57,0x09,&u8_temp,1);
	//read and clear status register
	I2c_Master_Read_Slave(0x57,0x00,&reg_data,1);
	//MAX30102 INIT
	max30102_INT_GPIO_init();

	//max30102 init
	u8_temp = 0xc0;
	I2c_Master_Write_Slave(0x57,REG_INTR_ENABLE_1,&u8_temp,1);
	u8_temp = 0x00;
	I2c_Master_Write_Slave(0x57,REG_INTR_ENABLE_2,&u8_temp,1);
	u8_temp = 0x00;
	I2c_Master_Write_Slave(0x57,REG_FIFO_WR_PTR,&u8_temp,1);
	u8_temp = 0x00;
	I2c_Master_Write_Slave(0x57,REG_OVF_COUNTER,&u8_temp,1);
	u8_temp = 0x00;
	I2c_Master_Write_Slave(0x57,REG_FIFO_RD_PTR,&u8_temp,1);
	u8_temp = 0x0f;
	I2c_Master_Write_Slave(0x57,REG_FIFO_CONFIG,&u8_temp,1);
	u8_temp = 0x03;
	I2c_Master_Write_Slave(0x57,REG_MODE_CONFIG,&u8_temp,1);
	u8_temp = 0x27;
	I2c_Master_Write_Slave(0x57,REG_SPO2_CONFIG,&u8_temp,1);
	u8_temp = 0x24;
	I2c_Master_Write_Slave(0x57,REG_LED1_PA,&u8_temp,1);
	u8_temp = 0x24;
	I2c_Master_Write_Slave(0x57,REG_LED2_PA,&u8_temp,1);
	u8_temp = 0x7f;
	I2c_Master_Write_Slave(0x57,REG_PILOT_PA,&u8_temp,1);
	/****************************************/

	/* spo2 and heart_rate sample */
	uint32_t un_min, un_max, un_prev_data;  //variables to calculate the on-board LED brightness that reflects the heartbeats
	int i;
	int32_t n_brightness;
	float f_temp;
	uint32_t aun_ir_buffer[500]; //IR LED sensor data
	int32_t n_ir_buffer_length;    //data length
	uint32_t aun_red_buffer[500];    //Red LED sensor data
	int32_t n_sp02; //SPO2 value
	int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
	int32_t n_heart_rate;   //heart rate value
	int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid
	uint8_t uch_dummy;

	n_brightness=0;
	un_min=0x3FFFF;
	un_max=0;

	n_ir_buffer_length=500; //buffer length of 100 stores 5 seconds of samples running at 100sps

	printf("采集500个样本\r\n");
	//read the first 500 samples, and determine the signal range
	for(i=0;i<n_ir_buffer_length;i++)
	{
		while(max30102_INT_Capture() == FALSE);   //等待MAX30102中断引脚拉低
		printf("starting sample\r\n");

		max30102_read_redLed_and_irLed( &(aun_red_buffer[i]), &(aun_ir_buffer[i]));  //read from MAX30102 FIFO

		if(un_min>aun_red_buffer[i])
			un_min=aun_red_buffer[i];    //update signal min
		if(un_max<aun_red_buffer[i])
			un_max=aun_red_buffer[i];    //update signal max

		printf("red=");
		printf("%i", aun_red_buffer[i]);
		printf(", ir=");
		printf("%i\r\n", aun_ir_buffer[i]);

	}
	un_prev_data=aun_red_buffer[i];


	//calculate heart rate and SpO2 after first 500 samples (first 5 seconds of samples)
	maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);

	//Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
	while(1)
	{
		i=0;
		un_min=0x3FFFF;
		un_max=0;

		//dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
		for(i=100;i<500;i++)
		{
			aun_red_buffer[i-100]=aun_red_buffer[i];
			aun_ir_buffer[i-100]=aun_ir_buffer[i];

			//update the signal min and max
			if(un_min>aun_red_buffer[i])
			un_min=aun_red_buffer[i];
			if(un_max<aun_red_buffer[i])
			un_max=aun_red_buffer[i];
		}

		//take 100 sets of samples before calculating the heart rate.
		for(i=400;i<500;i++)
		{
			un_prev_data=aun_red_buffer[i-1];
			while(max30102_INT_Capture() == FALSE);   //等待MAX30102中断引脚拉低

			max30102_read_redLed_and_irLed((aun_red_buffer+i), (aun_ir_buffer+i));

			if(aun_red_buffer[i]>un_prev_data)//just to determine the brightness of LED according to the deviation of adjacent two AD data
			{
				f_temp=aun_red_buffer[i]-un_prev_data;
				f_temp/=(un_max-un_min);
				f_temp*=MAX_BRIGHTNESS;
				n_brightness-=(int)f_temp;
				if(n_brightness<0)
					n_brightness=0;
			}
			else
			{
				f_temp=un_prev_data-aun_red_buffer[i];
				f_temp/=(un_max-un_min);
				f_temp*=MAX_BRIGHTNESS;
				n_brightness+=(int)f_temp;
				if(n_brightness>MAX_BRIGHTNESS)
					n_brightness=MAX_BRIGHTNESS;
			}


			//send samples and calculation result to terminal program through UART
			printf("red=");
			printf("%i", aun_red_buffer[i]);
			printf(", ir=");
			printf("%i", aun_ir_buffer[i]);
			printf(", HR=%i, ", n_heart_rate);
			printf("HRvalid=%i, ", ch_hr_valid);
			printf("SpO2=%i, ", n_sp02);
			printf("SPO2Valid=%i\r\n", ch_spo2_valid);
		}
		maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
	}
	/********************************/


}

void app_main()
{
	xTaskCreate(MAX30102_test,"max30102_test_task",2048,NULL,1,NULL);
}
