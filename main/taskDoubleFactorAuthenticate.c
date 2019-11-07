#include "taskDoubleFactorAuthenticate.h"
#include "cJSON.h"
#include "authenticate.h"
#include "fpga.h"
#include "szm301.h"
#include "string.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "util.h"
#include "gatts_table.h"
#include "freertos/queue.h"
#include "szm301.h"

/************** global variables *****************************************/

/* BLE */
uint8_t *bleRcvBuf  = NULL;   
uint16_t bleRcvLen;

/* Task */
TaskHandle_t TH_sendJsonCR = NULL;
SemaphoreHandle_t xSemaWakeUpTaskRegister;

/* Queue */
QueueHandle_t xQueueRcvBleMsg; 
QueueHandle_t xQueueChallenge;
QueueHandle_t xQueueJsonToSend;

/************** End of global variables *****************************************/


/* ************ private function ************************************/

static void taskRcvBleMsg(void);
static void taskParseJson(void);
static void taskRegister(void);
static void taskAuthenticate(void);
static void taskSendBleMsg(void);

/* ************ End of private function ************************************/


static void taskRcvBleMsg(void)
{
    bleRcvBuf = (uint8_t *)malloc(512*sizeof(uint8_t));
    memset(bleRcvBuf,0,512);
    blue_init();
    xQueueRcvBleMsg = xQueueCreate( 3, sizeof(uint8_t *) );

    while (1)
    {
        /* waiting for ble msg */
        if(get_char_value(&bleRcvBuf,&bleRcvLen,4) != 0) 
        {
            vTaskDelay(10 / portTICK_RATE_MS);
            continue;
        }

        /* receive a ble msg, send bleRcvBuf ptr to taskParse via queue */
        if ( xQueueSend( xQueueRcvBleMsg, ( void * ) &bleRcvBuf, 100 / portTICK_RATE_MS ) != pdPASS )
        {
            printf(" send bleRcvBuf into queue failed! \r\n ");
        }


        UBaseType_t leftStackSize = uxTaskGetStackHighWaterMark( NULL );
        printf("taskRcvBleMsg stack size remained : %d \r\n",leftStackSize);

        vTaskDelay( 100 / portTICK_RATE_MS) ;
    }
    
}

static void taskParseJson(void)
{
    uint8_t *pu8BleRcvFromQue = NULL;
    cJSON *jsCmd = NULL;
    char *pcCmd = NULL;
    char *pcChallenge = NULL;

    xSemaWakeUpTaskRegister =  xSemaphoreCreateBinary();
    xQueueChallenge = xQueueCreate( 3, sizeof(char *) );
    xQueueJsonToSend = xQueueCreate( 3, sizeof(cJSON *) );

    while (1)
    {
        UBaseType_t leftStackSize = uxTaskGetStackHighWaterMark( NULL );
        printf("taskParseJson stack size remained : %d \r\n",leftStackSize);

        /* waiting for ble msg received */
        if(xQueueRcvBleMsg == NULL)
        {
            vTaskDelay(10 / portTICK_RATE_MS);
            continue;
        }
        xQueueReceive(xQueueRcvBleMsg, (void *)&pu8BleRcvFromQue, portMAX_DELAY);

        /* receive msg, judge if is json */
        printf("BufLen:%d \r\n",bleRcvLen);
        printf("bleRcvBuf: \n %s \n",(char *)pu8BleRcvFromQue);
        jsCmd = cJSON_Parse((const char *)pu8BleRcvFromQue);
        if(jsCmd == NULL) //received msg is not a json , drop
        {
            vTaskDelay(10 / portTICK_RATE_MS);
            continue;
        }

        /* receive a json msg, check if key(cmd) is contained */
        if(  cJSON_GetObjectItem( jsCmd , "cmd" ) == NULL ) //no "cmd" in jsCmd
        {
            printf("cmd is not contained in json received \r\n");
            vTaskDelay(10 / portTICK_RATE_MS);
            continue;
        }

        /* received a legal json */
        pcCmd = cJSON_GetObjectItem( jsCmd , "cmd" )->valuestring;

        /* if json cmd == "register", wake up taskRegister */
        if(strstr(pcCmd,"register") != 0) //receive cmd:register
        {
            if(  xSemaphoreGive(xSemaWakeUpTaskRegister) == pdFAIL )
            {
                printf(" wake up taskRegister failed! \r\n ");
            }
        }

        /* if json cmd == "authenticate", send C to xQueueChallenge and wake up taskAuthenticate */
        if( (strstr(pcCmd,"authenticate") != 0) && (cJSON_GetObjectItem( jsCmd , "challenge" ) != NULL) ) //receive cmd:authenticate
        {
            pcChallenge = calloc( strlen(cJSON_GetObjectItem( jsCmd , "challenge" )->valuestring) , sizeof(char));
            memcpy(pcChallenge,cJSON_GetObjectItem( jsCmd , "challenge" )->valuestring,strlen(cJSON_GetObjectItem( jsCmd , "challenge" )->valuestring));
            xQueueSend( xQueueChallenge, ( void * ) &pcChallenge, 100 / portTICK_RATE_MS );
        }

        cJSON_Delete(jsCmd);
        
    }
    
}

