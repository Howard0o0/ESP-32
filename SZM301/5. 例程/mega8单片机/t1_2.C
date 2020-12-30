//ICC-AVR application builder : 2011-11-22 下午 12:30:03
// Target : M8
// Crystal: 6.0000Mhz

#include <iom8v.h>
#include <macros.h>
#include <eeprom.h>

typedef unsigned char 	U8;
typedef unsigned short  U16;
typedef unsigned long   U32;

#define TRUE  1
#define FALSE 0

//基本应答信息定义
#define ACK_SUCCESS       0x00
#define ACK_FAIL          0x01
#define ACK_FULL          0x04
#define ACK_NO_USER		  0x05
#define ACK_TIMEOUT       0x08

#define ACK_GO_OUT		  0x0F
//用户信息定义
#define ACK_ALL_USER       0x00
#define ACK_GUEST_USER 	  0x01
#define ACK_NORMAL_USER 	  0x02
#define ACK_MASTER_USER    0x03

#define USER_MAX_CNT	   50

//命令定义
#define CMD_HEAD		  0xF5
#define CMD_TAIL		  0xF5
#define CMD_ADD_1  		  0x01
#define CMD_ADD_2 		  0x02
#define CMD_ADD_3	  	  0x03
#define CMD_MATCH		  0x0C
#define CMD_DEL			  0x04
#define CMD_DEL_ALL		  0x05
#define CMD_USER_CNT      0x09
#define CMD_LP_MODE		  0x2C

#define CMD_FINGER_DETECTED 0x14

#define Beep_Short		  45
#define Beep_Long		  230
#define Delay_Cap         300
#define Tout_Cnt		  5
#define Flash_Cnt         20

#define Flg_Add      	  0
		//注册标志位	  
#define Flg_Del 		  1
		//删除标志位
#define Flg_DelAll		  2
		//全部删除标志位
#define Flg_Door		  3
		//门状态传感器标志位
#define Flg_Close		  4
		//关门状态传感器标志位
#define Flg_Open		  5
		//开门状态传感器标志位
#define Flg_Sleep 	  	  6
		//休眠标志位
#define Flg_Interrupt 	  7
		//中断标志位

#define Key_Add			  0
		//注册按键        PB
#define Key_Del			  1
		//删除按键        PB
#define Key_DelAll		  2
		//全部删除按键    PB
#define Sensor_Status     3
		//采集头状态	  PB
#define State_Close       4
		//关门状态        PB
#define State_Open  	  5
		//开门状态  	  PB
		
#define Wg_Data0		  0
		//韦根输出0       PC
#define Wg_Data1		  1
		//韦根输出1       PC
#define Ctrl_Speaker	  2
		//蜂鸣器		  PC
#define Light_Green  	  3
		//绿灯			  PC
#define Light_Red		  4
		//红灯			  PC
#define Power_5V		  5
		//5V电源		  PC

#define Uart_Rx           0
		//UART输入        PD
#define Uart_Tx	  		  1		
		//UART输出		  PD
#define Reset_Dsp		  2
		//DSP复位		  PD
#define State_Door	  	  3
		//门状态		  PD
#define Power_Dsp		  4
		//DSP电源		  PD
#define Ctrl_AD			  5
		//电压检测控制	  PD
#define Trans_Negative 	  6
		//电机反转控制	  PD
#define Trans_Positive 	  7
		//电机正转控制	  PD
		
//=========全局变量==========
U8 gRsBuf[9];				 //串行接收缓存
U8 gTxBuf[9];				 //串行发送缓存
U8 gFlag, gFlagInput;
U8 gFlashRed, gFlashGreen;
U8 gIsLightOn;
U8 gSensorOn;
U8 gFlagInput;
U8 gKeyTemp1, gKeyTemp2;
U8 gTimer0Cnt;
U8 gRsLength;
U8 gDoorClosed;
U8 gDoorCloseEnable;
U8 gDoorCloseCnt;

void Delay100US()
{
 	 U16 i;
	 for (i = 85; i > 0; i--);
}
void DelayMS(U16 tim)         //1ms延时
{
	 U16 i;
	 while(tim > 0)
	 {
	  	 for(i = 856; i > 0; i--){;};
	  	 tim--;
      	 WDR(); //this prevents a timout on enabling
  	 }
}

