/*
 * author : 朕
 * NOTE : Remember To IIC_MASTER_INIT first!
 *
 * use instruction: (1)I2c_Master_Init() -> (2)max30102_init() -> (3)waiting_finger() -> (4)read_spo2_and_ir()
 *
 */
#ifndef MAIN_IC_DRIVER_MAX30102_H_
#define MAIN_IC_DRIVER_MAX30102_H_

#include <stdint.h>

//register addresses
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D
#define REG_PILOT_PA 0x10  //
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 0x1F
#define REG_TEMP_FRAC 0x20
#define REG_TEMP_CONFIG 0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF

#define TRUE 1
#define FALSE 0

typedef struct hrandir{
	int32_t spo2;
	int32_t hr;
}spo2Andhr;

uint32_t aun_ir_buffer[500]; //IR LED sensor data
uint32_t aun_red_buffer[500];    //Red LED sensor data
int32_t n_ir_buffer_length;    //data length,if want to change,at the top of  max30102_init()
int32_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int32_t n_heart_rate;   //heart rate value
int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid

#define MAX_BRIGHTNESS 255

/*################  outside interface   ################################ */
void max30102_init();

/*
 * func： 一直阻塞直到有手指放上采样器
 */
void waiting_finger();

/*
 * func: 读取[血氧]以及[心率]
 * return: [success]->spo2Andhr类型的结构体，包含血氧和心率两个成员，可以跳转查看结构体定义
 *         [fail]->NULL (采集过程中手指离开)
 * NOTE: 每次读取需要5s
 */
spo2Andhr* read_spo2_and_ir();

/*
 * func : 可单独调用，此接口会包含以上三个结构进行测试，通过串口打印信息
 */
void MAX30102_test();
/* ---------------------------------------------------------------- */


/*  inside interface--------------*/
static void max30102_INT_GPIO_init();
static void max30102_read_reg_FIFO(uint8_t *red_1,uint8_t *red_2,uint8_t *red_3,uint8_t *ir_1,uint8_t *ir_2,uint8_t *ir_3);
static void clear_FIFO();
static int get_led_data_into_buff();
static void max30102_read_redLed_and_irLed(uint32_t *pun_red_led, uint32_t *pun_ir_led);
static int max30102_INT_Capture();
static void calc_red_ir_led(uint8_t red_1,uint8_t red_2,uint8_t red_3,uint8_t ir_1,uint8_t ir_2,uint8_t ir_3,uint32_t *red_led,uint32_t *ir_led);
static void max3010_clear_status_reg();
static int finger_touch();
static void fresh_first_100_samples();
/* ------------------------------ */

#endif /* MAIN_IC_DRIVER_MAX30102_H_ */

/*
 * 	untouched:
 * 	red:1265 ,ir:1095
	red:1269 ,ir:1100
	red:1275 ,ir:1090
	red:1269 ,ir:1090
	red:1276 ,ir:1096

	touched:
	red:112423 ,ir:127871
	red:112453 ,ir:127958
	red:112476 ,ir:128031
	red:112516 ,ir:128100
	red:112547 ,ir:128168
 *
 */
