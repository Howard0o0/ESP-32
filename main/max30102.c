/*
 * max30102.c
 *
 *  Created on: 2019??5??10??
 *      Author: Administrator
 */

#include "max30102.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "iic.h"
#include "algorithm.h"

/*##############outside interface#######################*/
void max30102_init()
{

        uint8_t reg_data;
        max30102_INT_GPIO_init();

        uint8_t u8_temp;

        n_ir_buffer_length = 500;

        //MAX30102 RESET
        u8_temp = 0x40;
        I2c_Master_Write_Slave(0x57, REG_MODE_CONFIG, &u8_temp, 1);
        //read and clear status register
        I2c_Master_Read_Slave(0x57, REG_INTR_STATUS_1, &reg_data, 1);

        //max30102 init
        u8_temp = 0xc0;
        I2c_Master_Write_Slave(0x57, REG_INTR_ENABLE_1, &u8_temp, 1);
        u8_temp = 0x00;
        I2c_Master_Write_Slave(0x57, REG_INTR_ENABLE_2, &u8_temp, 1);
        u8_temp = 0x00;
        I2c_Master_Write_Slave(0x57, REG_FIFO_WR_PTR, &u8_temp, 1);
        u8_temp = 0x00;
        I2c_Master_Write_Slave(0x57, REG_OVF_COUNTER, &u8_temp, 1);
        u8_temp = 0x00;
        I2c_Master_Write_Slave(0x57, REG_FIFO_RD_PTR, &u8_temp, 1);
        u8_temp = 0x0f;
        I2c_Master_Write_Slave(0x57, REG_FIFO_CONFIG, &u8_temp, 1);
        u8_temp = 0x03;
        I2c_Master_Write_Slave(0x57, REG_MODE_CONFIG, &u8_temp, 1);
        u8_temp = 0x27;
        I2c_Master_Write_Slave(0x57, REG_SPO2_CONFIG, &u8_temp, 1);
        u8_temp = 0x24;
        I2c_Master_Write_Slave(0x57, REG_LED1_PA, &u8_temp, 1);
        u8_temp = 0x24;
        I2c_Master_Write_Slave(0x57, REG_LED2_PA, &u8_temp, 1);
        u8_temp = 0x7f;
        I2c_Master_Write_Slave(0x57, REG_PILOT_PA, &u8_temp, 1);

        u8_temp = 0;
        I2c_Master_Write_Slave(0x57, REG_FIFO_WR_PTR, &u8_temp, 1);
        I2c_Master_Write_Slave(0x57, REG_OVF_COUNTER, &u8_temp, 1);
        I2c_Master_Write_Slave(0x57, REG_FIFO_RD_PTR, &u8_temp, 1);
}

void waiting_finger()
{
        while (finger_touch() == FALSE)
                ;
}

spo2Andhr *read_spo2_and_ir()
{

        //	fresh_first_100_samples();   //?????100??????  ,  ??????????????
        int result = get_led_data_into_buff();
        if (result == FALSE)
        {
                printf("get_led_data_into_buff failed!\r\n");
                return NULL;
        }
        else
        {
                maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
                spo2Andhr *spo2_ir = (spo2Andhr *)malloc(sizeof(spo2Andhr));
                spo2_ir->spo2 = ch_spo2_valid ? n_sp02 : 0;
                spo2_ir->hr = ch_hr_valid ? n_heart_rate : 0;
                if (ch_spo2_valid || ch_hr_valid)
                        return spo2_ir;
                else
                {
                        printf("[sample invalid] spo2_valid:%i, hr_valid:%i   ,  spo2:%i,  hr_%i \r\n", ch_spo2_valid, ch_hr_valid, n_sp02, n_heart_rate);
                        return NULL;
                }
        }
}

void MAX30102_test()
{
        I2c_Master_Init();
        max30102_init();
        printf("waiting for finger....\r\n");
        //collect samples and calc-------------------------------------------------------------
        while (1)
        {
                while (finger_touch() == FALSE)
                {
                };
                printf("collecting 500 samples,might consume 5s\r\n");
                printf("calculating heart_rate and spo2...\r\n");

                spo2Andhr *result = read_spo2_and_ir();
                if (result == NULL)
                {
                        printf("result invalid,waiting for finger....\r\n");
                }
                else
                        printf("HR: %i, SpO2: %i \r\n", result->hr, result->spo2);
        }
        //------------------------------------------------------------------------
}
/*#####################################################*/

static void max30102_INT_GPIO_init()
{

        gpio_config_t io_conf;
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pin_bit_mask = 1ULL << GPIO_NUM_15; // set io_15 as interrupt receiver
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        gpio_config(&io_conf);
}

static void max30102_read_reg_FIFO(uint8_t *red_1, uint8_t *red_2, uint8_t *red_3, uint8_t *ir_1, uint8_t *ir_2, uint8_t *ir_3)
{
        int i2c_master_port = I2C_MASTER_NUM;
        uint8_t dev_addr = 0x57;
        //read from FIFO
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (dev_addr << 1) | WRITE_BIT, ACK_CHECK_EN);
        i2c_master_write_byte(cmd, REG_FIFO_DATA, ACK_CHECK_EN);
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (dev_addr << 1) | READ_BIT, ACK_CHECK_EN);

        i2c_master_read_byte(cmd, red_1, I2C_MASTER_ACK);
        i2c_master_read_byte(cmd, red_2, I2C_MASTER_ACK);
        i2c_master_read_byte(cmd, red_3, I2C_MASTER_ACK);
        i2c_master_read_byte(cmd, ir_1, I2C_MASTER_ACK);
        i2c_master_read_byte(cmd, ir_2, I2C_MASTER_ACK);
        i2c_master_read_byte(cmd, ir_3, I2C_MASTER_NACK);
        i2c_master_stop(cmd);
        esp_err_t ret = i2c_master_cmd_begin(i2c_master_port, cmd, 1000 / 100);
        i2c_cmd_link_delete(cmd);

        if (ret == ESP_FAIL)
                printf("read FIFO FAIL!\r\n");
}