//Light Red
void LightRedOff()
{
 	 PORTC |= BIT(Light_Red);
}
void LightRedOn()
{
 	 PORTC &= ~BIT(Light_Red);
}
void LightRedFlashOff()
{
 	 LightRedOff();
 	 gFlashRed = FALSE;
}
void LightRedFlashOn()
{
 	 gFlashRed = TRUE;
}
//Light Green
void LightGreenOff()
{
 	 PORTC |= BIT(Light_Green);
}
void LightGreenOn()
{
 	 PORTC &= ~BIT(Light_Green);
}
void LightGreenFlashOff()
{
 	 LightGreenOff();
 	 gFlashGreen = FALSE;	 
}
void LightGreenFlashOn()
{
 	 gFlashGreen = TRUE;
}
void ADCtrlOff()
{
 	 PORTD &= ~BIT(Ctrl_AD);
}
void ADCtrlOn()
{
 	 PORTD |= BIT(Ctrl_AD);
}
//Power 5V
void Power5VOff()
{
 	 PORTC &= ~BIT(Power_5V);
}
void Power5VOn()
{
 	 PORTC |= BIT(Power_5V);
}
//Power DSP
void PowerDspOff()
{
  	 PORTD &= ~BIT(Power_Dsp);
}
void PowerDspOn()
{
 	 PORTD |= BIT(Power_Dsp);
}
//Reset DSP
void ResetDspOn()
{
 	 PORTD &= ~BIT(Reset_Dsp); 
}
void ResetDspOff()
{
 	 PORTD |= BIT(Reset_Dsp);
} 
//Trans_Positive
void TransPositiveOff()
{
 	 PORTD |= BIT(Trans_Negative);
 	 PORTD |= BIT(Trans_Positive);
}
void TransPositiveOn()
{
 	 PORTD |= BIT(Trans_Negative);
 	 PORTD &= ~BIT(Trans_Positive);
}
//Trans_Negative
void TransNegativeOff()
{
 	 PORTD |= BIT(Trans_Positive);
 	 PORTD |= BIT(Trans_Negative);
}
void TransNegativeOn()
{
 	 PORTD |= BIT(Trans_Positive);
 	 PORTD &= ~BIT(Trans_Negative);
}
//Ctrl Speaker
void SpeakerOff()
{
 	 PORTC &=  ~BIT(Ctrl_Speaker);
}
void SpeakerOn()
{
 	 PORTC |=  BIT(Ctrl_Speaker);
}
void UartTxLow()
{	 
 	 PORTD &=  ~BIT(Uart_Tx);
}
void UartTxHigh()
{
 	 PORTD |=  BIT(Uart_Tx);
}
void SetUartTxInput()
{
 	 DDRD &= ~BIT(Uart_Tx);
}
void SetUartTxOutput()
{
 	 DDRD |= BIT(Uart_Tx);
}
void StateDoorPullUpOff()
{
 	 PORTD &= ~BIT(State_Door);
}
void StateDoorPullUpOn()
{
 	 PORTD |= BIT(State_Door);
}
void StateOpenPullUpOff()
{
 	 PORTB &= ~BIT(State_Open);
}
void StateOpenPullUpOn()
{
 	 PORTB |= BIT(State_Open);
}
void StateClosePullUpOff()
{
 	 PORTB &= ~BIT(State_Close);
}
void StateClosePullUpOn()
{
 	 PORTB |= BIT(State_Close);
}

void WgData0Out()
{
 	 PORTC &= ~BIT(Wg_Data0);
	 Delay100US();
	 PORTC |= BIT(Wg_Data0);
	 DelayMS(1);
}
void WgData1Out()
{
 	 PORTC &= ~BIT(Wg_Data1);
	 Delay100US();
	 PORTC |= BIT(Wg_Data1);
	 DelayMS(1);
}	
void WiegandOut(U8 FacCode, U16 IDNum)
{
 	char i;
	U8 tmp8;
	U16 tmp16;
  	U8 ChkEven, ChkOdd;
 	//计算位1-位12的偶校验值
	ChkEven = 0;
	tmp8 = FacCode;
	for (i = 0; i < 8; i++)
	{
	 	if ((tmp8 & 0x80) != 0) ChkEven ^= 1;
		tmp8 <<= 1;
	}
	tmp16 = IDNum;
	for (i = 0; i < 4; i++)
	{
	 	if ((tmp16 & 0x8000) != 0) ChkEven ^= 1;
		tmp16 <<= 1;
	}		
	//计算位13-位24的奇校验值
	ChkOdd = 1;
	for (i = 4; i < 16; i++)
	{
	 	if ((tmp16 & 0x8000) != 0) ChkOdd ^= 1;
		tmp16 <<= 1;
	}
	
	//输出ChkEven
	if (ChkEven == 0) WgData0Out();
	   			else  WgData1Out();
	//输出FacCode
	tmp8 = FacCode;
	for (i = 0; i < 8; i++)
	{
	 	if ((tmp8 & 0x80) == 0) WgData0Out();
		   			   	  else  WgData1Out();
		tmp8 <<= 1;
	}
	//输出IDNum
	tmp16 = IDNum;
	for (i = 0; i < 16; i++)
	{
	 	if ((tmp16 & 0x8000) == 0) WgData0Out();
		   		   	 	else  WgData1Out();
		tmp16 <<= 1;
	}
	//输出ChkOdd
	if (ChkOdd == 0) WgData0Out();
	   		   else  WgData1Out();
}
/******************蜂鸣器鸣叫子程序void Buzzer(U16 tim)**********************/
/*功能：产生一定时间的鸣叫***************************************************/
/*参数：鸣叫毫秒数***********************************************************/
/*返回值：无*****************************************************************/
void Buzzer(U16 tim, U8 cnt)          //蜂鸣器鸣叫
{
 	 U8 i;
	 for (i = 0; i < cnt; i++)
	 {
	  	 SpeakerOn();
	 	 DelayMS(tim);
	 	 SpeakerOff();
		 DelayMS(tim);
	 }
}
U8 IsSensorOn()
{
   	 if ((PINB & 8) != 0)//BIT(Sensor_Status)
	 {
	  	 return TRUE;
	 }
	 else
	 {
	     return FALSE;
	 }
}
U8 IsDoorClosed()
{
     if ((PIND & BIT(State_Door)) == 0)
	 {
	  	 return TRUE;
	 }
	 else
	 {
	  	 return FALSE;
	 }
}
U8 IsStateOpen()
{
   	 if ((PINB & BIT(State_Open)) == 0)
	 {
	  	 return TRUE;
	 }
	 else
	 {
	  	 return FALSE;
	 }
}
U8 IsStateClose()
{
   	 if ((PINB & BIT(State_Close)) != 0)
	 {
	  	 return TRUE;
	 }
	 else
	 {
	  	 return FALSE;
	 }
}

