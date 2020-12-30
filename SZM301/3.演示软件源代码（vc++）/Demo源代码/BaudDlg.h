#if !defined(AFX_BAUDDLG_H__C5470AF8_DE96_4F95_8A9B_DB1C0EBF6058__INCLUDED_)
#define AFX_BAUDDLG_H__C5470AF8_DE96_4F95_8A9B_DB1C0EBF6058__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaudDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaudDlg dialog

class CBaudDlg : public CDialog
{
// Construction
public:
	CBaudDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBaudDlg)
	enum { IDD = IDD_BAUD_DIALOG };
	CComboBox	m_comboPort;
	CComboBox	m_comboBaud;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaudDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBaudDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BAUDDLG_H__C5470AF8_DE96_4F95_8A9B_DB1C0EBF6058__INCLUDED_)
