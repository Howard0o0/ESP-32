#include <stdbool.h>
#include <stdint.h>
#include <string.h> 
#include "ls013b7dh03.h"
#include "oled_font.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "sys/unistd.h"
#include "../util/include/delay.h"

// Commands
#define MLCD_WR 0x80 //MLCD write line command
#define MLCD_CM 0x20 //MLCD clear memory command
#define MLCD_NO 0x00 //MLCD NOP command (used to switch VCOM)

//defines the VCOM bit in the command word that goes to the LCD
#define VCOM_HI 0x40
#define VCOM_LO 0x00


// #define DIS_1 nrf_gpio_pin_set(8);
// #define DIS_0 nrf_gpio_pin_clear(8);

// #define EXT_1 
// #define EXT_0 

// #define SCS_1 nrf_gpio_pin_set(7);
// #define SCS_0 nrf_gpio_pin_clear(7);

// #define SI_1 nrf_gpio_pin_set(6);
// #define SI_0 nrf_gpio_pin_clear(6);

// #define SCLK_1 nrf_gpio_pin_set(5);
// #define SCLK_0 nrf_gpio_pin_clear(5);

// #define LCD_ON nrf_gpio_pin_set(4);
// #define LCD_OFF nrf_gpio_pin_clear(4);

#define LINE (128)
#define LINE_SIZE (16)

#define PARALLEL_LINES 16

#define PIN_NUM_CS 16 
#define PIN_NUM_MOSI 5
#define PIN_NUM_CLK 18
#define PIN_NUM_DISP_ON_OFF 4
#define GPIO_OUTPUT_PIN_SEL  ( (1ULL<<PIN_NUM_CS) | (1ULL<<PIN_NUM_MOSI) | (1ULL<<PIN_NUM_CLK) | (1ULL<<PIN_NUM_DISP_ON_OFF) )

#define DIS_0 gpio_set_level(PIN_NUM_DISP_ON_OFF,0);
#define DIS_1 gpio_set_level(PIN_NUM_DISP_ON_OFF,1);
#define SCS_0 gpio_set_level(PIN_NUM_CS,0);
#define SCS_1 gpio_set_level(PIN_NUM_CS,1);
#define SCLK_0 gpio_set_level(PIN_NUM_CLK,0);
#define SCLK_1 gpio_set_level(PIN_NUM_CLK,1);
#define SI_0 gpio_set_level(PIN_NUM_MOSI,0);
#define SI_1 gpio_set_level(PIN_NUM_MOSI,1);


spi_device_handle_t spi_device_handler_lcd;
static char LCD_GRAM[LINE][LINE_SIZE];

//This function is called (in irq context!) just before a transmission starts. It will
//set the D/C line to the value indicated in the user field.
void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
    // int dc=(int)t->user;
    // gpio_set_level(PIN_NUM_DC, dc);
}

void lcd_install(void){
      esp_err_t ret;
      
      spi_bus_config_t buscfg={
          .miso_io_num=-1,     //read operation is no need here
          .mosi_io_num=PIN_NUM_MOSI,
          .sclk_io_num=PIN_NUM_CLK,
          .quadwp_io_num=-1,
          .quadhd_io_num=-1,
          .max_transfer_sz=PARALLEL_LINES*320*2+8
      };
      spi_device_interface_config_t devcfg={
#ifdef CONFIG_LCD_OVERCLOCK
        .clock_speed_hz=26*1000*1000,           //Clock out at 26 MHz
#else
        .clock_speed_hz=10*1000*1000,           //Clock out at 10 MHz
#endif
        .mode=0,                                //SPI mode 0
        .spics_io_num=-1,               //CS pin
         .queue_size=7,                          //We want to be able to queue 7 transactions at a time
        .pre_cb=lcd_spi_pre_transfer_callback,  //Specify pre-transfer callback to handle D/C line
    };
    //Initialize the SPI bus
    ret=spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    ESP_ERROR_CHECK(ret);
    printf("bus initialized!\r\n");
    //Attach the LCD to the SPI bus
    ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spi_device_handler_lcd);
    printf("bus add device!\r\n");
    ESP_ERROR_CHECK(ret);
    //initial display_on_off&&CS to gpio output
    gpio_set_direction(PIN_NUM_DISP_ON_OFF, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_CS, GPIO_MODE_OUTPUT);
}