U8 DoorOpenProcess()
{
 	 U8 i;
	 
	 if (IsStateOpen() == TRUE) return TRUE;
	 
	 TransPositiveOn();
	 for (i = 0; i < 10; i++)
	 {
	  	 if (IsStateOpen() == TRUE) break;
		 DelayMS (100);
	 }
	 TransPositiveOff();
	 DelayMS (50);
	 	 
	 if (i < 100) return TRUE;
	 	   	 else return FALSE;
}
U8 DoorCloseProcess()
{
 	 U8 i;
	 
	 if (IsStateClose() == TRUE) return TRUE;
	
	 SpeakerOn();
	 TransNegativeOn();
	 for (i = 0; i < 10; i++)
	 {
	  	 if (IsStateClose() == TRUE) break;
		 DelayMS (100);
	 }
	 TransNegativeOff();
	 SpeakerOff();
	 
	 DelayMS (50);
	 
	 if (i < 100) return TRUE;
	 	   	 else return FALSE;
}

void TxByte(U8 byte)
{
 	 UDR = byte;
 	 while (!(UCSRA & 0x40));//查询发送结束
	 UCSRA |= 0x40;	  		//清除发送结束标志
}

U8 IsValidUser(U8 UserRole)
{
     if ((UserRole == 1) || (UserRole == 2) || (UserRole == 3)) return TRUE;
	 			  	   	  		   	  	   			 	   else return FALSE;
}

U8 IsMasterUser(U8 UserID)
{
     if ((UserID == 1) || (UserID == 2) || (UserID == 3)) return TRUE;
	 			   	 		   	  	   		  	    else  return FALSE;
}	 


/******************串口发送子程序bit SendUART(U8 Scnt,U8 Rcnt,U8 Delay)******/
/*功能：向DSP发送数据********************************************************/
/*参数：Scnt发送字节数；Rcnt接收字节数； Delay延时等待数*********************/
/*返回值：TRUE 成功；FALSE 失败*******************************************************/
U8 TxAndRsCmd(U8 Scnt, U8 Rcnt, U8 Delay)
{

   	 U8  i, j, CheckSum;
	 U8  bCatchOn;
	 U32 RsTimeCnt;
	 
	 TxByte(CMD_HEAD);		//标志头
	 
	 CheckSum = 0;
	 for (i = 0; i < Scnt; i++)
	 {
	  	 TxByte(gTxBuf[i]);		 
		 CheckSum ^= gTxBuf[i];
	 }
	
	 TxByte(CheckSum);
	 TxByte(CMD_TAIL);
	  
	 gRsLength = 0;
	 RsTimeCnt = Delay * 120000;
	 
	 bCatchOn = FALSE;
	 while (gRsLength < Rcnt && RsTimeCnt > 0)
	 {
	    WDR();
		RsTimeCnt--;
		   
		if (gFlagInput == 1)
		{
		  	if ((gFlag & BIT(Key_Add)) != 0) break;
			if ((gFlag & BIT(Key_Del)) != 0) break;
			if ((gFlag & BIT(Key_DelAll)) != 0) break;
	//		if ((gFlag & BIT(State_Door)) != 0) break;
		}
		//检测bCatchOn下降沿跳变
		if (IsSensorOn() == TRUE)
		{
		    bCatchOn = TRUE;
		}
		else if (bCatchOn == TRUE)
		{
		    bCatchOn = FALSE;
			Buzzer(Beep_Short, 1);
		}
	 }
	 if (gFlagInput == 1) return ACK_GO_OUT;
	 if (gRsLength != Rcnt)return ACK_TIMEOUT;
	 if (gRsBuf[0] != CMD_HEAD) return ACK_FAIL;
	 if (gRsBuf[Rcnt - 1] != CMD_TAIL) return ACK_FAIL;
	 if (gRsBuf[1] != (gTxBuf[0])) return ACK_FAIL;
	 
	 CheckSum = 0;
	 for (j = 1; j < gRsLength - 1; j++) CheckSum ^= gRsBuf[j];
	 if (CheckSum != 0) return ACK_FAIL;
	  
	 gSensorOn = FALSE;	 
	 return ACK_SUCCESS;
}	 
/******************指纹比对子程序U8 CompFinger(void)*************************/
/*功能：采集指纹与库内指纹比对,方式为1：N ***********************************/
/*参数：无 ******************************************************************/
/*返回值：TRUE成功；FALSE失败*******************************************/
U8 CompFinger(U8 Total)
{
   	 U8 m;
	 
	 gTxBuf[0] = CMD_MATCH;
	 gTxBuf[1] = 0;
	 gTxBuf[2] = 0;
	 gTxBuf[3] = 0;
	 gTxBuf[4] = 0;	 
	  	 
	 LightGreenFlashOn();		 //绿灯闪烁
	 
	 m = TxAndRsCmd(5, 8, Tout_Cnt);
//	 Buzzer(Beep_Short, 1);
	 LightGreenFlashOff();
	 
	 DelayMS(500);
	  
	 if (m == ACK_TIMEOUT)
	 {	  	  
	    return ACK_TIMEOUT;
	 }
	 else if (m == ACK_GO_OUT)
	 {
	  	  return ACK_GO_OUT;
	 }
	 else if (Total == 0)
	 {
	  	  return ACK_SUCCESS;
	 }		 
	 else if ((m == ACK_SUCCESS) && (gRsBuf[3] != 0) && (IsValidUser(gRsBuf[4]) == TRUE))
	 {	  	  
	  	  LightGreenOn();	 //绿灯闪烁
		  Buzzer(Beep_Short, 3);		 //“笛笛笛”三声短响
		  LightGreenOff();
		   
	  	  return ACK_SUCCESS;
	 }
	 else
	 {
	      
	  	  if (gFlagInput != 1)
		  {
		      LightRedOn();		   //红灯长亮
		  
		  	  Buzzer(Beep_Long, 3);//“笛笛笛”三声长响
		  		  
		  	  LightRedOff();
		  }
		  
	  	  return ACK_FAIL;
	 }  
}

