#include <stm32f10x.h>
#include "usart.h"
#include "timer.h"
#include "fingerprint.h"


/******************
LED1 ״̬��ʾ��
LED2 �ɹ���ʾ��
LED3 ʧ����ʾ��
LED4 ������ʾ��
USER KEY		���ָ��
PRESS KEY 	�ȶ�ָ��
JOYSTICK C���� 	ɾ������ָ��
******************/
void delay()
{
	u16 i, j;
	for (i = 0; i < 1000; i++)
		for(j = 0; j < 10000; j++);
}

void USERGPIO_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(GPIOF, &GPIO_InitStructure);					
 GPIO_ResetBits(GPIOF,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);						 

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		
 GPIO_Init(GPIOG, &GPIO_InitStructure);				

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
 GPIO_Init(GPIOC, &GPIO_InitStructure);		
}

u8 key_scan(void)
{
	if(!USER_KEY||!PRESS_KEY||!DEL_KEY)
	{
		delay();
		if(!USER_KEY)	return 1;
		if(!PRESS_KEY) return 2;
		if(!DEL_KEY)	return 3;
	}
	return 0;
}

int main(void)
{
	int i = 5;
	SystemInit();
	USERGPIO_Init();
	usart_Configuration(19200,19200);		//ָ��ģ��Ĭ����19200

	printf("Test Uart Fingerprint Reader\r\n");
 	SetcompareLevel(5);
	printf("�ȶԵȼ�Ϊ��%d\r\n", GetcompareLevel());
	printf("��ǰ��ʱʱ�䣺%d\r\n",GetTimeOut());
	printf("User:%d\r\n",GetUserCount());
	
	while(1)
	{	
		switch(key_scan())
		{
			case 1:
				printf("User:%d\r\n",GetUserCount());
				switch(AddUser(i))
				{
					case ACK_SUCCESS:
						i++;
						printf("ָ����ӳɹ�\r\n");
						LED2_ON;	
						delay();
						LED2_OFF;
						break;
					
					case ACK_FAIL: 			
						printf("����ʧ��\r\n");
						LED3_ON;	
						delay();
						LED3_OFF;
						break;
					
					case ACK_FULL:			
						printf("ָ�ƿ�����\r\n");
						LED4_ON;
						delay();
						LED4_OFF;
						break;		
				}	
					break;
			case 2:
					switch(VerifyUser())
					{
						case ACK_SUCCESS:	
							printf("ƥ��ɹ�\r\n");
							LED2_ON;	
							delay();
							LED2_OFF;
							break;
						case ACK_NO_USER:
							printf("�޴��û�\r\n");
							LED3_ON;	
							delay();
							LED3_OFF;
							break;
						case ACK_TIMEOUT:	
							printf("��ʱ\r\n");
							LED3_ON;	
							delay();
							LED3_OFF;
							break;	
						case ACK_GO_OUT:
							printf("GO OUT\r\n");
							break;
					};
				break;
					
					
			case 3:
				ClearAllUser();
				printf("����ָ�������\r\n");
				break;
		}
	}
}


