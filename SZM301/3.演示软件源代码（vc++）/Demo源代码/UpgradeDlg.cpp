// UpgradeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "STTStar.h"
#include "upgradeDlg.h"
#include "cmdHead.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int gnBaud;
extern int gnPort;

extern int  gnLenCmd;	
extern int  gnLenAck;
extern BYTE gpCmdDataBuf[SECT_LEN * 2 + 4];
extern BYTE gpAckDataBuf[304 * 304];

WORD   gpProgData[0x10000];
BYTE   gpProgByte[0x20000];

WORD Hex2Dec(char *pBuf)
{
	char chVal;
	WORD wParm;
	int i;

	wParm = 0;
	for (i = 0; i < strlen(pBuf); i++)
	{
		wParm *= 16;	
		chVal = pBuf[i];
		if (isupper(chVal))
		{
			wParm += chVal - 'A' + 10;
		}
		else if(islower(chVal))
		{
			wParm += chVal-'a'+10;
		}
		else
		{
			wParm += chVal-'0';
		}		
	}
	return wParm;
}
/////////////////////////////////////////////////////////////////////////////
// CUpgradeDlg dialog


CUpgradeDlg::CUpgradeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpgradeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUpgradeDlg)
	m_strFilePath = _T("");
	m_strFilePath2 = _T("");
	//}}AFX_DATA_INIT
}


void CUpgradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpgradeDlg)
	DDX_Control(pDX, IDC_BUTTON_BROWSE2, m_btnBrowse2);
	DDX_Control(pDX, IDC_BTNCREATEHEXFILE, m_btnMakeHex);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_UPGRADE, m_btnUpgrade);
	DDX_Control(pDX, IDC_BUTTON_RESTORE, m_btnRestore);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_btnBrowse);
	DDX_Control(pDX, IDC_PROGRESS_UPGRADE, m_progressUpgrade);
	DDX_Text(pDX, IDC_EDIT_FILE_PATH, m_strFilePath);
	DDX_Control(pDX, IDC_MSCOMM1, m_mscomm);
	DDX_Text(pDX, IDC_EDIT_FILE_PATH2, m_strFilePath2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUpgradeDlg, CDialog)
	//{{AFX_MSG_MAP(CUpgradeDlg)
	ON_BN_CLICKED(IDC_BUTTON_UPGRADE, OnButtonUpgrade)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_RESTORE, OnButtonRestore)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BTNCREATEHEXFILE, OnBtncreatehexfile)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE2, OnButtonBrowse2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpgradeDlg message handlers
void CUpgradeDlg::CommSettings(int nBaud)
{
	switch (nBaud)
	{
	case 1:	
		m_mscomm.SetSettings("9600,n,8,1");
		break;
	case 2:
	default:
		m_mscomm.SetSettings("19200,n,8,1");
		break;
	case 3:
		m_mscomm.SetSettings("38400,n,8,1");
		break;
	case 4:
		m_mscomm.SetSettings("57600,n,8,1");
		break;
	case 5:
		m_mscomm.SetSettings("115200,n,8,1");
	}
}