/******************管理员确认子程序U8 VerityManager(void)********************/
/*功能：采集指纹与库内指纹比对,方式为1：N ***********************************/
/*参数：无 ******************************************************************/
/*返回值：TRUE 成功；FALSE失败*******************************************************/
U8 VerifyManager(U8 Total, U8 Option)
{
    U8 m;
	
	gTxBuf[0] = CMD_MATCH;
	gTxBuf[1] = 0;
	gTxBuf[2] = 0;
	gTxBuf[3] = 0;
	gTxBuf[4] = 0;
	
	LightRedFlashOn();		//红灯闪烁
	LightGreenFlashOn();	//绿灯闪烁
	Buzzer(Beep_Short, 1);
	
	m = ACK_SUCCESS;
	if (Total > 0)
	{
	    m = TxAndRsCmd(5, 8, Tout_Cnt);
	}

	LightRedFlashOff();
	LightGreenFlashOff();
	
	if (m == ACK_TIMEOUT) 
	{
	    return ACK_TIMEOUT;
	}
	else if (m == ACK_GO_OUT)
	{
	 	return ACK_GO_OUT;
	}
	else if ((m == ACK_SUCCESS) && (IsMasterUser(gRsBuf[3]) == TRUE))
	{	
		 LightGreenOn();   
	   	 Buzzer(Beep_Short, 3); //“笛笛笛”三声短响
		 LightGreenOff();
		 return ACK_SUCCESS;
	}
	else
	{
	 	 LightRedOn();			 //红灯亮
		 if (Option == 1)//添加用户验证
		 {
	 	  	  Buzzer(Beep_Long, 1);   //“笛”一声长响
		 }
		 else if (Option == 2)//删除用户验证
		 {
		  	  Buzzer(Beep_Long, 1);   //“笛笛笛”三声长响
		 }
		 else if (Option == 3)//删除管理员验证
		 {
		  	  Buzzer(Beep_Long, 1);  //“笛”一声短响
		 }
	
		 LightRedOff();
		 
		 return ACK_FAIL;
	}
}
		 
/******************指纹存储子程序**********************/
/*功能：存储用户指纹 ********************************************************/
/*参数：k 用户指纹存储编号**************************************************/
/*返回值：TRUE成功，FALSE失败**************************/
U8 AddUser(U8 k)
{
   	U8 m;
	
	DelayMS (50);
	
	gTxBuf[0] = CMD_ADD_1;
	gTxBuf[1] = 0;
	gTxBuf[2] = k;
	gTxBuf[3] = 3;
	gTxBuf[4] = 0;	
	LightGreenFlashOn();
	
	m = TxAndRsCmd(5, 8, Tout_Cnt);
	
//	Buzzer(Beep_Short, 1);
	

	LightGreenFlashOff();
	
	DelayMS(Delay_Cap);
	
	if (m == ACK_TIMEOUT)
	{
	    return ACK_TIMEOUT;
	}
	else if (m == ACK_GO_OUT)
	{
	 	return ACK_GO_OUT;
	}
	else if (m == ACK_SUCCESS && gRsBuf[4] == ACK_SUCCESS)
	{
	   	 LightGreenFlashOn();  //绿灯闪烁
//		 Buzzer(Beep_Short, 1);//“笛”一声短响
		 
		 gTxBuf[0] = CMD_ADD_2;
		 
		 m = TxAndRsCmd(5, 8, Tout_Cnt);
		 
//		 Buzzer(Beep_Short, 1);
		 LightGreenFlashOff();
		 
		 DelayMS(Delay_Cap);
		 
		 if (m == ACK_TIMEOUT)
		 {
		     return ACK_TIMEOUT;	 
		 }
		 else if (m == ACK_GO_OUT)
		 {
	 	  	 return ACK_GO_OUT;
		 }
		 else if (m == ACK_SUCCESS && gRsBuf[4] == ACK_SUCCESS)
		 {
	   	  	 LightGreenFlashOn();  //绿灯闪烁
//		 	 Buzzer(Beep_Short, 1);//“笛”一声短响		 	 
	 
		 	 gTxBuf[0] = CMD_ADD_3;
			
			 m = TxAndRsCmd(5, 8, Tout_Cnt);
			 
//			 Buzzer(Beep_Short, 1);
			 LightGreenFlashOff();
			 
			 DelayMS(Delay_Cap);
			 DelayMS(Delay_Cap);
		 
		 	 if (m == ACK_TIMEOUT)
			 {
			     return ACK_TIMEOUT;
			 }
			 else if (m == ACK_GO_OUT)
			 {
	 		  	 return ACK_GO_OUT;
			 }
		 	 else if (m == ACK_SUCCESS && gRsBuf[4] == ACK_SUCCESS)
		 	 { 
			   	 LightGreenOn();
				 Buzzer(Beep_Short, 3);	//“笛笛笛”三声短响
				 LightGreenOff();
				 
				 return ACK_SUCCESS;
			 }
		 }
 	}
	//失败
	LightRedOn();
	Buzzer(Beep_Long, 3);//“笛笛笛”三声长响
	LightRedOff();
	return ACK_FAIL;
}

