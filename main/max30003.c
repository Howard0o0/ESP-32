/*
 * max30003.c
 *
 *  Created on: 2019��5��29��
 *      Author:ly
 */
#include "max30003.h"
#include "spi.h"
#include "uart.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "delay.h"
#include "freertos/task.h"
#include <string.h>


#define PIN_SPI_CS   19
#define PIN_SPI_MOSI 5
#define PIN_SPI_MISO 17
#define PIN_SPI_SCK  18

#define WREG 0x00   //???????��
#define RREG 0x01	//???????��

#define   STATUS          (0x01)
#define   EN_INT          (0x02)
#define   EN_INT2         (0x03)
#define   MNGR_INT        (0x04)
#define   MNGR_DYN        (0x05)
#define   SW_RST          (0x08)
#define   SYNCH           (0x09)
#define   FIFO_RST        (0x0A)
#define   INFO            (0x0F)
#define   CNFG_GEN        (0x10)
#define   CNFG_CAL        (0x12)
#define   CNFG_EMUX       (0x14)
#define   CNFG_ECG        (0x15)
#define   CNFG_RTOR1      (0x1D)
#define   CNFG_RTOR2      (0x1E)
#define   ECG_FIFO_BURST  (0x20)
#define   ECG_FIFO        (0x21)
#define   RTOR            (0x25)
#define   NO_OP           (0x7F)

uint8_t SPI_TX_Buff[4];
uint8_t SPI_RX_Buff[10];
uint8_t SPI_temp_32b[4];

uint32_t ecgData;
uint32_t measTimeCnt;

signed long ecgdata;
unsigned long data;
spi_device_handle_t spi;

void MAX30003_Write_Reg(uint8_t REG_ADDRESS, unsigned long data)
{
  uint8_t wRegName = (REG_ADDRESS<<1) | WREG;

  uint8_t txData[4];

  txData[0]=wRegName;
  txData[1]=(data>>16);    //��λ
  txData[2]=(data>>8);
  txData[3]=(data);

  esp_err_t ret;
  spi_transaction_t t;

  memset(&t, 0, sizeof(t));       //Zero out the transaction

  t.length=32;                 //Len is in bytes, transaction length is in bits.
  t.tx_buffer=&txData;               //Data
  ret=spi_device_transmit(spi, &t);  //Transmit!  С��оƬ �ȷ��͵��ֽڼ���7λ��6-0���ٷ���15-8      ���ӣ�0xe684 6c4e  ǰ��ʮ��λ  Ӧ�������� 4e 6c 84 e6
  assert(ret==ESP_OK);            //Should have had no issues.
}
void MAX30003_Read_Reg(uint8_t REG_ADDRESS)
{
    uint8_t rRegName=(REG_ADDRESS<<1 ) | RREG;

    SPI_TX_Buff[0] =rRegName;
    SPI_TX_Buff[1]=0x00;
    SPI_TX_Buff[2]=0x00;
    SPI_TX_Buff[3]=0x00;

    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction

    t.length=32;
    t.rxlength=32;
    t.tx_buffer=&SPI_TX_Buff;
    t.rx_buffer=&SPI_RX_Buff;

    t.user=(void*)0;
    ret=spi_device_transmit(spi, &t);
    assert(ret==ESP_OK);            //Should have had no issues.

    SPI_temp_32b[0] = SPI_RX_Buff[1];  //���ֽ�
    SPI_temp_32b[1] = SPI_RX_Buff[2];
    SPI_temp_32b[2] = SPI_RX_Buff[3];

    printf("%x,%x,%x,OK\n",SPI_RX_Buff[1],SPI_RX_Buff[2],SPI_RX_Buff[3]);  //SPI_RX_Buff[0] = 0��
}

void MAX30003_ReadID(void)
{
//   uint8_t SPI_TX_Buff[4];
//   uint8_t SPI_RX_Buff[10];


	MAX30003_Read_Reg(INFO);

   printf("ID:%x,%x,%x,%x\n",SPI_RX_Buff[3],SPI_RX_Buff[2],SPI_RX_Buff[1],SPI_RX_Buff[0]);
}



/*begins new ecg data operating and record��Ϳ�ʼ���µ��ĵ������ͼ�¼����ʼ��
���ڲ���MSTRʱ�ӱ�Ե���½���
SPIͬ������(����32��sck����)
��Ե)��ֻ�е�DIN[23:0] = 0x000000ʱ�Ż�ִ��,����fifo��ͬ��*/
void MAX30003_Synch(void)
{
	MAX30003_Write_Reg(SYNCH,0x000000);
}