void lcd_display_on_off(uint8_t status){
    if(status)
      gpio_set_level(PIN_NUM_DISP_ON_OFF, 1);
    else
    {
      gpio_set_level(PIN_NUM_DISP_ON_OFF, 0);
    }
    
}

void lcd_cmd(uint8_t cmd)
{

    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length=8;                     //Command is 8 bits
    t.tx_buffer=&cmd;               //The data is the cmd itself
    ret=spi_device_polling_transmit(spi_device_handler_lcd, &t);  //Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.

}

void lcd_cs_chooese(uint8_t status){
  if(status)
      gpio_set_level(PIN_NUM_CS, 1);
    else
    {
      gpio_set_level(PIN_NUM_CS, 0);
    }
}


/*=========================================================================================== */

void lcd_gpio_init(void){
      gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
}



void lcd_init(void)
{
		lcd_gpio_init();
	
		DIS_0;
		SCS_0;
		lcd_clear();
		DIS_1;
		delay_us(30);

		delay_us(30);
}

void write_byte(uint8_t Dbyte)
{
	uint8_t i;
	SCLK_0;
	for(i=0;i<8;i++)
	{
		if(Dbyte&0x80)
		{
		 SI_1;
		}
		else
		{
		 SI_0;
		}

		SCLK_1;
		SCLK_0;
		Dbyte=Dbyte<<1;
		delay_us(5);
	}

}

void write_byte1(uint8_t Dbyte)
{
	uint8_t i;
	SCLK_0;
	for(i=0;i<8;i++)
	{
		 if(Dbyte&0x01)
		 {
			 SI_1;
		 }
		 else
		 {
			 SI_0;
		 }

		 SCLK_1;
		 SCLK_0;
		 Dbyte=Dbyte>>1;
		 delay_us(5);
	}
}



void lcd_clear(void)
{
	SCS_1;
	delay_us(5);
	write_byte(0x20);
	write_byte(0);

  for(int i=0;i<LINE;i++)
  {
    for(int j=0;j<LINE_SIZE;j++)
    LCD_GRAM[i][j]=0;
  }

	delay_us(5);
	SCS_0;
}

void lcd_refresh(void){
	uint16_t i=0,ln=0;
	SCS_1;
	delay_us(5);
	write_byte(0x80);
	
	for(ln=0;ln<128;ln++)
	{
		write_byte1(ln);
		for(i=0;i<16;i++)
		{
			write_byte(LCD_GRAM[ln][i]);
		}
		write_byte(0);
	}

	write_byte(0);write_byte(0);

	delay_us(5);
	SCS_0;
	
}

void lcd_drawpoint(uint16_t x,uint16_t y,bool bDraw){
	
 uint16_t pos,bx,tmp;

  if(x>OLED_MAX_X-1||y>OLED_MAX_Y-1)
    return;
  pos=15-y/8;
  bx=y%8;
	tmp=1<<(bx);
  if(bDraw)
     LCD_GRAM[x][pos]|= tmp;
    else
     LCD_GRAM[x][pos]&= ~tmp; 
    
}

void lcd_fillRect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,bool bDraw)
{
  uint16_t wi,hi;
  for(hi=0;hi<h;hi++){
   for(wi=0;wi<w;wi++)
      lcd_drawpoint(x+wi,y+hi,bDraw);
  }
}

void lcd_fillRectByXY(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,bool bDraw)
{
   uint16_t xStart=0,yStart=0;
   uint16_t w,h;
   
   if(x0<x1){
     xStart=x0;
     w=x1-x0+1;
   }else{
     xStart=x1;
     w=x0-x1+1;
   }
   
   if(y0<y1){
     yStart=y0;
     h=y1-y0+1;
   }else{
      yStart=y1;
      h=y0-y1+1;
   }
   lcd_fillRect(xStart,yStart,w,h,bDraw);
   
}