/******************查询用户总数子程序******************/
/*功能：查询用户总数 ********************************************************/
/*参数 无 *******************************************************************/
/*返回值：用户总数 0xFF:失败****************************************************************/
U8 GetUserCount()
{
   	U8 m;
	
	gTxBuf[0] = CMD_USER_CNT;
	gTxBuf[1] = 0;
	gTxBuf[2] = 0;
	gTxBuf[3] = 0;
	gTxBuf[4] = 0;	
	
	m = TxAndRsCmd(5, 8, 1);
		
	if (m == ACK_SUCCESS && gRsBuf[4] == ACK_SUCCESS)
	{
	    return gRsBuf[3];
	}
	else
	{
	 	return 0xFF;
	}
}

void ClearAllUser()
{
 	U8 m;
	
	gTxBuf[0] = CMD_DEL_ALL;
	gTxBuf[1] = 0;
	gTxBuf[2] = 0;
	gTxBuf[3] = 0;
	gTxBuf[4] = 0;
	
	m = TxAndRsCmd(5, 8, 5);
	
	if (m == ACK_SUCCESS && gRsBuf[4] == ACK_SUCCESS)
	{	    
		LightGreenOn();
		Buzzer(300, 1);
		LightGreenOff();		
	}
	else
	{
	 	LightRedOn();
		Buzzer(300, 1);
		LightRedOff();
	}
}
/******************清除普通用户子程序*******************/
/*功能： 删除普通用户********************************************************/
/*参数 cnt用户总数 *******************************************************************/
/*返回值：TRUE 成功 FALSE失败 ******************************************************/
U8 ClearCommonUser(U8 cnt)
{
    U8 i, m;
	
	gTxBuf[0] = CMD_DEL;
	gTxBuf[1] = 0;
	gTxBuf[2] = 0;
	gTxBuf[3] = 0;
	gTxBuf[4] = 0;
	
	Buzzer(Beep_Long, 3);		//“笛笛笛”三声长响
	
	for (i = 4; i <= USER_MAX_CNT; i++)
	{
	 	gTxBuf[2] = i;
	
		m = TxAndRsCmd(5, 8, 2);
		DelayMS(20);
	
		if (m != ACK_SUCCESS || gRsBuf[4] != ACK_SUCCESS) break;		
	}
	if (i > cnt)//成功
	{
		Buzzer(Beep_Short, 3); 	 //“笛笛笛”三声短响
		return ACK_SUCCESS;
	}
	else
	{
	 	return ACK_SUCCESS;
	}
}	

/******************清除管理员子程序*******************/
/*功能： 删除管理员用户********************************************************/
/*参数 cnt用户总数 *******************************************************************/
/*返回值：TRUE 成功 FALSE失败 ******************************************************/
U8 ClearManager(U8 cnt)
{
    U8 i, m;
	
	gTxBuf[0] = CMD_DEL_ALL;
	gTxBuf[1] = 0;
	gTxBuf[2] = 0;
	gTxBuf[3] = 0;
	gTxBuf[4] = 0;
	
	Buzzer(Beep_Long, 3);		//“笛笛笛”三声长响
	
	m = TxAndRsCmd(5, 8, 2);
	DelayMS(20);
	
	Buzzer(Beep_Short, 3); 	 //“笛笛笛”三声短响
/*	if (cnt <= 3)
	{
	    for (i = 1; i <= cnt; i++)
	    {
	 	    gTxBuf[2] = i;
		
		    m = TxAndRsCmd(5, 8, 2);
		    DelayMS(10);
		
		    if (m != ACK_SUCCESS || gRsBuf[4] != ACK_SUCCESS) break;
		}		
		if (i > cnt)//成功
		{
		    Buzzer(Beep_Short, 3); 	 //“笛笛笛”三声短响
			LightGreenFlashOff();

			return ACK_SUCCESS;
		}
	}
*/	
	return ACK_SUCCESS;
}

U8 NormalProcess()
{
    U8 m, Total;
		
	Total = GetUserCount();	
	if (Total == 0xFF) return 0xFF;
	
	Buzzer(Beep_Short, 1);				 //“笛”一声短响
	
	while (1)
	{
	 	m = CompFinger(Total);
		
		if (m == ACK_TIMEOUT)
		{		   	 
		    return ACK_TIMEOUT;
		}
		else if (m == ACK_GO_OUT)
		{
		 	return ACK_GO_OUT;
		}
		else if (m == ACK_SUCCESS || gFlagInput == 1)
		{
		    break;
		}
	}
	
	if (m == ACK_SUCCESS)
	{
	 	if (Total != 0) WiegandOut (1, (U16)gRsBuf[3]);
	   
	    return ACK_SUCCESS;
	}	
	else
	{
	    return ACK_FAIL;
	}
}	
	
