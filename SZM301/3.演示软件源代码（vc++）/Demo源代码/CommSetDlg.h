//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES
#if !defined(AFX_COMMSETDLG_H__4DB17B7C_470D_47C7_A4AC_0E0ADE326BF8__INCLUDED_)
#define AFX_COMMSETDLG_H__4DB17B7C_470D_47C7_A4AC_0E0ADE326BF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommSetDlg.h : header file
//
#include "CoolB.h"
/////////////////////////////////////////////////////////////////////////////
// CCommSetDlg dialog

class CCommSetDlg : public CDialog
{
// Construction
public:
	void CommSettings(int nBaud);
	CCommSetDlg(CWnd* pParent = NULL);   // standard constructor
public:
	void TXCommand(BYTE *pCmd,int lenCmd);
	int  RCVCommand(BYTE *pCmd);
// Dialog Data
	//{{AFX_DATA(CCommSetDlg)
	enum { IDD = IDD_COMM_SET_DIALOG };
	CComboBox	m_comboPort;
	CCoolButton	m_btnCancel;
	CCoolButton	m_btnSetBaud;
	CButton	m_chkChange;
	CComboBox	m_comboSetBaud;
	CComboBox	m_comboBaud;
	CMSComm	m_mscomm;
	BOOL	m_bChange;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboBaud();
	afx_msg void OnOnCommMscomm1();
	afx_msg void OnButtonSetBaud();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCheckChahge();
	afx_msg void OnCloseupComboBaud();
	afx_msg void OnSelchangeComboPort();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMSETDLG_H__4DB17B7C_470D_47C7_A4AC_0E0ADE326BF8__INCLUDED_)
