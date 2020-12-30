#if !defined(AFX_COMMPORTSETDLG_H__81847EDF_3F22_4F06_8FEA_6FAA46BC5262__INCLUDED_)
#define AFX_COMMPORTSETDLG_H__81847EDF_3F22_4F06_8FEA_6FAA46BC5262__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommPortSetDlg.h : header file
//
#include "CoolB.h"
/////////////////////////////////////////////////////////////////////////////
// CCommPortSetDlg dialog

class CCommPortSetDlg : public CDialog
{
// Construction
public:
	CCommPortSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCommPortSetDlg)
	enum { IDD = IDD_COMM_PORT_SET };
	CCoolButton	m_btnOK;
	CCoolButton	m_btnCancel;
	CComboBox	m_comboPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommPortSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommPortSetDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMPORTSETDLG_H__81847EDF_3F22_4F06_8FEA_6FAA46BC5262__INCLUDED_)