void CUpgradeDlg::TXCommand(BYTE *pCmd,int lenCmd)
{
	VARIANT   varCmd;
	SAFEARRAY safeArray;
	int i, blockNum, blockLeft;
	//有的计算机最大发码长度有限制，所以分成512的小块发送
	blockNum = (lenCmd + 511) / 512;
	blockLeft = lenCmd % 512;
    if (blockLeft == 0) blockLeft = 512;

	for (i = 0; i < blockNum; i++)
	{
		safeArray.fFeatures = VT_VARIANT;
		safeArray.cbElements = 1;
		safeArray.cDims = 1;
		safeArray.cLocks = 0;
		safeArray.pvData = (LPVOID)(pCmd + 512 * i);

		safeArray.rgsabound[0].cElements = 512;
		if (i == blockNum - 1) safeArray.rgsabound[0].cElements = blockLeft;

		safeArray.rgsabound[0].lLbound = 0;

		VariantInit(&varCmd);
		varCmd.vt = 0x2011;//VT_ARRAY|VT_BOOL;
		varCmd.plVal=(long*)&safeArray;
		m_mscomm.SetOutput(varCmd);
	}
}
int CUpgradeDlg::RCVCommand(BYTE *pCmd)
{
	int       lenCmd;
	VARIANT   varCmd;
	SAFEARRAY *pArray;
	
	varCmd = m_mscomm.GetInput();
	pArray = (SAFEARRAY *)varCmd.plVal;
	lenCmd = pArray->rgsabound[0].cElements;
	memcpy(pCmd, pArray->pvData, lenCmd);

	return   lenCmd;
}
void CUpgradeDlg::Init()
{	
	m_nBlkCnt = 0;
	m_fInUpgrading = false;

	//init comm
	m_mscomm.SetRThreshold(1);
	m_mscomm.SetInputLen(200);
	CommSettings(gnBaud);
	
	//init progress bar
	m_progressUpgrade.SetRange(0, 32);
	m_progressUpgrade.SetStep(1);
	m_progressUpgrade.SetPos(0);
	//Init btn
	
	if (m_strFilePath.GetLength() == 0)
	{		
		m_btnUpgrade.EnableWindow(false);
	}
	else
	{
		m_btnUpgrade.EnableWindow(true);
	}
	
	UpdateData(false);
}

BOOL CUpgradeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_mscomm.SetCommPort(gnPort);
/*	if (!m_mscomm.GetPortOpen())
	{
		m_mscomm.SetPortOpen(true);
	}	
*/
	m_mscomm.SetInputMode(1);//byte mode

	Init();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CUpgradeDlg::SendCmd(BYTE nCmdType, BYTE P1, BYTE P2, BYTE P3)
{
	int i;
	BYTE  bChk=0;
	gpCmdDataBuf[0] = CMD_HEAD;
	gpCmdDataBuf[1] = nCmdType;
	gpCmdDataBuf[2] = P1;
	gpCmdDataBuf[3] = P2;
	gpCmdDataBuf[4] = P3;
	gpCmdDataBuf[5] = CMD_CH;
	for (i = 1; i <= 5; i++)
	{
		bChk ^= gpCmdDataBuf[i];
	}
	gpCmdDataBuf[6] = bChk;
	gpCmdDataBuf[7] = CMD_TAIL;
	TXCommand(gpCmdDataBuf, 8);
}
void CUpgradeDlg::OnButtonUpgrade() 
{	
	SendCmd(CMD_GET_USER_SUM_DB, 0, 0, 0);
	m_btnUpgrade.EnableWindow(false);
}

void CUpgradeDlg::SendProgBlock(int nBlkCnt)//每执行一次，升级一块数据
{
	int i;
		
	gpCmdDataBuf[0] = CMD_HEAD;
	gpCmdDataBuf[1] = nBlkCnt;
	//copy prog data to cmd buf
	memcpy(&gpCmdDataBuf[2], &gpProgData[nBlkCnt * SECT_LEN], 2 * SECT_LEN);
	//get chk value
	BYTE bChk = 0;
	for (i = 1; i < SECT_LEN * 2 + 2; i++)
	{
		bChk ^= gpCmdDataBuf[i];
	}
	gpCmdDataBuf[SECT_LEN * 2 + 2] = bChk;
	gpCmdDataBuf[SECT_LEN * 2 + 3] = CMD_TAIL;
	TXCommand(gpCmdDataBuf, SECT_LEN * 2 + 4);
}