/*�����MAX30003����Ϊ��ԭʼĬ��ֵ*/
void MAX30003_SW_Reset(void)
{
	MAX30003_Write_Reg(SW_RST,0x000000);
    vTaskDelay(100 / 100);
}


void MAX30003_Spi_Pre_Transfer_Callback(spi_transaction_t *t)
{
;
}

void ecg3_writeReg( uint8_t reg, uint32_t  data){
    uint8_t wRegName = (reg<<1) | WREG;

    uint8_t txData[4];

    txData[0]=wRegName;
    txData[1]=(data>>16);    //��λ
    txData[2]=(data>>8);
    txData[3]=(data);

    esp_err_t ret;
    spi_transaction_t t;

    memset(&t, 0, sizeof(t));       //Zero out the transaction

    t.length=32;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer=&txData;               //Data
    ret=spi_device_transmit(spi, &t);  //Transmit!  С��оƬ �ȷ��͵��ֽڼ���7λ��6-0���ٷ���15-8      ���ӣ�0xe684 6c4e  ǰ��ʮ��λ  Ӧ�������� 4e 6c 84 e6
    assert(ret==ESP_OK);            //Should have had no issues.
    
}


void ecg3_swReset( void )
{
    ecg3_writeReg( _ECG3_SW_RST_REG, _ECG3_SW_RST_CMD );
}

void ecg3_fifoReset( void )
{
    ecg3_writeReg( _ECG3_FIFO_RST_REG, _ECG3_FIFO_RST_CMD );
}

void ecg3_init( void )
{
    ecg3_writeReg( _ECG3_CNFG_GEN_REG, _ECG3_FMSTR_32768HZ_ECG_512HZ | _ECG3_ECG_CHANN_EN | _ECG3_DCLOFF_EN | _ECG3_RBIAS_100M_OHM | _ECG3_RBIASP_EN | _ECG3_RBIASN_EN );
    ecg3_writeReg( _ECG3_CNFG_CAL_REG, _ECG3_VCAL_EN | _ECG3_VMODE_BIPOL | _ECG3_VMAG_500MICROV );
    ecg3_writeReg( _ECG3_CNFG_EMUX_REG, _ECG3_ECGP_EN | _ECG3_ECGN_EN | _ECG3_ECGP_CAL_VCALP | _ECG3_ECGN_CAL_VCALN );
    ecg3_writeReg( _ECG3_CNFG_ECG_REG, 0x800000 | _ECG3_DHPF_500MILIHZ | _ECG3_DLPF_40HZ );
    ecg3_writeReg( _ECG3_CNFG_RTOR1_REG, _ECG3_WNDW_12 | _ECG3_RRGAIN_AUTO_SCALE | _ECG3_RTOR_EN | _ECG3_PAVG_8 | 0x000600 );
}

void MAX30003_Init(int pin_miso, int pin_mosi, int pin_sck, int pin_cs )
{

    esp_err_t ret;
    spi_bus_config_t buscfg=
    {
        .miso_io_num=pin_miso,
        .mosi_io_num=pin_mosi,
        .sclk_io_num=pin_sck,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1
    };

    spi_device_interface_config_t devcfg=
    {
        .clock_speed_hz=4000000,             	  //Clock out at 10 MHz
        .mode=0,                               	 //SPI mode 0
        .spics_io_num=pin_cs,              		 //CS pin
        .queue_size=7,                          //We want to be able to queue 7 transactions at a time
        .pre_cb=MAX30003_Spi_Pre_Transfer_Callback,  //Specify pre-transfer callback to handle D/C line
    };

    //Initialize the SPI bus
    ret=spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    assert(ret==ESP_OK);
    //Attach the max30003 to the SPI bus
    ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    assert(ret==ESP_OK);

    // vTaskDelay(300 / 10);  //delay 300ms
    vTaskDelay(300 / portTICK_RATE_MS);
    ecg3_swReset();
    ecg3_fifoReset();

    // vTaskDelay(100 / 10);  //delay 100ms
    vTaskDelay(100 / portTICK_RATE_MS);
    ecg3_init();

    // vTaskDelay(300 / 10);  //delay 300ms
     vTaskDelay(300 / portTICK_RATE_MS);


    // MAX30003_SW_Reset();
    // vTaskDelay(100 / 100);

    // MAX30003_Write_Reg(CNFG_GEN, 0x080004);   //���� ECG Channel enabled
    // vTaskDelay(100 / 100);

    // MAX30003_Write_Reg(CNFG_CAL, 0x720000);  // 0x700000
    // vTaskDelay(100 / 100);

    // MAX30003_Write_Reg(CNFG_EMUX,0x050000); //  ��vmid  0x0B0000
    // vTaskDelay(100 / 100);

    // MAX30003_Write_Reg(CNFG_ECG, 0x005000);  // d23 - d22 : 10 for 250sps , 00:500 sps    �˲�
    // vTaskDelay(100 / 100);

    // MAX30003_Write_Reg(CNFG_RTOR1,0x3fa300);//default 0x3fa300     0x3fc600
    // vTaskDelay(100 / 100);

    // MAX30003_Write_Reg(MNGR_INT, 0x000004);
    // vTaskDelay(100 / 100);

    // MAX30003_Synch();
    // vTaskDelay(100 / 100);

}

