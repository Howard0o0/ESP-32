//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES
#if !defined(AFX_UPGRADEDLG_H__A9D340DA_1AB6_4595_8498_C618E00923C4__INCLUDED_)
#define AFX_UPGRADEDLG_H__A9D340DA_1AB6_4595_8498_C618E00923C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UpgradeDlg.h : header file
//
#include "CmdHead.h"
#include "CoolB.h"
/////////////////////////////////////////////////////////////////////////////
// CUpgradeDlg dialog

class CUpgradeDlg : public CDialog
{
// Construction
public:
	CUpgradeDlg(CWnd* pParent = NULL);   // standard constructor
public:
	bool m_fInUpgrading;
	int  m_nBlkCnt;
	WORD   m_pwProgData[PROG_LEN];
	
	void CommSettings(int nBaud);
	void SendCmd(BYTE nCmdType, BYTE P1, BYTE P2, BYTE P3);
	void Init();	
	void SendProgBlock(int nBlkBlock);
	void TXCommand(BYTE *pCmd,int lenCmd);
	int  RCVCommand(BYTE *pCmd);

// Dialog Data
	//{{AFX_DATA(CUpgradeDlg)
	enum { IDD = IDD_UPGRADE };
	CCoolButton	m_btnBrowse2;
	CCoolButton	m_btnMakeHex;
	CCoolButton	m_btnCancel;
	CCoolButton	m_btnUpgrade;
	CCoolButton	m_btnRestore;
	CCoolButton	m_btnBrowse;
	CProgressCtrl	m_progressUpgrade;
	CString	m_strFilePath;
	CMSComm	m_mscomm;
	CString	m_strFilePath2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpgradeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUpgradeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonUpgrade();
	afx_msg void OnMscomm();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonRestore();
	afx_msg void OnButtonBrowse();
	afx_msg void OnBtncreatehexfile();
	afx_msg void OnButtonBrowse2();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPGRADEDLG_H__A9D340DA_1AB6_4595_8498_C618E00923C4__INCLUDED_)