static void max3010_clear_status_reg()
{
        uint8_t u8_temp;
        //clear status reg
        I2c_Master_Read_Slave(0x57, REG_INTR_STATUS_1, &u8_temp, 1);
        I2c_Master_Read_Slave(0x57, REG_INTR_STATUS_2, &u8_temp, 1);
}

static void max30102_read_redLed_and_irLed(uint32_t *pun_red_led, uint32_t *pun_ir_led)
{

        uint8_t red_led_1;
        uint8_t red_led_2;
        uint8_t red_led_3;
        uint8_t ir_led_1;
        uint8_t ir_led_2;
        uint8_t ir_led_3;

        *pun_red_led = 0;
        *pun_ir_led = 0;

        while (max30102_INT_Capture() == FALSE)
                ; //???MAX30102?��?????????
        max3010_clear_status_reg();
        max30102_read_reg_FIFO(&red_led_1, &red_led_2, &red_led_3, &ir_led_1, &ir_led_2, &ir_led_3);
        calc_red_ir_led(red_led_1, red_led_2, red_led_3, ir_led_1, ir_led_2, ir_led_3, pun_red_led, pun_ir_led);

        // output FIFO reg info
        //    printf("red1:%#x,red2:%#x,red3:%#x \r\n",red_led_1,red_led_2,red_led_3);
        //    printf("ir1:%#x,ir2:%#x,ir3:%#x \r\n",ir_led_1,ir_led_2,ir_led_3);
        //    printf("red_led:%i , ir_led:%i \r\n",(*pun_red_led),(*pun_ir_led));
}

static void calc_red_ir_led(uint8_t red_1, uint8_t red_2, uint8_t red_3, uint8_t ir_1, uint8_t ir_2, uint8_t ir_3, uint32_t *red_led, uint32_t *ir_led)
{
        //calculate red_led && ir_led
        uint32_t u32_red_led1 = ((uint32_t)red_1) << 16;
        uint32_t u32_red_led2 = ((uint32_t)red_2) << 8;
        uint32_t u32_red_led3 = (uint32_t)red_3;
        uint32_t u32_ir_led1 = ((uint32_t)ir_1) << 16;
        uint32_t u32_ir_led2 = ((uint32_t)ir_2) << 8;
        uint32_t u32_ir_led3 = (uint32_t)ir_3;
        *red_led = u32_red_led1 + u32_red_led2 + u32_red_led3;
        *ir_led = u32_ir_led1 + u32_ir_led2 + u32_ir_led3;
        *red_led &= 0x03FFFF; //Mask MSB [23:18]
        *ir_led &= 0x03FFFF;  //Mask MSB [23:18]
}

static int max30102_INT_Capture()
{
        int io_23_status = gpio_get_level(GPIO_NUM_15);

        //if io_23 is high, interrupt not comes yet,return false
        if (io_23_status)
                return FALSE;
        else
                return TRUE;
}

static int get_led_data_into_buff()
{
        uint32_t red_led;
        uint32_t ir_led;
        int illegal_cnt;
        int i;
        for (i = 0; i < n_ir_buffer_length; i++)
        {
                illegal_cnt = 0;
                int legalty = FALSE;
                while (legalty == FALSE)
                {
                        max30102_read_redLed_and_irLed(&red_led, &ir_led);
                        //			printf("red:%i ,ir:%i \r\n",red_led,ir_led);
                        if ((red_led) < 100000 || (ir_led) < 100000)
                        { //???????????????(???????????)???????
                                illegal_cnt++;
                        }
                        else
                                legalty = TRUE;

                        if (illegal_cnt >= 500)
                                return FALSE;
                }
                aun_red_buffer[i] = red_led;
                aun_ir_buffer[i] = ir_led;
                //		printf("cnt[%d]  red:%i ,ir:%i \r\n",i,aun_red_buffer[i],aun_ir_buffer[i]);
        }
        return TRUE;
}

static void clear_FIFO()
{
        uint8_t u8_temp_1, u8_temp_2, u8_temp_3, u8_temp_4, u8_temp_5, u8_temp_6;
        int i;
        for (i = 0; i < 32; i++)
                max30102_read_reg_FIFO(&u8_temp_1, &u8_temp_2, &u8_temp_3, &u8_temp_4, &u8_temp_5, &u8_temp_6);
}

int finger_touch()
{
        clear_FIFO();
        uint8_t u8_temp_1, u8_temp_2, u8_temp_3, u8_temp_4, u8_temp_5, u8_temp_6;
        uint32_t red_led, ir_led;
        max30102_read_reg_FIFO(&u8_temp_1, &u8_temp_2, &u8_temp_3, &u8_temp_4, &u8_temp_5, &u8_temp_6);
        calc_red_ir_led(u8_temp_1, u8_temp_2, u8_temp_3, u8_temp_4, u8_temp_5, u8_temp_6, &red_led, &ir_led);
        if (red_led < 100000 || ir_led < 100000)
                return FALSE;
        else
                return TRUE;
}

static void fresh_first_100_samples()
{
        int i;
        uint32_t red, ir;

        for (i = 0; i < 100; i++)
        {
                max30102_read_redLed_and_irLed(&red, &ir);
        }
}
