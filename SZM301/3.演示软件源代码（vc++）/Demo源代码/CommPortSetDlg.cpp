// CommPortSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "STTStar.h"
#include "CommPortSetDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int gnPort;
/////////////////////////////////////////////////////////////////////////////
// CCommPortSetDlg dialog


CCommPortSetDlg::CCommPortSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommPortSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommPortSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCommPortSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommPortSetDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_PORT, m_comboPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommPortSetDlg, CDialog)
	//{{AFX_MSG_MAP(CCommPortSetDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommPortSetDlg message handlers

void CCommPortSetDlg::OnOK() 
{
	gnPort = m_comboPort.GetCurSel() + 1;
	
	CDialog::OnOK();
}

BOOL CCommPortSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_comboPort.InsertString(0, "COM1");
	m_comboPort.InsertString(1, "COM2");

	m_comboPort.SetCurSel(gnPort - 1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
