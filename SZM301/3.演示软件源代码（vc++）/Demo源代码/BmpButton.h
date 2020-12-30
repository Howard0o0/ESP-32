#if !defined(AFX_BMPBUTTON_H__8256FFA4_C06D_48C2_BD51_59CEFF4446A7__INCLUDED_)
#define AFX_BMPBUTTON_H__8256FFA4_C06D_48C2_BD51_59CEFF4446A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BmpButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBmpButton window

class CBmpButton : public CButton
{
// Construction
public:
	CBmpButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBmpButton();
public:
	LPBITMAPINFO  m_pBmi;
	LPBYTE        m_pImage; 
	DWORD         m_dwWidth;
	DWORD         m_dwHeight;

	// Generated message map functions
protected:
	//{{AFX_MSG(CBmpButton)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPBUTTON_H__8256FFA4_C06D_48C2_BD51_59CEFF4446A7__INCLUDED_)