static void taskRegister(void)
{
    while (1)
    {
        
        UBaseType_t leftStackSize = uxTaskGetStackHighWaterMark( NULL );
        printf("taskRegister stack size remained : %d \r\n",leftStackSize);

        /* 1. waiting to wake up */
         xSemaphoreTake(xSemaWakeUpTaskRegister,portMAX_DELAY);

        /* 2. build C-R json and send json ptr to xQueueJsonToSend */
        cJSON *cjsonRegister = makeJsonForRegister();
        if(cjsonRegister == NULL)
        {
            cjsonRegister = makeJsonRegisterErr();
        }
        
        xQueueSend( xQueueJsonToSend, ( void * ) &cjsonRegister, 100 / portTICK_RATE_MS );
        
    }
    
}

static void taskAuthenticate(void)
{
    char *pcChallengeFromxQue = NULL;
    cJSON *cjsonAuth = NULL;
    int iErr;

    while (1)
    {
        UBaseType_t leftStackSize = uxTaskGetStackHighWaterMark( NULL );
        printf("taskAuthenticate stack size remained : %d \r\n",leftStackSize);

        /* waiting for challenge */
        if(xQueueChallenge == NULL)
        {
            vTaskDelay(10 / portTICK_RATE_MS);
            continue;
        }
        xQueueReceive( xQueueChallenge, ( void * ) &pcChallengeFromxQue, portMAX_DELAY );

        /* malloc a space for HexStrToHex */
        uint8_t *pu8ChallengeHex = malloc(199*sizeof(uint8_t) + 1);
        HexStrToByte((const char*) pcChallengeFromxQue, (unsigned char*)pu8ChallengeHex, 199*2);



        /* make R json */
        cjsonAuth = makeJsonForAuthenticate(pu8ChallengeHex);
        if(cjsonAuth == NULL)
        {
            cjsonAuth = makeJsonAuthenticateErr();
        }

        /* send json */
        xQueueSend( xQueueJsonToSend, ( void * ) &cjsonAuth, 100 / portTICK_RATE_MS );

        /* free calloc */
        free(pcChallengeFromxQue);
        pcChallengeFromxQue = NULL;
        free(pu8ChallengeHex);
        pu8ChallengeHex = NULL;
        
        
        /* build R json and send json ptr to xQueueJsonToSend */
        // cJSON *jsonR = makeJson_R();
        // xQueueSend( xQueueJsonToSend, ( void * ) &jsonR, 100 / portTICK_RATE_MS );

    }
    
}

static void taskSendBleMsg(void)
{
    cJSON *jsonToSend = NULL;

    while (1)
    {
        UBaseType_t leftStackSize = uxTaskGetStackHighWaterMark( NULL );
        printf("leftStackSize stack size remained : %d \r\n",leftStackSize);


        /* waiting for jsonToSend */
        if(xQueueJsonToSend == NULL)
        {;
            vTaskDelay(10 / portTICK_RATE_MS);
            continue;
        }
        xQueueReceive( xQueueJsonToSend, ( void * ) &jsonToSend, portMAX_DELAY );

        /* turn json into char* and send BLE msg */
        char *pcjson = cJSON_Print(jsonToSend);
        server_notify_client_test((uint8_t *)pcjson,strlen(pcjson));
        printf("len(%d) , sent json : \r\n %s \r\n",(int)strlen(pcjson),pcjson);

        /* free cjson */
        free(pcjson);
        cJSON_Delete(jsonToSend);
    }
    
}


void mainTaskAuthenticate(void)
{


    xTaskCreate(	(TaskFunction_t)taskRcvBleMsg,		/* Pointer to the function that implements the task. */
                    "taskRcvBleMsg",	/* Text name for the task.  This is to facilitate debugging only. */
                    16*1024,		/* Stack depth in words. */
                    NULL,		/* We are not using the task parameter. */
                    configMAX_PRIORITIES -3,			/* This task will run at priority 1. */
                    NULL);	

    xTaskCreate(	(TaskFunction_t)taskParseJson,		/* Pointer to the function that implements the task. */
                    "taskParseJson",	/* Text name for the task.  This is to facilitate debugging only. */
                    16*1024,		/* Stack depth in words. */
                    NULL,		/* We are not using the task parameter. */
                    configMAX_PRIORITIES -3,			/* This task will run at priority 1. */
                    NULL);	
    
    xTaskCreate(	(TaskFunction_t)taskRegister,		/* Pointer to the function that implements the task. */
                    "taskRegister",	/* Text name for the task.  This is to facilitate debugging only. */
                    16*1024,		/* Stack depth in words. */
                    NULL,		/* We are not using the task parameter. */
                    configMAX_PRIORITIES -3,			/* This task will run at priority 1. */
                    NULL);	
    
    xTaskCreate(	(TaskFunction_t)taskAuthenticate,		/* Pointer to the function that implements the task. */
                    "taskAuthenticate",	/* Text name for the task.  This is to facilitate debugging only. */
                    52*1024,		/* Stack depth in words. */
                    NULL,		/* We are not using the task parameter. */
                    configMAX_PRIORITIES -3,			/* This task will run at priority 1. */
                    NULL);	
    
    xTaskCreate(	(TaskFunction_t)taskSendBleMsg,		/* Pointer to the function that implements the task. */
                    "taskSendBleMsg",	/* Text name for the task.  This is to facilitate debugging only. */
                    16*1024,		/* Stack depth in words. */
                    NULL,		/* We are not using the task parameter. */
                    configMAX_PRIORITIES -3,			/* This task will run at priority 1. */
                    NULL);	

    vTaskDelete(NULL);
}