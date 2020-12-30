//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES
#if !defined(AFX_COMMTESTDLG_H__75932E5B_C78A_40AE_B17A_B80C0F907A15__INCLUDED_)
#define AFX_COMMTESTDLG_H__75932E5B_C78A_40AE_B17A_B80C0F907A15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommTestDlg.h : header file
//
#include "BmpButton.h" 
#include "CoolB.h"
/////////////////////////////////////////////////////////////////////////////
// CCommTestDlg dialog

class CCommTestDlg : public CDialog
{
// Construction
public:
	CCommTestDlg(CWnd* pParent = NULL);   // standard constructor

public:
	void UnlockEncode(BYTE *pSrc, BYTE *pDst, WORD nLenByte);
	void UnlockDecode(BYTE *pSrc, BYTE *pDst, WORD nLenByte);
	bool m_bFea;
	bool ReadFeature();
	bool gbRegStart;
	WORD GetParm(int nIndx);
	bool m_bEditChange;
		
	void CmdRefresh();
	void TXCommand(BYTE *pCmd,int lenCmd);
	int  RCVCommand(BYTE *pCmd);
	void Init();
	void CommSettings(int nBaud);
	void OnCmdInter();

// Dialog Data
	//{{AFX_DATA(CCommTestDlg)
	enum { IDD = IDD_COMM_TEST_DIALOG };
	CEdit	m_editBatchIndx;
	CEdit	m_editBatchCnt;
	CStatic	m_staticBatchIndx;
	CStatic	m_staticBatchCnt;
	CButton	m_btnBatch;
	CCoolButton	m_btnEnable;
	CEdit	m_editParm3;
	CEdit	m_editParm2;
	CEdit	m_editParm12;
	CEdit	m_editParm1;
	CEdit	m_editFeaPath;
	CEdit	m_editCmd;
	CEdit	m_editAckInfo;
	CEdit	m_editAckCode;
	CCoolButton	m_btnStoreFea;
	CCoolButton	m_btnCmd;
	CCoolButton	m_btnBrowse;
	CBmpButton	m_btnBmp;
	int		m_nType;
	CMSComm	m_mscomm;
	CString	m_strFeaPath;
	int		m_nMoveRight;
	int		m_nMoveUp;
	int		m_nLumEnc;
	BOOL	m_bIDEnc1;
	BOOL	m_bTwice;
	BOOL	m_bUpload;
	UINT	m_nCapTime;
	BOOL	m_bIDEnc2;
	BOOL	m_b2FP;
	BOOL	m_bImgRaw;
	UINT	m_nBatchCnt;
	UINT	m_nBatchIndx;
	BOOL	m_bEnableBatch;
	BOOL	m_bGetStorage;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommTestDlg)
	afx_msg void OnRadioGetUsercnt();
	afx_msg void OnRadioUserReg();
	afx_msg void OnRadioGetUserrole();
	afx_msg void OnRadioDelUser();
	afx_msg void OnRadioDelAlluser();
	afx_msg void OnRadioMatch1();
	afx_msg void OnRadioMatchN();
	afx_msg void OnRadioGetImage();
	afx_msg void OnRadioDownFeature();
	afx_msg void OnRadioDownMatch();
	afx_msg void OnRadioDownMatch1();
	afx_msg void OnRadioDownMatchN();
	afx_msg void OnRadioUpFeature();
	afx_msg void OnRadioGetFeature();
	afx_msg void OnRadioSetReg();
	afx_msg void OnRadioCommTest();
	afx_msg void OnRadioGetVersion();
	afx_msg void OnButtonCmd();
	afx_msg void OnButtonEnable();
	afx_msg void OnButtonFeatureBrowse();
	afx_msg void OnButtonStoreFeature();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnOnCommMscomm1();
	afx_msg void OnChangeParm1();
	afx_msg void OnChangeParm2();
	afx_msg void OnChangeParm3();
	afx_msg void OnChangeParm12();
	afx_msg void OnRadioSetMatchLevel();
	afx_msg void OnRadioGetUserInfo();
	afx_msg void OnRadioSetLpMode();
	afx_msg void OnRadioSetEnrollMode();
	afx_msg void OnRadioGetOneRecord();
	afx_msg void OnRadioGetNewRecord();
	afx_msg void OnRadioClearRecord();
	afx_msg void OnRadioSetTime();
	afx_msg void OnRadioGetTime();
	afx_msg void OnBtnMoveRight();
	afx_msg void OnBtnMoveUp();
	afx_msg void OnCheckTwice();
	afx_msg void OnCheckUpload();
	afx_msg void OnCheckIdenc1();
	afx_msg void OnBtnLum();
	afx_msg void OnCheck2fp();
	afx_msg void OnCheckImgRaw();
	afx_msg void OnCheckIdenc2();
	afx_msg void OnLumAdjust();
	afx_msg void OnBtnBatch();
	afx_msg void OnCheckEnableBatch();
	afx_msg void OnCheckGetStorage();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMTESTDLG_H__75932E5B_C78A_40AE_B17A_B80C0F907A15__INCLUDED_)