BEGIN_EVENTSINK_MAP(CUpgradeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CUpgradeDlg)
	ON_EVENT(CUpgradeDlg, IDC_MSCOMM1, 1 /* OnComm */, OnMscomm, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CUpgradeDlg::OnMscomm() 
{
	int nEvent;

	nEvent = m_mscomm.GetCommEvent();
	if (nEvent == EVENT_RCV)//rcv event
	{
		m_mscomm.SetRThreshold(200);
		SetTimer(TIMER_RCV_DELAY, 100, NULL);
	}	
}

void CUpgradeDlg::OnTimer(UINT nIDEvent) 
{
	int  i,j;
	int  lenAck;
	int  ack;
	BYTE *pBuf;

	switch(nIDEvent)
	{
	case TIMER_RCV_DELAY:
		
		KillTimer(TIMER_RCV_DELAY);
		m_mscomm.SetInputLen(200);
		lenAck = RCVCommand(gpAckDataBuf);	
				
		for (i = 0; i < lenAck; i++)
		{
			if (gpAckDataBuf[i] == CMD_HEAD)
			{
				pBuf = &gpAckDataBuf[i];
				break;
			}
		}

		UpdateData(true);

		if (lenAck - i >= 8)//length ok
		{
			//chk validation test
			BYTE bChk = 0;
			for (j = 1; j <= 5; j++)
			{
				bChk ^= pBuf[j];
			}
			if (pBuf[0] == CMD_HEAD			&&
				pBuf[6] == bChk				&&
				pBuf[7] == CMD_TAIL)

			{
				switch (pBuf[1])
				{
				case ACK_GET_USER_SUM_DB:
					ack = MessageBox ("下载新的程序会冲掉片内原程序，是否继续？", "", MB_OKCANCEL);
					if (ack == IDOK)
					{
						SetTimer(TIMER_WAIT_DSP, 100, NULL);
					}
					else
					{
						Init();
					}
					break;
				case ACK_PROG_UPGRADE:
					if (m_fInUpgrading == true)
					{
						if (gpAckDataBuf[4] == ST_SUCCESS)
						{
							if (gpAckDataBuf[3] == 31)//the last block
							{
								//upgrading is finished
								MessageBox("已成功完成下载!");
								Init();
							}
							else
							{
								m_progressUpgrade.StepIt();								
								m_nBlkCnt++;								
							}
						}
					}
					else
					{
						ack = MessageBox ("片内程序有误，是否继续？", "", MB_OKCANCEL);
						if (ack == IDOK)
						{
							m_fInUpgrading = true;
							m_progressUpgrade.StepIt();	
							m_nBlkCnt++;
						}
						else
						{
							Init();
						}
					}
				}
			}
		}
		
		if (m_fInUpgrading == true)
		{
			SetTimer(TIMER_WAIT_DSP, 500, NULL);
		}
		while (m_mscomm.GetInBufferCount() != 0)
		{
			RCVCommand(gpAckDataBuf);
		}
		
		break;

	case TIMER_WAIT_DSP:
		
		KillTimer(TIMER_WAIT_DSP);
				
		m_btnUpgrade.EnableWindow(false);
		if (m_fInUpgrading == true)
		{
			SendProgBlock(m_nBlkCnt - 1);
		}
		else
		{
			SendCmd (CMD_PROG_UPGRADE, 0, 0, 0);
			m_fInUpgrading = true;
		}		
		break;
	}
	m_mscomm.SetRThreshold(1);
	
	CDialog::OnTimer(nIDEvent);
}

void CUpgradeDlg::OnButtonRestore() 
{
	int ack;
	ack = MessageBox ("取消此次下载，是否确定？", "", MB_OKCANCEL);
	if (ack == IDOK)
	{
		Init();
	}
}

#define S_MINUTE 60
#define S_HOUR (60*S_MINUTE)
#define S_DAY (24*S_HOUR)
#define S_YEAR (365*S_DAY)

typedef char 		   int_8;
typedef unsigned char  uint_8;
typedef short 		   int_16;
typedef unsigned short uint_16;
typedef long 		   int_32;
typedef unsigned long  uint_32;

const uint_32 pMonth1[12]=
{
	/*01月*/S_DAY * (0),
	/*02月*/S_DAY * (31),
	/*03月*/S_DAY * (31 + 28),
	/*04月*/S_DAY * (31 + 28 + 31),
	/*05月*/S_DAY * (31 + 28 + 31 + 30),
	/*06月*/S_DAY * (31 + 28 + 31 + 30 + 31),
	/*07月*/S_DAY * (31 + 28 + 31 + 30 + 31 + 30),
	/*08月*/S_DAY * (31 + 28 + 31 + 30 + 31 + 30 + 31),
	/*09月*/S_DAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31),
	/*10月*/S_DAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30),
	/*11月*/S_DAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),
	/*12月*/S_DAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)
};

