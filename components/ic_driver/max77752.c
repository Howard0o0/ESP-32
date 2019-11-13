/*
 * max77752.c
 *
 *  Created on: 2019��5��10��
 *      Author: Administrator
 */


#include "iic.h"
#include "uart.h"
#include "driver/i2c.h"
#include "max77752.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


uint8_t BUCK1_Voltage = 0xC0;
uint8_t BUCK2_Voltage = 0x90;
uint8_t BUCK3_Voltage = 0x4A;
uint8_t LDO_Voltage = 0x08;



void MAX77752_write_ctrl1(){
	uint8_t temp = 0x10;
	I2c_Master_Write_Slave(MAX77752_ADDR,MAX77752_I2C_CTRL1,&temp,1);
}

esp_err_t MAX77752_WirteRegister(uint8_t reg_addr, uint8_t* Cmd, size_t size)
{
    int i2c_master_port = I2C_NUM_0;
    uint8_t i2c_write_cccess = 0x00;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MAX77752_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, MAX77752_I2C_CTRL2, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, i2c_write_cccess, ACK_CHECK_EN);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MAX77752_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, reg_addr, ACK_CHECK_EN);
    if (size > 1)
    	i2c_master_write(cmd, Cmd, size, ACK_CHECK_EN);
    else
    	i2c_master_write_byte(cmd, *Cmd, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_master_port, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}



void MAX77752_SetBUCKVoltage(uint8_t BUCKX, float voltage)
{
	uint8_t reg_data = 0;

	switch(BUCKX)
	{
	case MAX77752_BUCK1:
		reg_data = (uint8_t)((voltage - 0.6) / 0.00625);
		MAX77752_WirteRegister(MAX77752_BUCK1CNFG1, &reg_data, 1);
		break;
	case MAX77752_BUCK2:
		reg_data = (uint8_t)((voltage - 0.6) / 0.00625);
		MAX77752_WirteRegister(MAX77752_BUCK2CNFG1, &reg_data, 1);
		break;
	case MAX77752_BUCK3:
		reg_data = (uint8_t)((voltage - 0.26) / 0.01);
		MAX77752_WirteRegister(MAX77752_BUCK3CNFG1, &reg_data, 1);
//		MAX77752_WirteRegister(MAX77752_BUCK3CNFG5, &reg_data, 1);
		break;
	case MAX77752_LDO:
		reg_data = (uint8_t)((voltage - 0.8) / 0.025);
		MAX77752_WirteRegister(MAX77752_LDO_CNFG1, &reg_data, 1);
		break;
	case MAX77752_LOAD_SW:
		reg_data = 0x05;
		MAX77752_WirteRegister(MAX77752_CNFG_LSW1, &reg_data, 1);
		break;
	default:
		break;
	}
}

void MAX77752_ReadBUCKVoltage(uint8_t BUCKX)
{
	uint8_t reg_data = 0;

	switch(BUCKX)
	{
	case MAX77752_BUCK1:
		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_BUCK1CNFG1, &reg_data, 1);
		printf("\r\nBUCK 1 Voltage is %X = %fV\r\n",reg_data,((float)reg_data) * 0.00625 + 0.6);
		break;
	case MAX77752_BUCK2:
		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_BUCK2CNFG1, &reg_data, 1);
		printf("\r\nBUCK 2 Voltage is %X = %fV\r\n",reg_data,((float)reg_data) * 0.00625 + 0.6);
		break;
	case MAX77752_BUCK3:
		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_BUCK3CNFG1, &reg_data, 1);
		printf("\r\nBUCK 3 Voltage is %X = %fV\r\n",reg_data,((float)reg_data) * 0.01 + 0.26);
