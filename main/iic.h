/*
 * iic.h
 *
 *  Created on: 2019��5��11��
 *      Author: Administrator
 */

#ifndef MAIN_EMD_PROCTOL_IIC_H_
#define MAIN_EMD_PROCTOL_IIC_H_

#include <stdio.h>
#include "esp_err.h"
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"

void iic_test();

esp_err_t I2c_Master_Init(void);
esp_err_t I2c_Master_Read_Slave(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data_rd, size_t size);

esp_err_t I2c_Master_Write_Slave(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data_wr, size_t size);

//0-success 1-fail
int8_t BMI160_I2c_Master_Read_Slave(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data_rd, uint16_t size);
//0-success 1-fail
int8_t BMI160_I2c_Master_Write_Slave(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data_wr, uint16_t size);


/***************I2C GPIO******************/
#define I2C_Pin_SCL		GPIO_NUM_27
#define I2C_Pin_SDA		GPIO_NUM_13
/*********************************************/


#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)

#define DATA_LENGTH 512                  		/*!< Data buffer length of test buffer */
#define RW_TEST_LENGTH 128               		/*!< Data length for r/w test, [0,DATA_LENGTH] */
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 		/*!< delay time between different test items */

#define I2C_SLAVE_SCL_IO GPIO_NUM_18            /*!< gpio number for i2c slave clock */
#define I2C_SLAVE_SDA_IO GPIO_NUM_19            /*!< gpio number for i2c slave data */
#define I2C_SLAVE_NUM I2C_NUMBER(1) 			/*!< I2C port number for slave dev */
#define I2C_SLAVE_TX_BUF_LEN (2 * DATA_LENGTH)  /*!< I2C slave tx buffer size */
#define I2C_SLAVE_RX_BUF_LEN (2 * DATA_LENGTH)  /*!< I2C slave rx buffer size */

#define I2C_MASTER_SCL_IO GPIO_NUM_27			/*!< gpio number for I2C master clock */                //id: 0d 0f
#define I2C_MASTER_SDA_IO GPIO_NUM_13           /*!< gpio number for I2C master data  */
// #define I2C_MASTER_SCL_IO GPIO_NUM_18                                                                        //id : 0B
// #define I2C_MASTER_SDA_IO GPIO_NUM_19
#define I2C_MASTER_NUM I2C_NUMBER(0) 			/*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000        		/*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0             /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0             /*!< I2C master doesn't need buffer */

#define MAX17048_SENSOR_ADDR 0x68   			/*!< slave address for MAX17048 sensor */
#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1                            /*!< I2C nack value */


#define SCL_H         gpio_set_level(I2C_Pin_SCL, 1)
#define SCL_L         gpio_set_level(I2C_Pin_SCL, 0)
#define SDA_H         gpio_set_level(I2C_Pin_SDA, 1)
#define SDA_L         gpio_set_level(I2C_Pin_SDA, 0)

#define SDA_read      gpio_get_level(I2C_Pin_SDA)

extern volatile uint8_t I2C_FastMode;

void I2c_Soft_Init(void);
void I2c_Soft_SendByte(uint8_t SendByte);
uint8_t I2c_Soft_ReadByte(uint8_t);

//int I2c_Soft_Single_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
//int I2c_Soft_Single_Read(uint8_t SlaveAddress,uint8_t REG_Address);
//int I2c_Soft_Mult_Read(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t * ptChar,uint8_t size);

uint8_t IIC_Write_1Byte(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
uint8_t IIC_Read_1Byte(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t *REG_data);
uint8_t IIC_Write_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t len, uint8_t *buf);
uint8_t IIC_Read_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t len, uint8_t *buf);



#endif /* MAIN_EMD_PROCTOL_IIC_H_ */