// uint32_t lcd_getpoint(uint16_t x,uint16_t y)
// {
// }

void lcd_print_char(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{                   
    uint8_t temp,t,t1;
    uint8_t y0=y;
    uint8_t csize=(size/8+((size%8)?1:0))*(size/2);      
    chr=chr-' ';       
    for(t=0;t<csize;t++)
    {   
        if(size==12)temp=asc2_1206[chr][t];         
        else if(size==16)temp=asc2_1608[chr][t];    
        else if(size==24)temp=asc2_2412[chr][t];    
        else return;                                
        for(t1=0;t1<8;t1++)
        {
            if(temp&0x80)lcd_drawpoint(x,y,mode);
            else lcd_drawpoint(x,y,!mode);
            temp<<=1;
            y++;
            if((y-y0)==size)
            {
                y=y0;
                x++;
                break;
            }
        }    
    }          
}

uint32_t mypow(uint8_t m,uint8_t n)
{
    uint32_t result=1;    
    while(n--)result*=m;    
    return result;
}                 
         
void lcd_print_num(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{           
    uint8_t t,temp;
    uint8_t enshow=0;                           
    for(t=0;t<len;t++)
    {
        temp=(num/mypow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                lcd_print_char(x+(size/2)*t,y,' ',size,1);
                continue;
            }else enshow=1; 
              
        }
        lcd_print_char(x+(size/2)*t,y,temp+'0',size,1); 
    }
} 

void lcd_print_charString(uint8_t x,uint8_t y,const char *p,uint8_t size)
{
    lcd_print_string(x,y,(uint8_t *)p,size);
    lcd_refresh();
}

void lcd_print_string(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size)
{   
    while((*p<='~')&&(*p>=' '))
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(128-size)){y=x=0;lcd_clear();}
        lcd_print_char(x,y,*p,size,1);    
        x+=size/2;
        p++;
    }  
     
}

void lcd_print_photo(uint8_t x,uint8_t y,int type)
{
  uint8_t temp,x1,y1;
  uint8_t y0=y;  
  const unsigned char *data;


  switch(type){
    case 0:{
      data=miniunbt;
      x1=2;
      y1=16;
      break; 
    }
    case 1:{       
      data=minibt;
      x1=2;
      y1=16;
      break; 
    }
    case 2:{       
      data=ministep;
      x1=3;
      y1=24;
      break; 
    }
    case 3:{       
      data=minitemperature;
      x1=3;
      y1=24;
      break; 
    }
    case 4:{       
      data=miniheartrate;
      x1=3;
      y1=24;
      break; 
    }
    case 5:{       
      data=minibloodoxygen;
      x1=3;
      y1=24;
      break; 
    }
    default:
    {
      return;
      break;
    }
      
  }

  // printf("ID:%d\r\n",type);
  // for(int l=0;l<y1;l++) printf("1");
  // printf("\r\n");
  // int xx=x;
  // for(int i=0;i<y1;i++)
  // {
  //   for(int j=0;j<x1;j++)
  //   {
  //     temp=data[i+j*y1];
  //     for(int k=0;k<8;k++)
  //     {
  //       if(temp&0x01) printf("1");
  //         else printf(" ");
  //       temp>>=1;
  //       y++;
  //     }
  //   }
  //   printf("1\r\n");
  //   y=y0;
  //   xx++;
  // }

  for(int i=0;i<y1;i++)
  {
    for(int j=0;j<x1;j++)
    {
      temp=data[i+j*y1];
      for(int k=0;k<8;k++)
      {
        if(temp&0x01) lcd_drawpoint(x,y,1);
          else lcd_drawpoint(x,y,0);
        temp>>=1;
        y++;
      }
    }
    y=y0;
    x++;
  }
}


