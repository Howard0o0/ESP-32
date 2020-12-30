
//--------------  Radio Map -----------------
#define FEA_LEN          193

#define CMD_HEAD         0xF5
#define CMD_TAIL         0xF5
#define CMD_CH           0
#define TIMER_RCV_DELAY  0x01
#define TIMER_WAIT_DSP   0x02
#define TIMER_REG_RW	 0x03

#define EVENT_TX         0x01
#define EVENT_RCV        0x02

#define ST_SUCCESS			0x00
#define ST_FAIL				0x01
#define ST_FULL				0x04
#define ST_NO_USER			0x05
#define ST_USER_OCCUPIED	0x06
#define ST_FINGER_OCCUPIED	0x07
#define ST_TIMEOUT			0x08

#define ST_IMAGE_RAW	 0x20

#define ST_SYSMASTER	 0x03
#define ST_LIMMASTER	 0x02
#define ST_COMMUSER		 0x01

#define ST_BAUD_9600     1
#define ST_BAUD_19200    2
#define ST_BAUD_38400    3
#define ST_BAUD_57600    4
#define ST_BAUD_115200   5
//基本操作
#define RADIO_GET_USER_SUM_DB    0
#define RADIO_REG_DB			 1
#define RADIO_GET_USER_RIGHT_DB	 2
#define RADIO_REG_DELETE_DB		 3
#define RADIO_REG_ALLDEL_DB  	 4
#define RADIO_VERIFY_DB			 5
#define RADIO_IDENTIFY_DB    	 6
#define RADIO_GET_IMAGE			 7
//特征值下传操作	
#define RADIO_FROM_VALUE_DB  	 8 
#define RADIO_FROM_VERIFY	 	 9
#define RADIO_FROM_VERIFY_DB 	10 
#define RADIO_FROM_IDENTIFY_DB	11 
 
//特征值上传操作 
#define RADIO_TO_VALUE_DB		12 
#define RADIO_GET_VALUE		   	13 
//一般性操作

//#define RADIO_TEST_COMM		15
#define RADIO_GET_USER_INFO		14
#define RADIO_SET_LP_MODE		15
#define RADIO_GET_VERSION		16
#define	RADIO_SET_MATCH_LEVEL	17

#define RADIO_GET_LUM			18
#define RADIO_LUM_ADJUST		19

#define RADIO_GET_ONE_RECORD	20
#define RADIO_GET_NEW_RECORD	21
#define RADIO_CLEAR_RECORD		22
#define RADIO_SET_TIME			23
#define RADIO_GET_TIME			24

#define RADIO_SET_REG			25
//------------- Cmd Map ---------------
#define CMD_REG_START_DB		0x01
#define CMD_REG_SECOND_DB       0x02
#define CMD_REG_END_DB			0x03
#define CMD_REG_DELETE_DB		0x04
#define CMD_REG_ALLDEL_DB 		0x05
#define CMD_REG_UPLOAD_DB		0x06

#define CMD_GET_USER_NUMBER_DB 	0x08
#define CMD_GET_USER_SUM_DB    	0x09
#define CMD_GET_USER_RIGHT_DB	0x0A
#define CMD_VERIFY_DB			0x0B                                
#define CMD_IDENTIFY_DB    		0x0C

#define CMD_LUM_ADJUST			0x0F
 
#define CMD_TO_VALUE_DB			0x31

#define CMD_GET_ONE_RECORD		0x38//取单条记录
#define CMD_GET_NEW_RECORD		0x39//取新记录
#define CMD_CLEAR_RECORD		0x3B//清空记录
#define CMD_SET_TIME			0x49//设置系统时间
#define CMD_GET_TIME			0x3C//读取系统时间

#define CMD_FROM_VALUE_DB  		0x41
#define CMD_FROM_VERIFY_DB 		0x42
#define CMD_FROM_IDENTIFY_DB	0x43
#define CMD_FROM_VERIFY  		0x44
#define CMD_FROM_VERIFY_2  		0x46

#define CMD_SET_BAUD			0x21 
#define CMD_SET_REG				0x22
#define CMD_GET_VALUE   		0x23
#define CMD_GET_IMAGE			0x24       
 
