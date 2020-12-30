// BaudDlg.cpp : implementation file
//

#include "stdafx.h"
#include "STTStar.h"
#include "BaudDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int gnBaud;
extern int gnPort;
/////////////////////////////////////////////////////////////////////////////
// CBaudDlg dialog


CBaudDlg::CBaudDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBaudDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBaudDlg)
	//}}AFX_DATA_INIT
}


void CBaudDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBaudDlg)
	DDX_Control(pDX, IDC_COMBOPORT, m_comboPort);
	DDX_Control(pDX, IDC_COMBOBAUD, m_comboBaud);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBaudDlg, CDialog)
	//{{AFX_MSG_MAP(CBaudDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaudDlg message handlers

BOOL CBaudDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	m_comboBaud.InsertString(0,"9,600");
	m_comboBaud.InsertString(1,"19,200");
	m_comboBaud.InsertString(2,"38,400");
	m_comboBaud.InsertString(3,"57,600");
	m_comboBaud.InsertString(4,"115,200");

	m_comboBaud.SetCurSel(1);

	m_comboPort.InsertString(0,"COM1");
	m_comboPort.InsertString(1,"COM2");

	m_comboPort.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBaudDlg::OnOK() 
{
	gnBaud=m_comboBaud.GetCurSel()+1;
	gnPort=m_comboPort.GetCurSel()+1;
			
	CDialog::OnOK();
}
