/*
 * max30102.c
 *
 *  Created on: 2019Äê5ÔÂ10ÈÕ
 *      Author: Administrator
 */

#include "max30102.h"
#include "driver/gpio.h"
#include "../emd_proctol/iic.h"

void max30102_INT_GPIO_init(){

	gpio_config_t io_conf;
	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pin_bit_mask = 1ULL<<GPIO_NUM_15; // set io_15 as interrupt receiver
	io_conf.pull_up_en = GPIO_PULLUP_DISABLE ;
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE ;
	gpio_config(&io_conf);
}

esp_err_t max30102_read_redLed_and_irLed(uint32_t *pun_red_led, uint32_t *pun_ir_led){

	uint8_t u8_temp;
	int i2c_master_port = I2C_MASTER_NUM;
	uint8_t dev_addr = 0x57;
	char reg_addr = REG_FIFO_DATA;
	uint8_t red_led_1;
	uint8_t red_led_2;
	uint8_t red_led_3;
	uint8_t ir_led_1;
	uint8_t ir_led_2;
	uint8_t ir_led_3;

	*pun_red_led = 0;
	*pun_ir_led = 0;
	//clear status reg
	I2c_Master_Read_Slave(0x57,REG_INTR_STATUS_1,&u8_temp,1);
	I2c_Master_Read_Slave(0x57,REG_INTR_STATUS_2,&u8_temp,1);

	//read from FIFO
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev_addr << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, REG_FIFO_DATA, ACK_CHECK_EN);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev_addr << 1) | READ_BIT, ACK_CHECK_EN);

    i2c_master_read_byte(cmd, &red_led_1, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &red_led_2, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &red_led_3, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &ir_led_1, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &ir_led_2, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &ir_led_3, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_master_port, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    printf("red_led1:%#x ,red_led2:%#x ,red_led3:%#x\r\n",(uint32_t)red_led_1,(uint32_t)red_led_2,(uint32_t)red_led_3);
    printf("ir_led1:%#x ,ir_led2:%#x ,ir_led3:%#x\r\n",(uint32_t)ir_led_1,(uint32_t)ir_led_2,(uint32_t)ir_led_3);

    //calculate red_led && ir_led
//    red_led_1 <<= 16;
//    red_led_2 <<= 8;
//    ir_led_1 <<= 16;
//    ir_led_2 <<= 8;
    uint32_t u32_red_led1 = ((uint32_t)red_led_1) << 16;
    uint32_t u32_red_led2 = ((uint32_t)red_led_2) << 8;
    uint32_t u32_red_led3 = (uint32_t)red_led_3;
    uint32_t u32_ir_led1 = ((uint32_t)ir_led_1) << 16;
    uint32_t u32_ir_led2 = ((uint32_t)ir_led_2) << 8;
    uint32_t u32_ir_led3 = (uint32_t)ir_led_3;
    *pun_red_led = u32_red_led1 + u32_red_led2 + u32_red_led3;
    *pun_ir_led =  u32_ir_led1 + u32_ir_led2 + u32_ir_led3;
    *pun_red_led&=0x03FFFF;  //Mask MSB [23:18]
    *pun_ir_led&=0x03FFFF;  //Mask MSB [23:18]

    if(ret == ESP_FAIL) printf("read FIFO FAIL!\r\n");
    return ret;

}

esp_err_t max30102_read_redLed_and_irLed_2(uint32_t *pun_red_led, uint32_t *pun_ir_led){

	uint8_t u8_temp;
	int i2c_master_port = I2C_MASTER_NUM;
	uint8_t dev_addr = 0x57;
	char reg_addr = REG_FIFO_DATA;
	uint8_t red_led_1;
	uint8_t red_led_2;
	uint8_t red_led_3;
	uint8_t ir_led_1;
	uint8_t ir_led_2;
	uint8_t ir_led_3;
	esp_err_t ret;

	*pun_red_led = 0;
	*pun_ir_led = 0;
	//clear status reg
	I2c_Master_Read_Slave(0x57,REG_INTR_STATUS_1,&u8_temp,1);
	I2c_Master_Read_Slave(0x57,REG_INTR_STATUS_2,&u8_temp,1);

	//read from FIFO
	I2c_Master_Read_Slave(0x57,REG_FIFO_DATA,&red_led_1,1);
	I2c_Master_Read_Slave(0x57,REG_FIFO_DATA,&red_led_2,1);
	I2c_Master_Read_Slave(0x57,REG_FIFO_DATA,&red_led_3,1);
	I2c_Master_Read_Slave(0x57,REG_FIFO_DATA,&ir_led_1,1);
	I2c_Master_Read_Slave(0x57,REG_FIFO_DATA,&ir_led_2,1);
	ret = I2c_Master_Read_Slave(0x57,REG_FIFO_DATA,&ir_led_3,1);

    printf("led1:%#x ,led2:%#x ,led3:%#x\r\n",red_led_1,red_led_2,red_led_3);

    //calculate red_led && ir_led
    red_led_1 <<= 16;
    red_led_2 <<= 8;
    ir_led_1 <<= 16;
    ir_led_2 <<= 8;
    *pun_red_led = red_led_1 + red_led_2 + red_led_3;
    *pun_ir_led = ir_led_1 + ir_led_2 + ir_led_3;
    *pun_red_led&=0x03FFFF;  //Mask MSB [23:18]
    *pun_ir_led&=0x03FFFF;  //Mask MSB [23:18]

    if(ret == ESP_FAIL) printf("read FIFO FAIL!\r\n");
    return ret;

}

int max30102_INT_Capture(){
	int io_23_status = gpio_get_level(GPIO_NUM_15);

	//if io_23 is high, interrupt not comes yet,return false
	if (io_23_status)
		return FALSE;
	else
		return TRUE;
}
