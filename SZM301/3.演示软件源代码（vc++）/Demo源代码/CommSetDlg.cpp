// CommSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "STTStar.h"
#include "CommSetDlg.h"

#include "CmdHead.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int gnBaud;
extern int gnPort;

extern int  gnLenCmd;	
extern int  gnLenAck;
extern BYTE gpCmdDataBuf[];
extern BYTE gpAckDataBuf[];
/////////////////////////////////////////////////////////////////////////////
// CCommSetDlg dialog
CCommSetDlg::CCommSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommSetDlg)
	m_bChange = FALSE;
	//}}AFX_DATA_INIT
}


void CCommSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommSetDlg)
	DDX_Control(pDX, IDC_COMBO_PORT, m_comboPort);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_SET_BAUD, m_btnSetBaud);
	DDX_Control(pDX, IDC_CHECK_CHAHGE, m_chkChange);
	DDX_Control(pDX, IDC_COMBO_SET_BAUD, m_comboSetBaud);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_comboBaud);
	DDX_Control(pDX, IDC_MSCOMM1, m_mscomm);
	DDX_Check(pDX, IDC_CHECK_CHAHGE, m_bChange);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommSetDlg, CDialog)
	//{{AFX_MSG_MAP(CCommSetDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUD, OnSelchangeComboBaud)
	ON_BN_CLICKED(IDC_BUTTON_SET_BAUD, OnButtonSetBaud)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_CHAHGE, OnCheckChahge)
	ON_CBN_CLOSEUP(IDC_COMBO_BAUD, OnCloseupComboBaud)
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, OnSelchangeComboPort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommSetDlg message handlers

BOOL CCommSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	m_comboPort.InsertString(0, "COM1");
	m_comboPort.InsertString(1, "COM2");
	m_comboPort.InsertString(2, "COM3");
	m_comboPort.InsertString(3, "COM4");
	m_comboPort.InsertString(4, "COM5");
	m_comboPort.InsertString(5, "COM6");
	m_comboPort.InsertString(6, "COM7");
	m_comboPort.InsertString(7, "COM8");
	

	m_comboPort.SetCurSel(gnPort - 1);


	m_comboBaud.InsertString(0, "9,600");
	m_comboBaud.InsertString(1, "19,200");
	m_comboBaud.InsertString(2, "38,400");
	m_comboBaud.InsertString(3, "57,600");
	m_comboBaud.InsertString(4, "115,200");

	m_comboBaud.EnableWindow(false);

	m_comboBaud.SetCurSel(gnBaud - 1);

	m_comboSetBaud.InsertString(0, "9,600");
	m_comboSetBaud.InsertString(1, "19,200");
	m_comboSetBaud.InsertString(2, "38,400");
	m_comboSetBaud.InsertString(3, "57,600");
	m_comboSetBaud.InsertString(4, "115,200");

	m_comboSetBaud.SetCurSel(gnBaud - 1);

	
	//init comm
	m_mscomm.SetCommPort(gnPort);
/*	if(!m_mscomm.GetPortOpen())
	{
		m_mscomm.SetPortOpen(true);
	}
*/
	CommSettings(gnBaud);	
	m_mscomm.SetInputMode(1);//byte mode
		
	m_mscomm.SetRThreshold(1);
	m_mscomm.SetInputLen(9000);

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CCommSetDlg::RCVCommand(BYTE *pCmd)
{
	int       lenCmd;
	VARIANT   varCmd;
	SAFEARRAY *pArray;
	
	varCmd=m_mscomm.GetInput();
	pArray=(SAFEARRAY *)varCmd.plVal;
	lenCmd=pArray->rgsabound[0].cElements;
	memcpy(pCmd,pArray->pvData,lenCmd);

	return   lenCmd;
}

void CCommSetDlg::TXCommand(BYTE *pCmd, int lenCmd)
{
	VARIANT   varCmd;
	SAFEARRAY safeArray;

	safeArray.fFeatures=VT_VARIANT;
	safeArray.cbElements=1;
	safeArray.cDims=1;
	safeArray.cLocks=0;
	safeArray.pvData=(LPVOID)pCmd;

	safeArray.rgsabound[0].cElements=lenCmd;
	safeArray.rgsabound[0].lLbound=0;

	VariantInit(&varCmd);
	varCmd.vt=0x2011;//VT_ARRAY|VT_BOOL;
	varCmd.plVal=(long*)&safeArray;
	m_mscomm.SetOutput(varCmd);
}