const uint_16 pMonth2[12] = 
{
	/*01月*/31,
	/*02月*/28,
	/*03月*/31,
	/*04月*/30,
	/*05月*/31,
	/*06月*/30,
	/*07月*/31,
	/*08月*/31,
	/*09月*/30,
	/*10月*/31,
	/*11月*/30,
	/*12月*/31
};

uint_32 MY_mktime (uint_8 uYear, uint_8 uMonth, uint_8 uDay,
					uint_8 uHour, uint_8 uMinute, uint_8 uSecond)
{
	uint_32 lSecond;

	lSecond = S_YEAR * uYear + S_DAY * ((uYear + 3) / 4);		//前几年过去的秒数
   	lSecond += pMonth1[uMonth - 1];       					//加上今年本月过去的秒数
   	if ((uMonth > 2) && ((uYear % 4) == 0)) lSecond += S_DAY;	//闰年加1天秒数
   	lSecond += S_DAY * (uDay - 1);         					//加上本天过去的秒数
   	lSecond += S_HOUR * uHour;           						//加上本小时过去的秒数
   	lSecond += S_MINUTE * uMinute;       						//加上本分钟过去的秒数
   	lSecond += uSecond;               						//加上当前秒数
   	return lSecond;
}

void MY_gmtime (uint_32 lSecond,
				uint_8 *uYear, uint_8 *uMonth, uint_8 *uDay,
				uint_8 *uHour, uint_8 *uMinute, uint_8 *uSecond)
{
	uint_32 lDays;
	uint_16 uLeap;
 
    *uSecond = lSecond % 60;		//获得秒
    lSecond /= 60;
    *uMinute = lSecond % 60;		//获得分
    lSecond /= 60;
    *uHour = lSecond % 24;			//获得时
    lDays = lSecond / 24;			//获得总天数
    uLeap = (lDays + 1095) / 1461;	//过去了多少个闰年(4年一闰)
    if (((lDays + 1096) % 1461) == 0)
    {		
    	//闰年的最后1天
    	*uYear = (uint_16)(lDays / 366);	//获得年
        *uMonth = 12;              	//调整月
        *uDay = 31;
        return;
    }
    
    lDays -= uLeap;
    *uYear = (uint_16)(lDays / 365);//获得年
    lDays %= 365;                  	//今年的第几天
    lDays = lDays + 1;            	//1日开始
    if (((*uYear) % 4) == 0)
    {
       	if (lDays == 60)
		{
			*uMonth = 2;
           	*uDay = 29;
          	return;
		}
		else if (lDays > 60) 
       	{
       		lDays--;            	//闰年调整
       	}      
    }
    
    for (*uMonth = 0; pMonth2[*uMonth] < lDays; (*uMonth)++)
    {
        lDays -= pMonth2[*uMonth];
    }
    (*uMonth) ++;               	//调整月
    (*uDay) = (uint_16)lDays;      //获得日
}

uint_32 Date2Second(uint_8 uYear, uint_8 uMonth, uint_8 uDay,
					uint_8 uHour, uint_8 uMinute, uint_8 uSecond)
{
	uint_32  lTime, lTime0;

	if (uYear > 99 ||
		uMonth == 0 || uMonth > 12 ||
		uDay == 0 || uDay > 31 ||
		uHour > 23 ||
		uMinute > 59 ||
		uSecond > 59)
	{
		return 0;
	}
	               			 		
	lTime0 = MY_mktime (0, 1, 2, 0, 0, 0);	
	lTime = MY_mktime (uYear, uMonth, uDay, uHour, uMinute, uSecond);
	
	return lTime - lTime0;
}

void Second2Date(uint_32 lSecond,
				uint_8 *uYear, uint_8 *uMonth, uint_8 *uDay,
				uint_8 *uHour, uint_8 *uMinute, uint_8 *uSecond)
{
	uint_32 lTime, lTime0;
	
	lTime0 = MY_mktime (0, 1, 2, 0, 0, 0);	
	lTime = lTime0 + lSecond;
	
	MY_gmtime (lTime, uYear, uMonth, uDay, uHour, uMinute, uSecond);
}