//		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_BUCK3CNFG5, &reg_data, 1);
//		printf("\r\nBUCK 3 Voltage is %X = %fV\r\n",reg_data,(float)reg_data * 0.01 + 0.26);
		break;
	case MAX77752_LDO:
		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_LDO_CNFG1, &reg_data, 1);
		printf("\r\nLDO Voltage is %X = %fV\r\n",reg_data,((float)reg_data) * 0.025 + 0.8);
		break;
	case MAX77752_LOAD_SW:
		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_CNFG_LSW1, &reg_data, 1);
		printf("\r\nLSW1 is %X \r\n",reg_data);
		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_BUCK1CNFG1, &reg_data, 1);
		printf("\r\nBUCK 1 Voltage is %X = %fV\r\n",reg_data,((float)reg_data) * 0.00625 + 0.6);
		break;
	default:
		break;
	}

}

/////////////////////////////////////


void set()
{
	uint8_t reg_data = 0;

	reg_data=0x3f;	
	MAX77752_WirteRegister(MAX77752_BUCK_INTM, &reg_data, 1);
	I2c_Master_Read_Slave(MAX77752_ADDR,MAX77752_BUCK_INTM,&reg_data,1);	
	printf("MAX77752_BUCK_INTM set:%#x\r\n",reg_data);

	reg_data = 0x3f;  //primitive: 0x15
	MAX77752_WirteRegister(MAX77752_OPMD1, &reg_data, 1);
	I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_OPMD1, &reg_data, 1);
	printf("OPMD1 set:%#x\r\n",reg_data);

	reg_data = 0x7;  
	MAX77752_WirteRegister(MAX77752_OPMD2, &reg_data, 1);
	I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_OPMD2, &reg_data, 1);
	printf("OPMD2 set:%#x\r\n",reg_data);

	MAX77752_SetBUCKVoltage(MAX77752_BUCK3, 1.0);	//reset 50
	vTaskDelay(500 / portTICK_RATE_MS);
	MAX77752_SetBUCKVoltage(MAX77752_LDO, 1.0);		//reset A8
	vTaskDelay(500 / portTICK_RATE_MS);
	MAX77752_SetBUCKVoltage(MAX77752_BUCK1, 1.8);	//reset C0
	vTaskDelay(500 / portTICK_RATE_MS);
	MAX77752_SetBUCKVoltage(MAX77752_LOAD_SW, 1.8);		//reset A8
	vTaskDelay(500 / portTICK_RATE_MS);
	MAX77752_SetBUCKVoltage(MAX77752_BUCK2, 1.5);	//reset 78
	
	
	
}

