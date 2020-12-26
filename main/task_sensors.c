#include "task_sensor.h"

uint16_t step=0;
spo2Andhr   bo2andhrinit={0,0};
spo2Andhr * bo2andhr=&bo2andhrinit;
float temp=0;

void sensor_data_update()
{
    while(1)
    {
        step=BMI160_read_step_count();
        // if(finger_touch()){
        //         bo2andhr=read_spo2_and_ir();
        //         if(!bo2andhr){
        //                 bo2andhr = &bo2andhrinit;
        //         }
        // }
        // else 
        //     bo2andhr=&bo2andhrinit;
        
        // temp=MAX30205_ReadTemperature();
        vTaskDelay(3000 / portTICK_RATE_MS);
    }
    
}

void ble_data_update()
{
    uint8_t stepdata[2],bo2data[4],hrdata[4],tempdata[2];
    while(1)
    {
        for(int i=0;i<2;i++)
            stepdata[i]=step>>((1-i)*8);
        for(int i=0;i<4;i++)
            bo2data[i]=bo2andhr->spo2>>((3-i)*8);
        for(int i=0;i<4;i++)
            hrdata[i]=bo2andhr->hr>>((3-i)*8);
        tempdata[0]=(uint8_t)temp;
        tempdata[1]=(uint8_t)((temp-(float)tempdata[0])*100);
        change_char_value(stepdata,sizeof(stepdata),1);
        change_char_value(bo2data,sizeof(bo2data),2);
        change_char_value(hrdata,sizeof(hrdata),3);
        change_char_value(tempdata,sizeof(tempdata),4);
        vTaskDelay(3000 / portTICK_RATE_MS);
        
        printf("ble data updated\r\n");
        /*test*/
        // step+=1;
        // temp+=1.1;
        // bo2andhr->spo2+=2;
        // bo2andhr->hr+=3;
        printf("+++++++++++++++++++++\r\n");
        printf("step:%d\r\ntemp:%f\r\nbo2:%d\r\nhr:%d\r\n",step,temp,bo2andhr->spo2,bo2andhr->hr);
        printf("+++++++++++++++++++++\r\n");
    }
    
}

typedef struct lcdA
{
    bool bleconnect;
    uint8_t power;
    uint8_t hour;
    uint8_t min;
    uint16_t stepnum;
}lcdmain;

typedef struct lcdB
{
    bool bleconnect;
    uint8_t power;
    uint32_t hr;
    uint32_t bo2;
    float tem;
}lcdsensor;

SemaphoreHandle_t mainSemaphore,sensorSemaphore;
QueueHandle_t mainQueue,sensorQueue;
void lcd_data_update()
{
    mainQueue=xQueueCreate(1,sizeof(lcdmain));
    sensorQueue=xQueueCreate(1,sizeof(lcdsensor));
    mainSemaphore = xSemaphoreCreateBinary();
    sensorSemaphore = xSemaphoreCreateBinary();
    
    while(1)
    {
        lcdmain A;
        lcdsensor B;

        A.bleconnect=isconnect();
        A.stepnum=step;

        B.bleconnect=isconnect();
        B.hr=bo2andhr->hr;
        B.bo2=bo2andhr->spo2;
        B.tem=temp;


        xQueueSend(mainQueue,(void *)&A,10);
        xQueueSend(sensorQueue,(void *)&B,10);
        xSemaphoreGive( mainSemaphore );
        xSemaphoreGive( sensorSemaphore );
        vTaskDelay(5000 / portTICK_RATE_MS);

        printf("lcd data updated\r\n");
    }
    
}


void lcd_show_main()
{
  	
	lcd_init();	

    while(1)
    {
        lcd_clear();
        lcdmain xMessage;
        if(xSemaphoreTake( mainSemaphore, 10 ) == pdTRUE)
        {
            char str[100]={0};
            xQueueReceive( mainQueue, (void *)&xMessage, portMAX_DELAY );
            if(xMessage.bleconnect)
                lcd_print_photo(0,0,1);
            else
             lcd_print_photo(0,0,0);

            memset(str,0,sizeof(str));
            // sprintf(str, "%d%%", xMessage.power);
            sprintf(str, "96%%");
            lcd_print_string(104,0,&str,16);
            

            lcd_print_photo(0,103,2);
            memset(str,0,sizeof(str)); 
            sprintf(str, "%d", xMessage.stepnum);
            lcd_print_string(33,107,&str,16);

            memset(str,0,sizeof(str));
            // sprintf(str, "%d:%d", xMessage.hour, xMessage.min);
            sprintf(str, "11:32");
            lcd_print_string(34,51,&str,24);

            lcd_refresh();
            vTaskDelay(5000 / portTICK_RATE_MS);
        }  

    }
	
}


void lcd_show_sensor()
{
  	
	lcd_init();	

    while(1)
    {
        lcd_clear();
        lcdsensor xMessage;
        if(xSemaphoreTake( sensorSemaphore, 10 ) == pdTRUE)
        {
            char str[100]={0};
            xQueueReceive( sensorQueue,(void *)&xMessage, portMAX_DELAY );

            if(xMessage.bleconnect)
                lcd_print_photo(0,0,1);
            else
             lcd_print_photo(0,0,0);

            memset(str,0,sizeof(str));
            // sprintf(str, "%d%%", xMessage.power);
            sprintf(str, "96%%");
            lcd_print_string(104,0,&str,16);
            

            lcd_print_photo(0,53,3);
            memset(str,0,sizeof(str)); 
            sprintf(str, "%.2f`C", xMessage.tem);
            lcd_print_string(33,57,&str,16);

            lcd_print_photo(0,78,4);
            memset(str,0,sizeof(str)); 
            if(xMessage.hr==0)
                sprintf(str, "--");
            else
                sprintf(str, "%d", xMessage.hr);
            lcd_print_string(33,82,&str,16);

            lcd_print_photo(0,103,5);
            memset(str,0,sizeof(str)); 
            if(xMessage.bo2==0)
                sprintf(str, "--");
            else
                sprintf(str, "%d", xMessage.bo2);
            lcd_print_string(33,107,&str,16);


            lcd_refresh();

            vTaskDelay(5000 / portTICK_RATE_MS);
        }
    }
	
}