U8 KeyProcess()
{
 	U8 i, m, Total;
	U8 Flag1;

	Flag1 = gFlag;
	gFlag &= ~BIT(Flg_Add);
   	gFlag &= ~BIT(Flg_Del);
   	gFlag &= ~BIT(Flg_DelAll);
//	gFlag &= ~BIT(Flg_Door);

	gFlagInput = 0;
	if ((Flag1 & BIT(Key_Add)) != 0)
	{
	    	    
		Total = GetUserCount();
 		
//	 	if (Total == 0xFF);
		
		m = ACK_SUCCESS;
		while (1)
		{
		 	if (Total > 0)
			{
		        m = VerifyManager(Total, 1);
				if (m == ACK_TIMEOUT)
				{
			         return ACK_TIMEOUT;
				}
				else if (m == ACK_GO_OUT)
				{
	 			 	 return ACK_GO_OUT;
		    	}
				if (m == ACK_SUCCESS || gFlagInput == 1)
				{
			       	 break;
				}
			}
			else
			{
			 	//第一次
	    		LightRedOn();
	    		LightGreenOn();		
				Buzzer(Beep_Short, 1);
				DelayMS(300);
				LightRedOff();
				LightGreenOff();
				break;
			}
		}
		if (m == ACK_SUCCESS)
		{
		    while (Total < USER_MAX_CNT)
			{		 	
		 		m = AddUser(Total + 1);				
				if (m == ACK_TIMEOUT)
				{
				    return ACK_TIMEOUT;
				}
				else if (m == ACK_GO_OUT)
				{
	 				return ACK_GO_OUT;
			    }			
				if (gFlagInput == 1)
				{
			        break;
				}
				Total = GetUserCount();
			}
		}
		return ACK_GO_OUT;
	}
	else if ((Flag1 & BIT(Key_Del)) != 0)
	{
		Total = GetUserCount();		
//		if (Total == 0xFF);
		while (1)
		{
		 	m = VerifyManager(Total, 2);
			
			if (m == ACK_TIMEOUT)
			{
			    return ACK_TIMEOUT;
			}
			else if (m == ACK_GO_OUT)
			{
	 		 	return ACK_GO_OUT;
			}
			if (m == ACK_SUCCESS || Total == 0 || gFlagInput == 1)
			{
			    break;
			}
		}
		ClearCommonUser(Total);			
		
		Total = GetUserCount();		
//		if (Total == 0xFF);		
		while (1)
		{
		 	m = VerifyManager(Total, 3);
			if (m == ACK_TIMEOUT)
			{
			    return ACK_TIMEOUT;
			}
			else if (m == ACK_GO_OUT)
			{
	 		 	return ACK_GO_OUT;
			}
			else if (m == ACK_SUCCESS || Total == 0)
			{
		        break;
			}
		}
		ClearManager(Total);
		
		return ACK_TIMEOUT;//引发进入休眠
	}
	else if ((Flag1 & BIT(Key_DelAll)) != 0)
	{
	 	ClearAllUser();
		
		return ACK_TIMEOUT;//引发进入休眠 
	}	
	else
	{	
	 	return ACK_TIMEOUT;//引发进入休眠
	}		
}
//=========初始化程序段===========
void Rst_Dsp()       //DSP复位
{
// 	UartTxLow();
    ResetDspOn();//PORTD &=~ BIT(Ctr_Dsp_Reset);                       //DSP复位
	DelayMS(40);
	ResetDspOff();//PORTD |=  BIT(Ctr_Dsp_Reset);	
	DelayMS(90);
//	UartTxHigh();
	DelayMS(120);
}

void Init_Port()      //端口初始化
{
 	PORTB = 0xFF;
 	DDRB  = 0x00;
 	PORTC = 0x5B; //m103 output only
 	DDRC  = 0x3F;
 	PORTD = 0xDF;//0xFF;
 	DDRD  = 0xF6;
}

//UART0 initialisation
// desired baud rate: 19200
// actual: baud rate:18750 (2.4%)
// char size: 8 bit
// parity: Disabled
void Init_Uart0(void)         //串口初始化
{
 	SPCR  = 0x00; //set SPI status
 	UCSRB = 0x00; //disable while setting baud rate
 	UCSRA = 0x00;
 	UCSRC = 0x86;
 	UBRRL = 0x13; //set baud rate lo
 	UBRRH = 0x00; //set baud rate hi
 	UCSRB = 0x98;
}

