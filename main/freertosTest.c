#include "freertosTest.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/FreeRTOSConfig.h"

/* Global Variable */
QueueHandle_t xQueue;
TaskHandle_t TH_defaultTask = NULL;
TaskHandle_t TH_notifyRcv = NULL;
TaskHandle_t TH_notifySend = NULL;
SemaphoreHandle_t xSemaphore;

void defaultTask(void)
{

    xSemaphore = xSemaphoreCreateBinary();
    xQueue = xQueueCreate(3,sizeof(char *));

	/* test taskNotify */
	xTaskCreatePinnedToCore(	(TaskFunction_t)task1,		/* Pointer to the function that implements the task. */
								"Task 1",	/* Text name for the task.  This is to facilitate debugging only. */
								(1024*16-14896)*1.5,		/* Stack depth in words. */
								NULL,		/* We are not using the task parameter. */
								configMAX_PRIORITIES-1,			/* This task will run at priority 1. */
								NULL,
								0 );		

	xTaskCreatePinnedToCore(	(TaskFunction_t)task2,		/* Pointer to the function that implements the task. */
								"Task 2",	/* Text name for the task.  This is to facilitate debugging only. */
								(1024*16-14896)*1.5,		/* Stack depth in words. */
								NULL,		/* We are not using the task parameter. */
								configMAX_PRIORITIES-1,			/* This task will run at priority 1. */
								NULL, 
								1);		

    /* test message queue */
    // xTaskCreate(	(TaskFunction_t)taskNotifySend,		/* Pointer to the function that implements the task. */
    //                 "Task 1",	/* Text name for the task.  This is to facilitate debugging only. */
    //                 1024,		/* Stack depth in words. */
    //                 NULL,		/* We are not using the task parameter. */
    //                 tskIDLE_PRIORITY,			/* This task will run at priority 1. */
    //                 &TH_notifySend);
    
    // xTaskCreate(	(TaskFunction_t)taskNotifySend,		/* Pointer to the function that implements the task. */
    //                 "Task 1",	/* Text name for the task.  This is to facilitate debugging only. */
    //                 1024,		/* Stack depth in words. */
    //                 NULL,		/* We are not using the task parameter. */
    //                 tskIDLE_PRIORITY,			/* This task will run at priority 1. */
    //                 &TH_notifySend);
	
    vTaskDelete(TH_defaultTask);

	
}


char acChallenge[100] = "kkkkkk";
void taskNotifySend(void *pvParameters)
{
	while (1)
	{
        acChallenge[0] = 'a';
        acChallenge[1] = 'b';
        acChallenge[2] = 'c';
        acChallenge[3] = 0;
		printf("taskNotifySend \r\n");
        if( xSemaphore != NULL )
        {
		    xSemaphoreGive(xSemaphore);
        }
		vTaskDelay( 1000 / portTICK_RATE_MS) ;

        UBaseType_t leftStackSize = uxTaskGetStackHighWaterMark( NULL );
        printf("taskNotifySend stack size remained : %d \r\n",leftStackSize);
	}
	
}

void taskNotifyReceive(void *pvParameters)
{
	while (1)
	{
        if( xSemaphore != NULL )
        {
    		xSemaphoreTake(xSemaphore,portMAX_DELAY);
        }
		
		printf("taskNotifyReceive \r\n");
        printf("acChallenge : %s \r\n",acChallenge);
        UBaseType_t leftStackSize = uxTaskGetStackHighWaterMark( NULL );
        printf("taskNotifyReceive stack size remained : %d \r\n",leftStackSize);
	}
	 
}

/* Message Queue Test */
void task1(void)
{
	while (1)
	{
		printf("task1 \r\n");
		char *sendMsg1 = "hello";
		char *sendMsg2 = "hi";
		if(xQueue != NULL)
		{
			xQueueSend( xQueue, ( void * ) &sendMsg1, ( TickType_t ) 10 );	
			xQueueSend( xQueue, ( void * ) &sendMsg2, ( TickType_t ) 10 );	
		}
		vTaskDelay( 1000 / portTICK_RATE_MS) ;
	}
	
}

void task2(void)
{
	char *rcvMsg = NULL;
	while (1)
	{
		printf("task2 \r\n");
		if(xQueueReceive(xQueue, &rcvMsg, ( TickType_t ) 10) == pdPASS)
		{
            printf("queue space left : %d \r\n",uxQueueMessagesWaiting(xQueue));
			printf("task rcv msg : %s \r\n",rcvMsg);
		}

        if(xQueueReceive(xQueue, &rcvMsg, ( TickType_t ) 10) == pdPASS)
		{
            printf("queue space left : %d \r\n",uxQueueMessagesWaiting(xQueue));
			printf("task rcv msg : %s \r\n",rcvMsg);
		}
		vTaskDelay( 1000 / portTICK_RATE_MS) ;
	}
	
}

/* End of Message Queue Test */