void ecg3_readReg( uint8_t REG_ADDRESS, uint32_t *dataOut ){
    uint8_t rRegName=(REG_ADDRESS<<1 ) | RREG;

    SPI_TX_Buff[0] =rRegName;
    SPI_TX_Buff[1]=0x00;
    SPI_TX_Buff[2]=0x00;
    SPI_TX_Buff[3]=0x00;

    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction

    t.length=32;
    t.rxlength=32;
    t.tx_buffer=&SPI_TX_Buff;
    t.rx_buffer=&SPI_RX_Buff;

    t.user=(void*)0;
    ret=spi_device_transmit(spi, &t);
    assert(ret==ESP_OK);            //Should have had no issues.

    *dataOut = SPI_RX_Buff[ 1 ];
    *dataOut <<= 8;
    *dataOut |= SPI_RX_Buff[ 2 ];
    *dataOut <<= 8;
    *dataOut |= SPI_RX_Buff[ 3 ];

    // printf("%x,%x,%x,OK\n",SPI_RX_Buff[1],SPI_RX_Buff[2],SPI_RX_Buff[3]);  //SPI_RX_Buff[0] = 0��

}

void ecg3_getECG( uint32_t *outECG )
{
    ecg3_readReg( _ECG3_ECG_FIFO_REG, outECG );
    *outECG >>= 6;
}


void plotECG(void)
{
    if (ecgData > 50000)
    {
        printf("measuer count[%d],ecgdata = %X",measTimeCnt,ecgData);

        if (measTimeCnt == 0xFFFFFFFF)
        {
            measTimeCnt = 0;
        }
        else
        {
            measTimeCnt++;
        }
    }
    // vTaskDelay(8 / 100); //delay 8ms
     vTaskDelay(8 / portTICK_RATE_MS);
}


void MAX30003_Read_Data(void)
{
//	MAX30003_SW_Reset();
	MAX30003_Read_Reg(ECG_FIFO);
    unsigned long data0 = (unsigned long) (SPI_temp_32b[0]);   // �ȶ�����λ�����ݣ���16λ�����ݣ���8λ��tag
    data0 = data0 <<24;
    printf("*************data0 = %ld\n",data0);
    unsigned long data1 = (unsigned long) (SPI_temp_32b[1]);   //��rx_buffer[3]������
    data1 = data1 <<16;
    printf("*************data1 = %ld\n",data1);
    unsigned long data2 = (unsigned long) (SPI_temp_32b[2]);
    data2 = data2 >>6;
    data2 = data2 & 0x03;
    printf("*************data2 = %ld\n",data2);
    data = (unsigned long) (data0 | data1 | data2);
    ecgdata = (signed long) (data);
    printf("*************ecgdata = %ld\n",ecgdata);

    MAX30003_Read_Reg(RTOR);
    unsigned long RTOR_msb = (unsigned long) (SPI_temp_32b[0]);
    unsigned char RTOR_lsb = (unsigned char) (SPI_temp_32b[1]);
    printf("*************RTOR_msb = %ld\n",RTOR_msb);
    unsigned long rtor = (RTOR_msb<<8 | RTOR_lsb);
    rtor = ((rtor >>2) & 0x3fff) ;
    printf("*************rtor = %ld\n",rtor);
    float hr =  60 /((float)rtor*0.008);

    unsigned int HR = (unsigned int)hr;  // type cast to int
    unsigned int RR = (unsigned int)rtor*8 ;  //8ms

    printf("HR = %d\n",HR);
    printf("RR = %d\n",RR);
//    MAX30003_Synch();
}

void Read_Data(void)			// calls max30003read_data to update to aws_iot.
{
 	uint8_t* db;

    while(1)
    {
  		MAX30003_Read_Data();
  		vTaskDelay(1000/10); //delay 1s
    }
}