//TIMER0 initialisation - prescale:256
// WGM: Normal
// desired value: 10mSec
// actual value:  9.984mSec (0.2%)
void Init_Timer0()      //定时器0初始化?
{
 	TCCR0 = 0x00; //stop
 	TCNT0 = 0x16; //set count
 	TCCR0 = 0x04; //start timer	
}
#pragma interrupt_handler timer0_ovf_isr:10
void timer0_ovf_isr()    // 定时器0初始化，10ms定时
{
 	TCNT0 = 0x16; //reload counter value
 	gKeyTemp1 = (PINB | 0xF8);
	
 	if (gKeyTemp1 != 0xFF)
 	{
  	 	if (gKeyTemp1 == gKeyTemp2)
  		{
   		 	if((gKeyTemp1 | 0xFE) == 0xFE) gFlag |= BIT(Flg_Add);  //注册按键
			if((gKeyTemp1 | 0xFD) == 0xFD) gFlag |= BIT(Flg_Del);   //删除按键
			if((gKeyTemp1 | 0xFB) == 0xFB) gFlag |= BIT(Flg_DelAll);//全部删除按键
			//if((gKeyTemp1 | 0xF7) == 0xF7) gFlag |= BIT(Flg_Door);  //门状态传感器
  			//if((gKeyTemp1 | 0xEF) == 0xEF) gFlag |= BIT(Flg_Close); //关门到位传感器
   			//if((gKeyTemp1 | 0xDF) == 0xDF) gFlag |= BIT(Flg_Open);  //开门到位传感器
			
			
			gFlagInput = 1;			
  		}
 	}
 	gKeyTemp2 = gKeyTemp1;
	
	if (gTimer0Cnt == 0)
	{
	    if (gFlashRed == TRUE)
		{
            if (IsSensorOn() == TRUE)
			{
			    if (gIsLightOn == TRUE)
			    {		   
		            LightRedOn();
				}
			    else
			    {
			        LightRedOff();
				}
			}
			else
			{
			 	LightRedOff();
			}
		}
		if (gFlashGreen == TRUE)
		{
            if (IsSensorOn() == TRUE)
			{
			    if (gIsLightOn == TRUE)
			    {		   
		            LightGreenOn();
				}
			    else
			    {
			        LightGreenOff();
				}
			}
			else
			{
			 	LightGreenOff();
			}
		}	
		
		if (gIsLightOn == TRUE) gIsLightOn = FALSE;
		   			   	  else  gIsLightOn = TRUE;
	}
	//门状态判断
	if (gDoorCloseEnable == TRUE)
	{
	   	if (IsDoorClosed () == TRUE)
		{
	       	if (IsStateClose () == FALSE)
			{
			    gDoorCloseCnt ++;
				if (gDoorCloseCnt >= 80)
				{
	       	   	   	gDoorClosed = FALSE;
	       		   	SpeakerOn();
	   	   			TransNegativeOn();
				}
			}
			else 
			{
			 	gDoorCloseCnt = 0;
			}
		}
		else
		{
		 	gDoorCloseCnt = 0;
		}
		if (IsStateClose () == TRUE)
		{
	   	   	if (gDoorClosed == FALSE)
	   		{
	      	   gDoorClosed = TRUE;
		  	   SpeakerOff();
		  	   TransNegativeOff();
	   		}
		}
	}
	
	gTimer0Cnt++;
	if (gTimer0Cnt > Flash_Cnt) gTimer0Cnt = 0;	
}

//TIMER1 initialisation - prescale:1024
// WGM: 0) Normal, TOP=0xFFFF
// desired value: 1Sec
// actual value:  1.000Sec (0.0%)
void Init_Timer1()        //定时器1初始化
{
  	TCCR1B = 0x00; //stop
 	TCNT1H = 0xE9; //setup
 	TCNT1L = 0x1D;
 	OCR1AH = 0x16;
	OCR1AL = 0xE3;
	OCR1BH = 0x16;
 	OCR1BL = 0xE3;
 	ICR1H  = 0x16;
 	ICR1L  = 0xE3;
 	TCCR1A = 0x00;
 	TCCR1B = 0x05; //start Timer
}
#pragma interrupt_handler int1_isr:3
void int1_isr(void)
{
}

#pragma interrupt_handler timer1_ovf_isr:9
void timer1_ovf_isr(void)      //定时器1中断服务程序
{
 	//TIMER1 has overflowed
 	TCNT1H = 0xE9; //reload counter high value
 	TCNT1L = 0x1D; //reload counter low value
}

void Uart0_Init()
{
 	UCSRB = 0x00; //disable while setting baud rate
 	UCSRA = 0x00;
 	UCSRC = 0x86;
 	UBRRL = 0x13; //set baud rate lo
 	UBRRH = 0x00; //set baud rate hi
 	UCSRB = 0x98;
}

#pragma interrupt_handler uart0_rx_isr:12
void uart0_rx_isr(void)     //串口中断服务程序
{
  	unsigned char temp,temp1;
  	CLI();
//  if(UDR==0xAA) gRsLength=0;LUU
  	gRsBuf[gRsLength++] = UDR;
  	SEI();
}

//call this routine to initialise all peripherals
void Init_Devices()        //设备初始化
{
 	//stop errant interrupts until set up
 	CLI(); //disable all interrupts
 	Init_Port();
	
	gTimer0Cnt = 0;
	gKeyTemp1 = 0xFF;
	gKeyTemp2 = 0xFF;
	gFlag = 0;
	gSensorOn = FALSE;
	
// watchdog_init();//初始化看门狗
   	Init_Uart0();
   	Init_Timer0();
   	Init_Timer1();
   	MCUCR = 0x00;
   	GICR  = 0x00;
   	TIMSK = 0x05; //timer interrupt sources
   	SEI(); //re-enable interrupts
   	//all peripherals are now initialised
	
	LightRedOff();
	LightGreenOff();
	LightRedFlashOff();
	LightGreenFlashOff();
	gIsLightOn = TRUE;	
	gDoorClosed = TRUE;
	gDoorCloseCnt = 0;
	gDoorCloseEnable = TRUE;
	
}

