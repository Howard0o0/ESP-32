#if !defined(AFX_COOLB_H__C06A5CD0_5B59_11D4_BD73_000021479D21__INCLUDED_)
#define AFX_COOLB_H__C06A5CD0_5B59_11D4_BD73_000021479D21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoolB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoolButton window

class CCoolButton : public CButton
{
	DECLARE_DYNAMIC(CCoolButton)

// Construction
public:
	CCoolButton();

// Attributes
public:
	enum FlatStyle
	{
		BUTTONSTYLE_3D,
		BUTTONSTYLE_FLAT,
		BUTTONSTYLE_SEMIFLAT,
		BUTTONSTYLE_NOBORDERS
	};

	enum DispalyStyle
	{
		BUTTONSTYLE_IMAGE,
		BUTTONSTYLE_TEXT,
		BUTTONSTYLE_BOTH
	};

	DispalyStyle	m_nDisplayStyle;
	FlatStyle		m_nFlatStyle;
	BOOL			m_bRighImage;

	CToolTipCtrl& GetToolTipCtrl ()
	{
		return m_wndToolTip;
	}

public:
	BOOL IsPressed () const
	{
		return m_bPushed && m_bHighlighted;
	}

protected:
	BOOL			m_bPushed;
	BOOL			m_bHighlighted;
	BOOL			m_bCaptured;
	BOOL			m_bHover;

	CSize			m_sizeImage;
	HICON			m_hIcon;
	HBITMAP			m_hBitmap;
	HICON			m_hIconHot;
	HBITMAP			m_hBitmapHot;
	HBITMAP			m_hBitmapDisabled;
	BOOL			m_bAutoDestroyImage;

	CToolTipCtrl	m_wndToolTip;

	CSize			m_sizePushOffset;

	COLORREF		m_clrRegular;
	COLORREF		m_clrHover;

// Operations
public:
	void SetImage (HICON hIcon, BOOL bAutoDestroy = TRUE, HICON hIconHot = NULL);
	void SetImage (HBITMAP hBitmap, BOOL bAutoDestroy = TRUE, HBITMAP hBitmapHot = NULL);
	void SetImage (UINT uiBmpResId, UINT uiBmpHotResId = 0);
	void SetTooltip (LPCTSTR lpszToolTipText);	// lpszToolTip == NULL: disable tooltip

	void SetTextColor (COLORREF clrText)
	{
		m_clrRegular = clrText;
	}

	void SetTextHotColor (COLORREF clrTextHot)
	{
		m_clrHover = clrTextHot;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCoolButton();
	virtual void CleanUp ();

	virtual CSize SizeToContent (BOOL bCalcOnly = FALSE);

protected:
	virtual void OnDrawFocusRect (CDC* pDC, const CRect& rectClient);
	virtual void OnDraw (CDC* pDC, const CRect& rect, UINT uiState);

	virtual BOOL CreateDisabledImage ();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCoolButton)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCancelMode();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLB_H__C06A5CD0_5B59_11D4_BD73_000021479D21__INCLUDED_)