uint_8 GetWeek (uint_8 uYear, uint_8 uMonth, uint_8 uDay)
{
	uint_8  uWeek;
	
	if (uMonth <= 2)
	{
		uMonth += 12;
		uYear --;
	}
	
	uWeek = (uDay + 2 * uMonth + 3 * (uMonth + 1) / 5 +
			uYear + uYear / 4 - uYear / 100 + uYear / 400) % 7;
	
	if (uWeek == 6) return 0;
	return uWeek + 1;
}

void CUpgradeDlg::OnButtonBrowse() 
{
	uint_8 uYear, uMonth, uDay;
	uint_8 uHour, uMinute, uSecond;

	Second2Date (373201131, &uYear, &uMonth, &uDay, &uHour, &uMinute, &uSecond);

	//获取升级文件路径	
	CString strFileName;
	CFileDialog fileDlg(true, "dat", "",
								OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
								"DSP program data files - 28000 (*.dat)|*.dat|");
	CFile f;
	char str[100];
	int nFileIndx;
	char  ch=' ';	
	WORD wVal;
	WORD wXor;
	int  bDir;
	int i;

	m_btnUpgrade.EnableWindow(false);
//	if (m_strFilePath.GetLength() != 0)
//	{
//		fileDlg.m_ofn.nMaxFile = 400;
//		fileDlg.m_ofn.lpstrFile = m_strFilePath.GetBuffer(400);
//	}
	//取第一个文件名
	if (fileDlg.DoModal() == IDCANCEL) return;
	strFileName = fileDlg.GetPathName();
	//处理第一个文件
	if (f.Open(strFileName, CFile::modeRead) == NULL)
	{
		AfxMessageBox("无法打开 " + strFileName);
		goto get_file_error_out;
	}

	ch = ' ';
	for (i = 0; ch != 0x0A; i++)
	{
		f.Read(&ch,1);
	}
	//读第一个文件
	nFileIndx = 0;
	for (i = 0; i < 0x8000; i++)
	{
		f.Read(str, 2);
		if (f.Read(str, 6) != 6)
		{
			AfxMessageBox("文件读取失败!");
			f.Close();
			goto get_file_error_out;
		}
		str[4] = '\0';
		wVal = Hex2Dec(str);
		gpProgData[nFileIndx++] = ((wVal & 0xff00) >> 8) + ((wVal & 0xff) << 8);//wVal;	
	}
	f.Close();
	if (gpProgData[0] != 0x01D8 || gpProgData[1] != 0x28AD)
	{
		AfxMessageBox("无效文件: " + strFileName);
		goto get_file_error_out;
	}

	//填充前面部分0-0x02FF
	for (i = 0; i < 0x300; i++)
	{
		gpProgData[i] = 0xFFFF;
	}
	
	//求PAGE2异或值
	wXor = 0;
	for (i = 0x300; i < 0x7800 - 1; i++)
	{
		wXor ^= gpProgData[i];
	}
	gpProgData[0x7800 - 1] = wXor;

	m_strFilePath = strFileName;

	UpdateData (false);

	//data read ok, begin to upgrade
	if (m_strFilePath2.GetLength() != 0)
	{
		m_btnUpgrade.EnableWindow(true);
		m_btnMakeHex.EnableWindow(true);
	}
	return;

get_file_error_out:
	m_strFilePath = "";
	UpdateData(false);
	m_btnUpgrade.EnableWindow(false);
	m_btnMakeHex.EnableWindow(false);
}