void Set()
{
	uint8_t reg_data = 0;
	int flag = 0;
	while(1)
	{
		I2c_Master_Read_Slave(MAX77752_ADDR,MAX77752_BUCK_INTM,&reg_data,1);
		if(reg_data!=0x3f)
			flag++;

		I2c_Master_Read_Slave(MAX77752_ADDR,MAX77752_OPMD1,&reg_data,1);
		if(reg_data!=0x3f)
			flag++;
		
		I2c_Master_Read_Slave(MAX77752_ADDR,MAX77752_OPMD2,&reg_data,1);
		if(reg_data!=0x7)
			flag++;

		// I2c_Master_Read_Slave(MAX77752_ADDR,MAX77752_BUCK1CNFG1,&reg_data,1);
		// if(reg_data!=0xBF)
		// 	flag++;

		// I2c_Master_Read_Slave(MAX77752_ADDR,MAX77752_BUCK2CNFG1,&reg_data,1);
		// if(reg_data!=0x90)
		// 	flag++;

		// I2c_Master_Read_Slave(MAX77752_ADDR,MAX77752_BUCK3CNFG1,&reg_data,1);
		// // if(reg_data!=0x4A)
		// if(reg_data!=0x45)
		// 	flag++;

		// I2c_Master_Read_Slave(MAX77752_ADDR,MAX77752_LDO_CNFG1,&reg_data,1);
		// if(reg_data!=0x7)
		// 	flag++;

		// I2c_Master_Read_Slave(MAX77752_ADDR,MAX77752_CNFG_LSW1,&reg_data,1);
		// if(reg_data!=0x5)
		// 	flag++;

		if(flag!=0)
		{
			I2c_Master_Read_Slave(MAX77752_ADDR,MAX77752_GLBL_INT0,&reg_data,1);
		printf("MAX77752_GLBL_INT0:%#x\r\n",reg_data);

		I2c_Master_Read_Slave(MAX77752_ADDR,MAX77752_GLBL_INT1,&reg_data,1);
		printf("MAX77752_GLBL_INT1:%#x\r\n",reg_data);

		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_BUCK_INT, &reg_data, 1);
		printf("MAX77752_BUCK_INT:%#x\r\n",reg_data);

		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_LSW_INT, &reg_data, 1);
		printf("MAX77752_LSW_INT:%#x\r\n",reg_data);

		I2c_Master_Read_Slave(MAX77752_ADDR,MAX77752_BUCK_INTM,&reg_data,1);	
		printf("MAX77752_BUCK_INTM:%#x\r\n",reg_data);

		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_OPMD1, &reg_data, 1);
		printf("OPMD1:%#x\r\n",reg_data);

		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_OPMD2, &reg_data, 1);
		printf("OPMD2:%#x\r\n",reg_data);

		MAX77752_ReadBUCKVoltage(MAX77752_BUCK1);
		MAX77752_ReadBUCKVoltage(MAX77752_BUCK2);
		MAX77752_ReadBUCKVoltage(MAX77752_BUCK3);
		MAX77752_ReadBUCKVoltage(MAX77752_LDO);
		MAX77752_ReadBUCKVoltage(MAX77752_LOAD_SW);

			set();
			flag=0;
		}
		else
		{
			break;
		}
		
		
	}

	printf("set finish!!!\r\n");
}

void MAX77752_Init(void)
{
	
	Set();

	int time=0;
	while (1)
	{
		MAX77752_ReadBUCKVoltage(MAX77752_BUCK1);
		MAX77752_ReadBUCKVoltage(MAX77752_BUCK2);
		MAX77752_ReadBUCKVoltage(MAX77752_BUCK3);
		MAX77752_ReadBUCKVoltage(MAX77752_LDO);
		MAX77752_ReadBUCKVoltage(MAX77752_LOAD_SW);

		//GPIO get
		printf("gpio21:%d\r\n",gpio_get_level(GPIO_NUM_21));
		printf("gpio22:%d\r\n",gpio_get_level(GPIO_NUM_22));
		printf("gpio23:%d\r\n",gpio_get_level(GPIO_NUM_23));


		uint8_t reg_data = 0;

		I2c_Master_Read_Slave(MAX77752_ADDR,MAX77752_GLBL_INT0,&reg_data,1);
		printf("MAX77752_GLBL_INT0:%#x\r\n",reg_data);

		I2c_Master_Read_Slave(MAX77752_ADDR,MAX77752_GLBL_INT1,&reg_data,1);
		printf("MAX77752_GLBL_INT1:%#x\r\n",reg_data);

		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_BUCK_INT, &reg_data, 1);
		printf("MAX77752_BUCK_INT:%#x\r\n",reg_data);

		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_LSW_INT, &reg_data, 1);
		printf("MAX77752_LSW_INT:%#x\r\n",reg_data);

		I2c_Master_Read_Slave(MAX77752_ADDR,MAX77752_BUCK_INTM,&reg_data,1);	
		printf("MAX77752_BUCK_INTM:%#x\r\n",reg_data);

		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_OPMD1, &reg_data, 1);
		printf("OPMD1:%#x\r\n",reg_data);

		I2c_Master_Read_Slave(MAX77752_ADDR, MAX77752_OPMD2, &reg_data, 1);
		printf("OPMD2:%#x\r\n",reg_data);

		printf("read finish!!!\r\n");
		
		vTaskDelay(1000 / 10);

		// if(time%5==0)
		// 	set();
		// time++;

	}
	
	
}