#define CMD_TEST_COMM			0x25
#define CMD_GET_VERSION			0x26
#define CMD_SET_MATCH_LEVEL		0x28
#define CMD_PROCESS_IMAGE		0x29
#define CMD_GET_LUM		        0x2F

#define CMD_GET_USER_INFO	    0x2B
#define CMD_SET_LP_MODE			0x2C
#define CMD_SET_ENROLL_MODE		0x2D
#define CMD_GET_LUM				0x2F

#define CMD_UPLOAD_ID_UNLOCK	0x32
#define CMD_UPLOAD_RAND_UNLOCK	0x34
#define CMD_DNLOAD_RAND_UNLOCK	0x48	

#define CMD_PROG_UPGRADE		0xFF		

//---------------  Ack  ----------
#define ACK_REG_START_DB		(CMD_REG_START_DB)
#define ACK_REG_SECOND_DB       (CMD_REG_SECOND_DB)
#define ACK_REG_END_DB			(CMD_REG_END_DB)	
#define ACK_REG_DELETE_DB		(CMD_REG_DELETE_DB)
#define ACK_REG_ALLDEL_DB 		(CMD_REG_ALLDEL_DB)
#define ACK_REG_UPLOAD_DB		(CMD_REG_UPLOAD_DB)
                                
#define ACK_GET_USER_NUMBER_DB 	(CMD_GET_USER_NUMBER_DB)
#define ACK_GET_USER_SUM_DB    	(CMD_GET_USER_SUM_DB)   
#define ACK_GET_USER_RIGHT_DB	(CMD_GET_USER_RIGHT_DB)
#define ACK_VERIFY_DB			(CMD_VERIFY_DB)		
#define ACK_IDENTIFY_DB    		(CMD_IDENTIFY_DB)    	
                                
#define ACK_TO_VALUE_DB		    (CMD_TO_VALUE_DB)

#define ACK_GET_ONE_RECORD		(CMD_GET_ONE_RECORD)
#define ACK_GET_NEW_RECORD		(CMD_GET_NEW_RECORD)
#define ACK_CLEAR_RECORD		(CMD_CLEAR_RECORD)
#define ACK_SET_TIME			(CMD_SET_TIME)	
#define ACK_GET_TIME			(CMD_GET_TIME)
		
#define ACK_FROM_VALUE_DB  		(CMD_FROM_VALUE_DB)  	
#define ACK_FROM_VERIFY_DB 		(CMD_FROM_VERIFY_DB) 	
#define ACK_FROM_IDENTIFY_DB	(CMD_FROM_IDENTIFY_DB)
#define ACK_FROM_VERIFY  		(CMD_FROM_VERIFY)  	
#define ACK_FROM_VERIFY_2  		(CMD_FROM_VERIFY_2) 
                                
#define ACK_SET_BAUD			(CMD_SET_BAUD)		
#define ACK_SET_REG				(CMD_SET_REG)			
#define ACK_GET_VALUE   		(CMD_GET_VALUE)   	
#define ACK_GET_IMAGE			(CMD_GET_IMAGE)		
                                
#define ACK_TEST_COMM			(CMD_TEST_COMM)		
#define ACK_GET_VERSION			(CMD_GET_VERSION)		

#define ACK_SET_MATCH_LEVEL		(CMD_SET_MATCH_LEVEL)	
#define ACK_PROCESS_IMAGE		(CMD_PROCESS_IMAGE)
#define ACK_GET_LUM				(CMD_GET_LUM)
#define ACK_LUM_ADJUST			(CMD_LUM_ADJUST)

#define ACK_GET_USER_INFO		(CMD_GET_USER_INFO)
#define ACK_SET_LP_MODE			(CMD_SET_LP_MODE)
#define ACK_SET_ENROLL_MODE		(CMD_SET_ENROLL_MODE)
#define ACK_GET_LUM				(CMD_GET_LUM)

#define ACK_PROG_UPGRADE		(CMD_PROG_UPGRADE)

#define SECT_LEN				2048
#define PROG_LEN				0x7800

#define REG_NULL				0x00
#define REG_W11					0x01
#define REG_W13					0x02
#define REG_R11   				0x03
#define REG_R13   				0x04

#define DLG_SET					0x01
#define DLG_TEST				0x02
#define DLG_UPGRADE				0x03