void CUpgradeDlg::OnBtncreatehexfile() 
{
	int i;
	CFile fHex;
	CFileDialog fDlg(false, "bin","",
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				"bin files (*.bin)|*.bin|");

	if (fDlg.DoModal() != IDOK) return;

	if (fHex.Open (fDlg.GetPathName(), CFile::modeCreate | CFile::modeWrite) == NULL)
	{
		MessageBox ("HEX文件打开失败!");
		return;
	}

	for (i = 0; i < 0x10000; i++)
	{
		gpProgByte[2 * i] = gpProgData[i] >> 8;
		gpProgByte[2 * i + 1] = gpProgData[i] & 0xFF;
	}
	fHex.Write (gpProgByte, 0x20000);
	for (i = 0; i < 0x20000; i++)
	{
		gpProgByte[i] = 0xFF;
	}
	fHex.Write (gpProgByte, 0x20000);
	//for 400A
	/*fHex.Write (gpProgByte, 0x20000);
	fHex.Write (gpProgByte, 0x20000);
	//for 800A	
	fHex.Write (gpProgByte, 0x20000);
	fHex.Write (gpProgByte, 0x20000);
	fHex.Write (gpProgByte, 0x20000);
	fHex.Write (gpProgByte, 0x20000);
	*/
	//for 160
/*	fHex.Write (gpProgByte, 0x20000);
	fHex.Write (gpProgByte, 0x20000);
	fHex.Write (gpProgByte, 0x20000);
	fHex.Write (gpProgByte, 0x20000);
	fHex.Write (gpProgByte, 0x20000);
	fHex.Write (gpProgByte, 0x20000);
	fHex.Write (gpProgByte, 0x20000);
	fHex.Write (gpProgByte, 0x20000);
*/	
	fHex.Close ();
}


void CUpgradeDlg::OnButtonBrowse2() 
{
	//取第二个文件名
	CString strFileName;
	CFileDialog fileDlg(true, "dat", "",
								OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
								"DSP program data files - 28000 (*.dat)|*.dat|");	
	CFile f;
	char str[100];
	int nFileIndx;
	char  ch=' ';	
	WORD wVal;
	WORD wXor;
	int  bDir;
	int i;

//	if (m_strFilePath2.GetLength() != 0)
//	{
//		fileDlg.m_ofn.nMaxFile = 400;
//		fileDlg.m_ofn.lpstrFile = m_strFilePath2.GetBuffer(400);
//	}
	//取第一个文件名
	if (fileDlg.DoModal() == IDCANCEL) return;
	strFileName = fileDlg.GetPathName();
	//处理第一个文件
	if (f.Open(strFileName, CFile::modeRead) == NULL)
	{
		AfxMessageBox("无法打开 " + strFileName);
		goto get_file_error_out_2;
	}

	ch = ' ';
	for (i = 0; ch != 0x0A; i++)
	{
		f.Read(&ch,1);
	}
	//读第二个文件
	nFileIndx = 0x8000;

	for (i = 0; i < 0x8000; i++)
	{
		f.Read(str, 2);
		if (f.Read(str, 6) != 6)
		{
			MessageBox("文件读取失败!");
			f.Close();
			goto get_file_error_out_2;
		}
		str[4] = '\0';
		wVal = Hex2Dec(str);
		gpProgData[nFileIndx++] = ((wVal & 0xff00) >> 8) + ((wVal & 0xff) << 8);//wVal;		
	}
	f.Close();
	if (gpProgData[0x8000] != 0x01D8 || gpProgData[0x8001] != 0x38AD)
	{
		AfxMessageBox("无效文件: " + strFileName);
		goto get_file_error_out_2;
	}
	
	//填充0x8000-0x807F
	for (i = 0x8000; i < 0x8080; i++)
	{
		gpProgData[i] = 0xFFFF;
	}	
	//填充0xF400-0xF407
	for (i = 0xF400; i < 0xF408; i++)
	{
		gpProgData[i] = 0xFFFF;
	}
	
	//求PAGE3异或值
	wXor = 0;
	for (i = 0x8080; i < 0xF400 - 1; i++)
	{
		wXor ^= gpProgData[i];
	}
	gpProgData[0xF400 - 1] = wXor;

	//调整版本信息数据
	bDir = 0;
	for (i = 0xF408; i < 0xF800; i++)
	{
		if (gpProgData[i] == 0) bDir = 1;
		if (bDir == 0) gpProgData[i] ^= 0x9536;//0x3695;
	}
	
	m_strFilePath2 = strFileName;

	UpdateData(false);

	//data read ok, begin to upgrade
	if (m_strFilePath.GetLength() != 0)
	{
		m_btnUpgrade.EnableWindow(true);
		m_btnMakeHex.EnableWindow(true);
	}
	return;

get_file_error_out_2:
	m_strFilePath2 = "";
	UpdateData(false);
	m_btnUpgrade.EnableWindow(false);
	m_btnMakeHex.EnableWindow(false);
}