void Sleep_Board(U8 bPullUpOn)
{	
 	//close all IOs
	PORTB = 0;
	PORTC = 0;
	PORTD = 0;
	//disable UART
	UCSRB = 0;
	
	LightRedOff();
	LightGreenOff();
	if (bPullUpOn == TRUE)
	{
	    StateDoorPullUpOn();
	}
	else
	{
	 	StateDoorPullUpOff();	    
	}
	StateOpenPullUpOff();
	StateClosePullUpOff();
		 	
	SpeakerOff();
	PowerDspOff();
	Power5VOff();
	 
	//power down mode (SM2..0 = 010)
	MCUCR &= ~BIT(SM0);
	MCUCR |= BIT(SM1);
	MCUCR &= ~BIT(SM2);
	MCUCR |= BIT(SE);
	
	asm("SLEEP");
	
	//跳出休眠，置上拉电阻
	StateOpenPullUpOn();
	StateClosePullUpOn();
}

void Check_Vin()
{
    U8 i, ADValue;
	U8 ADCHTmp;
	
	//ADMUX  选择AD通道6,选择内部参考电压2.56V,
	//| REFS1 | REFS0 | ADLAR | - | MUX3 | MUX2 | MUX1 | MUX0 |
	//	  1       1       1     0     0      1      1      0
    ADMUX = 0xE6;
	//ADCSRA 置ADEN位,选择单转换,关闭AD中断,分频 = 8
	//| ADEN | ADSC | ADFR | ADIF | ADIE | ADPS2 | ADPS1 | ADPS0 |
	//    1      0      0      0      0      0       1       1
	ADCSRA = 0x83;
	
	ADCtrlOn();
		
	DelayMS(10);
	//启动AD
	ADCSRA |= 0x40;
	//判断AD是否结束
	for (i = 0; i < 50; i++)
	{
	 	if ((ADCSRA & 0x10) != 0) break;
		DelayMS(1);
	}
	DelayMS(10);
	
	ADCHTmp = (U8)ADCH;
	if (i >= 50 || ADCHTmp < (U8)0x9A) Buzzer (1500, 1);//5V为(0x95)
	//读ADH
	ADValue = ADCH;	
//	EEPROM_WRITE (0x0000, ADValue);
	ADValue = ADCL;
//	EEPROM_WRITE (0x0001, ADValue);
	
	
	//关闭AD，清ADEN位
	ADCSRA &= 0x7F;
	
	ADCtrlOff();
}	

void main()
{
 	U8 i, m;
	U8 bCatchOn;
	U32 RsTimeCnt;

 	Init_Devices();
	
	Check_Vin();
	
	Buzzer(Beep_Short, 1);				 //“笛”一声短响
	PowerDspOn();
	DelayMS(100);
	
	
	for (i = 0; i < 5; i++)
	{ 		
		WDR();
		//PowerDspOn();
		
		Rst_Dsp();		
		
		Power5VOn();
		gFlagInput = 0;		
		
		//如果添加/删除键/全部删除键都按下则处于恒上电状态
		if ((PINB & 0x07) == 0)
		{
		   	LightGreenFlashOn();	
		   	while ((PINB & 0x07) == 0)
			{
			 	//等待休眠命令
				bCatchOn = FALSE;
				gRsLength = 0;
				RsTimeCnt = 120000;
				while (gRsLength < 8 && RsTimeCnt > 0)
				{
				    WDR();
					RsTimeCnt--;		
					//检测bCatchOn下降沿跳变
					if (IsSensorOn() == TRUE)
					{
		    		   	bCatchOn = TRUE;
					}
					else if (bCatchOn == TRUE)
					{
		    		 	bCatchOn = FALSE;
						Buzzer(Beep_Short, 1);
					}
				}
				if (gRsLength >= 8)
				{
				    if (gRsBuf[1] == CMD_LP_MODE) goto full_sleep_mode;
				}
			}
		
			LightGreenFlashOff();
		}
		
		m = NormalProcess();
		
		if (m != 0xFF) break;
		
		//PowerDspOff();
		Power5VOff();
		DelayMS(150);
		WDR(); 	
	}	
	
	if (m == ACK_SUCCESS)
	{
	 	//开锁
		PowerDspOff();	
		Power5VOff();
		gDoorCloseEnable = FALSE;	 
		DoorOpenProcess();
		if (IsDoorClosed() == TRUE)
		{
	   	   DelayMS (700);
		}
	}	
	else if (m == ACK_TIMEOUT)
	{
	 	//无操作		
	}
	else if (m == ACK_GO_OUT)
	{			
		//读键盘操作
		while (1)
		{
		 	Rst_Dsp();
			
	 	 	m = KeyProcess();
			if (m == ACK_TIMEOUT)
			{
			   	 break;
			}			
		}
	}
	
	//休眠
sleep_mode:
	
	gDoorCloseEnable = FALSE;	 	   	
	while (IsStateClose() != TRUE)
	{
	 	GICR  = 0x80;
	 	Sleep_Board(TRUE);
		
		if (IsDoorClosed() == TRUE)
		{	
			GICR = 0x00;	   
		   	DelayMS (700);
		    if (IsDoorClosed() == TRUE)
		    {
	       	    DoorCloseProcess();
		    }
		}
	}
full_sleep_mode:
	GICR  = 0x00;//关Int1中断
	while (1)
	{
	 	Sleep_Board(FALSE);
	}			
}		
		
		   	     

