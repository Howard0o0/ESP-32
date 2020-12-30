// STTStarView.cpp : implementation of the CSTTStarView class
//

#include "stdafx.h"
#include "STTStar.h"

#include "STTStarDoc.h"
#include "STTStarView.h"

//#include "UpgradeDlg.h"

//#include "CommTestDlg.h"
//#include "CommSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString		gstrFileName = "";
CString		gstrXStart = "61";
CString     gstrYStart = "5C";

extern      int gnPort;
extern      int gnBaud;
int			gnCurDlg = DLG_TEST;

/////////////////////////////////////////////////////////////////////////////
// CSTTStarView

IMPLEMENT_DYNCREATE(CSTTStarView, CFormView)

BEGIN_MESSAGE_MAP(CSTTStarView, CFormView)
	//{{AFX_MSG_MAP(CSTTStarView)
	ON_COMMAND(ID_DSP_TEST, OnDspTest)
	ON_COMMAND(ID_DSP_SETTINGS, OnDspSettings)
	ON_COMMAND(ID_DSP_UPGRADE, OnDspUpgrade)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_DSP_SETTINGS, OnUpdateDspSettings)
	ON_UPDATE_COMMAND_UI(ID_DSP_TEST, OnUpdateDspTest)
	ON_UPDATE_COMMAND_UI(ID_DSP_UPGRADE, OnUpdateDspUpgrade)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTTStarView construction/destruction

CSTTStarView::CSTTStarView()
	: CFormView(CSTTStarView::IDD)
{
	//{{AFX_DATA_INIT(CSTTStarView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CSTTStarView::~CSTTStarView()
{
}

void CSTTStarView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTTStarView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CSTTStarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CSTTStarView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CSTTStarView printing

BOOL CSTTStarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSTTStarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSTTStarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSTTStarView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CSTTStarView diagnostics

#ifdef _DEBUG
void CSTTStarView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSTTStarView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSTTStarDoc* CSTTStarView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSTTStarDoc)));
	return (CSTTStarDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSTTStarView message handlers

void CSTTStarView::OnDspTest() 
{
	m_commSetDlg.ShowWindow(SW_HIDE);
	m_upgradeDlg.ShowWindow(SW_HIDE);
	
	m_commTestDlg.ShowWindow(SW_SHOW);	

	try
	{
		if (m_commSetDlg.m_mscomm.GetPortOpen())
		{
			m_commSetDlg.m_mscomm.SetPortOpen (false);
		}
		if (m_commTestDlg.m_mscomm.GetPortOpen())
		{
			m_commTestDlg.m_mscomm.SetPortOpen (false);
		}
		if (m_upgradeDlg.m_mscomm.GetPortOpen())
		{
			m_upgradeDlg.m_mscomm.SetPortOpen (false);
		}

		m_commTestDlg.m_mscomm.SetCommPort(gnPort);
		m_commTestDlg.m_mscomm.SetPortOpen (true);
		m_commTestDlg.CommSettings(gnBaud);
	}
	catch (...)
	{
		MessageBox ("Comm port error!");
	}

	m_commTestDlg.CmdRefresh();

	gnCurDlg = DLG_TEST;
}

void CSTTStarView::OnDspSettings() 
{
	m_commTestDlg.ShowWindow(SW_HIDE);
	m_upgradeDlg.ShowWindow(SW_HIDE);

	m_commSetDlg.ShowWindow(SW_SHOW);

	try
	{
		if (m_commSetDlg.m_mscomm.GetPortOpen())
		{
			m_commSetDlg.m_mscomm.SetPortOpen (false);
		}
		if (m_commTestDlg.m_mscomm.GetPortOpen())
		{
			m_commTestDlg.m_mscomm.SetPortOpen (false);
		}
		if (m_upgradeDlg.m_mscomm.GetPortOpen())
		{
			m_upgradeDlg.m_mscomm.SetPortOpen (false);
		}

		m_commSetDlg.m_mscomm.SetCommPort(gnPort);
		m_commSetDlg.m_mscomm.SetPortOpen (true);
		m_commSetDlg.CommSettings(gnBaud);
	}
	catch (...)
	{
		MessageBox ("Comm port error!");
	}

	gnCurDlg = DLG_SET;
}

void CSTTStarView::OnDspUpgrade() 
{
	m_commSetDlg.ShowWindow(SW_HIDE);
	m_commTestDlg.ShowWindow(SW_HIDE);

	m_upgradeDlg.ShowWindow(SW_SHOW);	
	
	try
	{
		if (m_commSetDlg.m_mscomm.GetPortOpen())
		{
			m_commSetDlg.m_mscomm.SetPortOpen (false);
		}
		if (m_commTestDlg.m_mscomm.GetPortOpen())
		{
			m_commTestDlg.m_mscomm.SetPortOpen (false);
		}
		if (m_upgradeDlg.m_mscomm.GetPortOpen())
		{
			m_upgradeDlg.m_mscomm.SetPortOpen (false);
		}

		m_upgradeDlg.m_mscomm.SetCommPort(gnPort);
		m_upgradeDlg.m_mscomm.SetPortOpen (true);
		m_upgradeDlg.CommSettings(gnBaud);
	}
	catch (...)
	{
		MessageBox ("Comm port error!");
	}

	gnCurDlg = DLG_UPGRADE;
}

int CSTTStarView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_commSetDlg.Create(IDD_COMM_SET_DIALOG, this);
	m_commTestDlg.Create(IDD_COMM_TEST_DIALOG, this);
	m_upgradeDlg.Create(IDD_UPGRADE, this);

	m_commSetDlg.ShowWindow(SW_HIDE);
	m_commTestDlg.ShowWindow(SW_SHOW);
	m_upgradeDlg.ShowWindow(SW_HIDE);
	
	OnDspTest();
	
	return 0;
}

void CSTTStarView::OnUpdateDspSettings(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(gnCurDlg == DLG_SET);	
}

void CSTTStarView::OnUpdateDspTest(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(gnCurDlg == DLG_TEST);	
}

void CSTTStarView::OnUpdateDspUpgrade(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(false);
	pCmdUI->SetCheck(gnCurDlg == DLG_UPGRADE);	
}