void CCommSetDlg::CommSettings(int nBaud)
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
		break;
	}
}

void CCommSetDlg::OnSelchangeComboBaud() 
{
	gnBaud = m_comboBaud.GetCurSel() + 1;	
	CommSettings(gnBaud);	
	
	m_bChange = false;
	UpdateData(false);
}


BEGIN_EVENTSINK_MAP(CCommSetDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCommSetDlg)
	ON_EVENT(CCommSetDlg, IDC_MSCOMM1, 1 /* OnComm */, OnOnCommMscomm1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCommSetDlg::OnOnCommMscomm1() 
{
	int nEvent;

	nEvent = m_mscomm.GetCommEvent();
	if (gnBaud == 0)
	{
		gnBaud = 1;
	}
	if (nEvent==EVENT_RCV)
	{
		m_mscomm.SetRThreshold(9000);
		SetTimer(TIMER_RCV_DELAY, 100, NULL);
	}	
}

void CCommSetDlg::OnTimer(UINT nIDEvent) 
{
	int  lenAck = 0;	

	switch (nIDEvent)
	{
	case TIMER_RCV_DELAY:		

		m_mscomm.SetRThreshold(1);
		
		lenAck = RCVCommand(&gpAckDataBuf[gnLenAck]);
		gnLenAck += lenAck;
		if (lenAck !=0 && gnLenAck < 0x3000)
		{
			return;
		}
			
		//串口接收数据完毕
		KillTimer(TIMER_RCV_DELAY);	
		
		if (gnLenAck != 8)
		{
			MessageBox("通信失败!");			
		}
		//分析返回数据
		switch (gpAckDataBuf[1])
		{
		case ACK_SET_BAUD:			
			switch (gpAckDataBuf[4])
			{
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				gnBaud = m_comboSetBaud.GetCurSel() + 1;
				CommSettings(gnBaud);		
				m_comboBaud.SetCurSel(gnBaud - 1);
				break;
			default:
				MessageBox("通信失败!");
				break;
			}
			break;	
		default:
			break;
		}
	}	
	CDialog::OnTimer(nIDEvent);
}

void CCommSetDlg::OnButtonSetBaud() 
{
	int i;

	gpCmdDataBuf[0] = CMD_HEAD;
	gpCmdDataBuf[1] = CMD_SET_BAUD;
	gpCmdDataBuf[2] = 0;
	gpCmdDataBuf[3] = 0;
	gpCmdDataBuf[4] = m_comboSetBaud.GetCurSel() + 1;	
	gpCmdDataBuf[5] = CMD_CH;
	gpCmdDataBuf[7] = CMD_TAIL;

	BYTE byXor=0;

	for (i = 1; i <= 5; i++)
	{
		byXor ^= gpCmdDataBuf[i];
	}
	gpCmdDataBuf[6] = byXor;
	gnLenCmd = 8;
	gnLenAck = 0;
	TXCommand(gpCmdDataBuf, gnLenCmd);
}


void CCommSetDlg::OnCheckChahge() 
{
	UpdateData(true);
	if (m_bChange)
	{
		m_comboBaud.EnableWindow(true);
	}
	else
	{
		m_comboBaud.EnableWindow(false);
	}
}

void CCommSetDlg::OnCloseupComboBaud() 
{
	gnBaud = m_comboBaud.GetCurSel() + 1;	
	CommSettings(gnBaud);	
	m_comboBaud.EnableWindow(false);
	
	m_bChange = false;
	UpdateData(false);
}

void CCommSetDlg::OnSelchangeComboPort() 
{
	try
	{
		if (m_mscomm.GetPortOpen())
		{
			m_mscomm.SetPortOpen(false);
		}

		gnPort = m_comboPort.GetCurSel() + 1;

		m_mscomm.SetCommPort(gnPort);	
		m_mscomm.SetPortOpen(true);
		CommSettings(gnBaud);
	}
	catch (...)
	{
		MessageBox ("串口错误!");
	}

	m_mscomm.SetInputMode(1);//byte mode
		
	m_mscomm.SetRThreshold(1);
	m_mscomm.SetInputLen(9000);	
}
