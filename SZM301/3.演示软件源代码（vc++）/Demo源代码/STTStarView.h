// STTStarView.h : interface of the CSTTStarView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STTSTARVIEW_H__5F94BFDB_257F_48CB_B1D9_8F120F1E4484__INCLUDED_)
#define AFX_STTSTARVIEW_H__5F94BFDB_257F_48CB_B1D9_8F120F1E4484__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommSetDlg.h"
#include "CommTestDlg.h"
#include "UpgradeDlg.h"

class CSTTStarView : public CFormView
{
protected: // create from serialization only
	CSTTStarView();
	DECLARE_DYNCREATE(CSTTStarView)
public:
	CCommSetDlg  m_commSetDlg;
	CCommTestDlg m_commTestDlg;
	CUpgradeDlg  m_upgradeDlg;
public:
	//{{AFX_DATA(CSTTStarView)
	enum{ IDD = IDD_STTSTAR_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CSTTStarDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTTStarView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSTTStarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSTTStarView)
	afx_msg void OnDspTest();
	afx_msg void OnDspSettings();
	afx_msg void OnDspUpgrade();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateDspSettings(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDspTest(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDspUpgrade(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in STTStarView.cpp
inline CSTTStarDoc* CSTTStarView::GetDocument()
   { return (CSTTStarDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STTSTARVIEW_H__5F94BFDB_257F_48CB_B1D9_8F120F1E4484__INCLUDED_)
