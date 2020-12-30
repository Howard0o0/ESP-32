// CoolB.cpp : implementation file
//

#include "stdafx.h"
#include "CoolB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int nImageHorzMargin = 10;
static const int nVertMargin = 5;
static const COLORREF clrDefault = (COLORREF) -1;

/////////////////////////////////////////////////////////////////////////////
// CCoolButton

IMPLEMENT_DYNAMIC(CCoolButton, CButton)

CCoolButton::CCoolButton()
{
	m_bPushed = FALSE;
	m_bHighlighted = FALSE;
	m_bCaptured = FALSE;
	m_nFlatStyle = BUTTONSTYLE_FLAT;
	m_nDisplayStyle = BUTTONSTYLE_BOTH;
	m_sizeImage = CSize (0, 0);
	m_bAutoDestroyImage = FALSE;
	m_hIcon = NULL;
	m_hBitmap = NULL;
	m_hIconHot = NULL;
	m_hBitmapHot = NULL;
	m_hBitmapDisabled = NULL;
	m_bRighImage = FALSE;
	m_sizePushOffset = CSize (2, 2);
	m_bHover = FALSE;
	m_clrRegular = clrDefault;
	m_clrHover = clrDefault;
}
//****************************************************************************
CCoolButton::~CCoolButton()
{
	CleanUp ();
}
//****************************************************************************
void CCoolButton::CleanUp ()
{
	if (m_bAutoDestroyImage)
	{
		if (m_hBitmap != NULL)
		{
			::DeleteObject (m_hBitmap);
		}

		if (m_hIcon != NULL)
		{
			::DestroyIcon (m_hIcon);
		}

		if (m_hBitmapHot != NULL)
		{
			::DeleteObject (m_hBitmapHot);
		}

		if (m_hIconHot != NULL)
		{
			::DestroyIcon (m_hIconHot);
		}
	}

	if (m_hBitmapDisabled != NULL)
	{
		::DeleteObject (m_hBitmapDisabled);
		m_hBitmapDisabled = NULL;
	}

	m_hBitmap = NULL;
	m_hIcon = NULL;
	m_hIconHot = NULL;
	m_hBitmapHot = NULL;
	m_bAutoDestroyImage = FALSE;

	m_sizeImage = CSize (0, 0);
}


BEGIN_MESSAGE_MAP(CCoolButton, CButton)
	//{{AFX_MSG_MAP(CCoolButton)
	ON_WM_ERASEBKGND()
	ON_WM_CANCELMODE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolButton message handlers

void CCoolButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	ASSERT (lpDIS != NULL);
	ASSERT (lpDIS->CtlType == ODT_BUTTON);

	CDC* pDC = CDC::FromHandle (lpDIS->hDC);
	ASSERT_VALID (pDC);

	CRect rectClient = lpDIS->rcItem;

	//--------------
	// Fill coltrol:
	//--------------
	COLORREF clrBtnFace = ::GetSysColor(COLOR_BTNFACE);
	CBrush brBtnFace;
	brBtnFace.CreateSolidBrush (clrBtnFace);

	pDC->FillRect (rectClient, &brBtnFace);

	if (lpDIS->itemState & ODS_FOCUS)
	{
		OnDrawFocusRect (pDC, rectClient);
	}

	//----------------
	// Draw 3d border:
	//----------------
	COLORREF clrBtnShadow,clrBtnHilite,clrBtnDkShadow,clrBtnLight;
	clrBtnShadow = ::GetSysColor(COLOR_BTNSHADOW);
	clrBtnDkShadow = ::GetSysColor(COLOR_3DDKSHADOW);
	clrBtnLight = ::GetSysColor(COLOR_3DLIGHT);
	clrBtnHilite = ::GetSysColor(COLOR_BTNHIGHLIGHT);

	if (m_nFlatStyle != BUTTONSTYLE_NOBORDERS)
	{
		if (m_bPushed && m_bHighlighted || (lpDIS->itemState & ODS_SELECTED))
		{
			pDC->Draw3dRect (rectClient,clrBtnDkShadow,clrBtnHilite);

			rectClient.DeflateRect (1, 1);

			if (m_nFlatStyle != BUTTONSTYLE_FLAT)
			{
				pDC->Draw3dRect (rectClient,clrBtnShadow,clrBtnLight);
			}

			rectClient.DeflateRect (1, 1);

			rectClient.left += m_sizePushOffset.cx;
			rectClient.top += m_sizePushOffset.cy;
		}
		else if (m_nFlatStyle != BUTTONSTYLE_FLAT || m_bHighlighted)
		{
			pDC->Draw3dRect (rectClient,clrBtnHilite,clrBtnDkShadow);
			rectClient.DeflateRect (1, 1);

			if (m_nFlatStyle == BUTTONSTYLE_3D ||
				(m_nFlatStyle == BUTTONSTYLE_SEMIFLAT && m_bHighlighted))
			{
				pDC->Draw3dRect (rectClient,clrBtnLight,clrBtnShadow);
			}

			rectClient.DeflateRect (1, 1);
		}
		else
		{
			rectClient.DeflateRect (2, 2);
		}
	}
	else
	{
		rectClient.DeflateRect (2, 2);
	}

	//---------------------
	// Draw button content:
	//---------------------
	OnDraw (pDC, rectClient, lpDIS->itemState);
}
//****************************************************************************
void CCoolButton::PreSubclassWindow() 
{
	ModifyStyle (BS_DEFPUSHBUTTON, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}
//****************************************************************************
BOOL CCoolButton::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= BS_OWNERDRAW;
	cs.style &= ~BS_DEFPUSHBUTTON;

	return CButton::PreCreateWindow(cs);
}
//****************************************************************************
BOOL CCoolButton::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
//****************************************************************************
void CCoolButton::OnDraw (CDC* pDC, const CRect& rect, UINT uiState)
{
	COLORREF clrBtnText = ::GetSysColor(COLOR_BTNTEXT);
	COLORREF clrBtnHilite = ::GetSysColor(COLOR_BTNHIGHLIGHT);
	COLORREF clrGrayedText = ::GetSysColor (COLOR_GRAYTEXT);

	CRect rectText = rect;
	CRect rectImage = rect;

	CString strText;
	GetWindowText (strText);

	if (m_sizeImage.cx != 0 && m_nDisplayStyle != BUTTONSTYLE_TEXT)
	{
		if (!strText.IsEmpty () && m_nDisplayStyle != BUTTONSTYLE_IMAGE)
		{
			if (m_bRighImage)
			{
				rectText.right -= m_sizeImage.cx + nImageHorzMargin / 2;
				rectImage.left = rectText.right;
				rectImage.right -= nImageHorzMargin / 2;
			}
			else
			{
				rectText.left +=  m_sizeImage.cx + nImageHorzMargin / 2;
				rectImage.left += nImageHorzMargin / 2;
				rectImage.right = rectText.left;
			}
		}

		// Center image:
		rectImage.DeflateRect ((rectImage.Width () - m_sizeImage.cx) / 2,
			max (0, (rect.Height () - m_sizeImage.cy) / 2));
	}
	else
	{
		rectImage.SetRectEmpty ();
	}

	//-----------
	// Draw text:
	//-----------
	pDC->SetBkMode (TRANSPARENT);

	if(m_nDisplayStyle != BUTTONSTYLE_IMAGE)
	{
		COLORREF clrText = m_clrRegular == clrDefault ? 
			clrBtnText : m_clrRegular;
	
		if (m_bHighlighted && m_clrHover != clrDefault)
		{
			clrText = m_clrHover;
		}

		UINT uiDTFlags = DT_VCENTER | DT_SINGLELINE | DT_CENTER;

		if (uiState & ODS_DISABLED)
		{
			pDC->SetTextColor (clrBtnHilite);

			CRect rectShft = rectText;
			rectShft.OffsetRect (1, 1);
			pDC->DrawText (strText, rectShft, uiDTFlags);

			clrText = clrGrayedText;
		}

		pDC->SetTextColor (clrText);
		pDC->DrawText (strText, rectText, uiDTFlags);
	}

	//------------
	// Draw image:
	//------------
	if (!rectImage.IsRectEmpty () && m_nDisplayStyle != BUTTONSTYLE_TEXT)
	{
		UINT uiFlags = (uiState & ODS_DISABLED) == 0 ? 
			DSS_NORMAL : DSS_DISABLED;

		if (m_hIcon != NULL)
		{
			ASSERT (m_hBitmap == NULL);

			HBRUSH hbr = NULL;
			pDC->DrawState (rectImage.TopLeft (), m_sizeImage, 
				m_bHighlighted && m_hIconHot != NULL ? m_hIconHot : m_hIcon,
				uiFlags, hbr);
		}
		else
		{
			HBITMAP hbmp = m_hBitmap;
			if (uiState & ODS_DISABLED)
			{
				hbmp = m_hBitmapDisabled;
			}
			else if (m_bHighlighted && m_hBitmapHot != NULL)
			{
				hbmp = m_hBitmapHot;
			}

			ASSERT (hbmp != NULL);
			pDC->DrawState (rectImage.TopLeft (), m_sizeImage, hbmp, uiFlags);
		}
	}
}
//****************************************************************************
void CCoolButton::SetImage (HICON hIcon, BOOL bAutoDestroy, HICON hIconHot)
{
	CleanUp ();

	if (hIcon == NULL)
	{
		return;
	}

	m_hIcon = hIcon;
	m_hIconHot = hIconHot;
	m_bAutoDestroyImage = bAutoDestroy;

	ICONINFO info;
	::GetIconInfo (m_hIcon, &info);

	BITMAP bmp;
	::GetObject (info.hbmColor, sizeof (BITMAP), (LPVOID) &bmp);

	m_sizeImage.cx = bmp.bmWidth;
	m_sizeImage.cy = bmp.bmHeight;

	::DeleteObject (info.hbmColor);
	::DeleteObject (info.hbmMask);
}
//****************************************************************************
void CCoolButton::SetImage (HBITMAP hBitmap, BOOL bAutoDestroy, HBITMAP hBitmapHot)
{
	CleanUp ();

	if (hBitmap == NULL)
	{
		return;
	}

	m_hBitmap = hBitmap;
	m_hBitmapHot = hBitmapHot;
	m_bAutoDestroyImage = bAutoDestroy;

	BITMAP bmp;
	::GetObject (m_hBitmap, sizeof (BITMAP), (LPVOID) &bmp);

	m_sizeImage.cx = bmp.bmWidth;
	m_sizeImage.cy = bmp.bmHeight;

	CreateDisabledImage ();
}
//****************************************************************************
void CCoolButton::SetImage (UINT uiBmpResId, UINT uiBmpHotResId)
{
	CleanUp ();

	if (uiBmpResId == 0)
	{
		return;
	}

	HBITMAP hbmp = (HBITMAP) ::LoadImage (
		AfxGetResourceHandle (),
		MAKEINTRESOURCE (uiBmpResId),
		IMAGE_BITMAP,
		0, 0,
		LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
	ASSERT (hbmp != NULL);

	HBITMAP hbmpHot = NULL;
	if (uiBmpHotResId != 0)
	{
		hbmpHot = (HBITMAP) ::LoadImage (
			AfxGetResourceHandle (),
			MAKEINTRESOURCE (uiBmpHotResId),
			IMAGE_BITMAP,
			0, 0,
			LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
		ASSERT (hbmp != NULL);
	}

	SetImage (hbmp, TRUE /* AutoDestroy */, hbmpHot);
}

//****************************************************************************
void CCoolButton::OnCancelMode() 
{
	CButton::OnCancelMode();
	
	if (m_bCaptured)
	{
		ReleaseCapture ();
		m_bCaptured = FALSE;

		Invalidate ();
		UpdateWindow ();
	}
}
//****************************************************************************
void CCoolButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_bHover = FALSE;

	if ((nFlags & MK_LBUTTON) || m_nFlatStyle != BUTTONSTYLE_3D)
	{
		BOOL bRedraw = FALSE;

		CRect rectClient;
		GetClientRect (rectClient);

		if (rectClient.PtInRect (point))
		{
			m_bHover = TRUE;

			if (!m_bHighlighted)
			{
				m_bHighlighted = TRUE;
				bRedraw = TRUE;
			}

			if ((nFlags & MK_LBUTTON) && !m_bPushed)
			{
				m_bPushed = TRUE;
				bRedraw = TRUE;
			}

			if (!m_bCaptured)
			{
				SetCapture ();
				m_bCaptured = TRUE;
				bRedraw = TRUE;
			}
		}
		else
		{
			if (nFlags & MK_LBUTTON)
			{
				if (m_bPushed)
				{
					m_bPushed = FALSE;
					bRedraw = TRUE;
				}
			}
			else if (m_bHighlighted)
			{
				m_bHighlighted = FALSE;
				bRedraw = TRUE;
			}

			if (m_bCaptured && (!nFlags & MK_LBUTTON))
			{
				ReleaseCapture ();
				m_bCaptured = FALSE;

				bRedraw = TRUE;
			}
		}

		if (bRedraw)
		{
			Invalidate ();
			UpdateWindow ();
		}
	}
	
	CButton::OnMouseMove(nFlags, point);
}
//****************************************************************************
void CCoolButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bPushed = TRUE;
	m_bHighlighted = TRUE;

	if (!m_bCaptured)
	{
		SetCapture ();
		m_bCaptured = TRUE;
	}

	Invalidate ();
	UpdateWindow ();

	CButton::OnLButtonDown(nFlags, point);
}
//****************************************************************************
void CCoolButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bPushed = FALSE;
	m_bHighlighted = FALSE;

	Invalidate ();
	UpdateWindow ();

	CButton::OnLButtonUp(nFlags, point);

	if (m_bCaptured)
	{
		ReleaseCapture ();
		m_bCaptured = FALSE;
	}
}
//****************************************************************************
CSize CCoolButton::SizeToContent (BOOL bCalcOnly)
{
	ASSERT (GetSafeHwnd () != NULL);

	CClientDC dc (this);

	CString strText;
	GetWindowText (strText);
	CSize sizeText = dc.GetTextExtent (strText);

	int cx,cy;
	switch (m_nDisplayStyle)
	{
	case BUTTONSTYLE_IMAGE:
		cx = m_sizeImage.cx + nImageHorzMargin;
		cy = m_sizeImage.cy + nVertMargin * 2;
		break;
		
	case BUTTONSTYLE_TEXT:
		cx = sizeText.cx + nImageHorzMargin;
		cy = sizeText.cy + nVertMargin * 2;
		break;

	case BUTTONSTYLE_BOTH:
		cx = sizeText.cx + m_sizeImage.cx + nImageHorzMargin;
		if (sizeText.cx > 0)
		{
			cx += nImageHorzMargin;
		}

		cy = max (sizeText.cy, m_sizeImage.cy) + nVertMargin * 2;
		break;
	}

	if (!bCalcOnly)
	{
		SetWindowPos (NULL, -1, -1, cx, cy,
			SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}

	return CSize (cx, cy);
}
//****************************************************************************
BOOL CCoolButton::PreTranslateMessage(MSG* pMsg) 
{
	if (m_wndToolTip.GetSafeHwnd () != NULL)
	{
		if (pMsg->message == WM_LBUTTONDOWN ||
			pMsg->message == WM_LBUTTONUP ||
			pMsg->message == WM_MOUSEMOVE)
		{
			m_wndToolTip.RelayEvent(pMsg);
		}
	}
	
	return CButton::PreTranslateMessage(pMsg);
}
//****************************************************************************
void CCoolButton::SetTooltip (LPCTSTR lpszToolTipText)
{
	ASSERT (GetSafeHwnd () != NULL);

	if (lpszToolTipText == NULL)
	{
		if (m_wndToolTip.GetSafeHwnd () != NULL)
		{
			m_wndToolTip.Activate (FALSE);
		}
	}
	else
	{
		if (m_wndToolTip.GetSafeHwnd () != NULL)
		{
			m_wndToolTip.UpdateTipText (lpszToolTipText, this);
		}
		else
		{
			m_wndToolTip.Create (this, TTS_ALWAYSTIP);
			m_wndToolTip.AddTool (this, lpszToolTipText);
		}

		m_wndToolTip.Activate (TRUE);
	}
}

//*****************************************************************************
void CCoolButton::OnDrawFocusRect (CDC* pDC, const CRect& rectClient)
{
	ASSERT_VALID (pDC);

	CRect rectFocus = rectClient;

	if (m_nFlatStyle == BUTTONSTYLE_FLAT)
	{
		rectFocus.DeflateRect (2, 2);
	}
	else
	{
		rectFocus.DeflateRect (3, 3);
	}

	pDC->DrawFocusRect (rectFocus);
}
//******************************************************************************
BOOL CCoolButton::CreateDisabledImage ()
{
	if (m_hBitmapDisabled != NULL)
	{
		::DeleteObject (m_hBitmapDisabled);
		m_hBitmapDisabled = NULL;
	}

	if (m_hBitmap == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	//-------------------------------------------------------
	// Create memory source DC and select an original bitmap:
	//-------------------------------------------------------
	CDC memDCSrc;
	memDCSrc.CreateCompatibleDC (NULL);

	HBITMAP hOldBitmapSrc = NULL;

	int iBitmapWidth;
	int iBitmapHeight;

	//-------------------------------
	// Get original bitmap attrbutes:
	//-------------------------------
	BITMAP bmp;
	if (::GetObject (m_hBitmap, sizeof (BITMAP), &bmp) == 0)
	{
		return FALSE;
	}

	hOldBitmapSrc = (HBITMAP) memDCSrc.SelectObject (m_hBitmap);
	if (hOldBitmapSrc == NULL)
	{
		return FALSE;
	}

	iBitmapWidth = bmp.bmWidth;
	iBitmapHeight = bmp.bmHeight;

	//----------------------------------------------------------
	// Create a new bitmap compatibel with the source memory DC:
	// (original bitmap SHOULD BE ALREADY SELECTED!):
	//----------------------------------------------------------
	m_hBitmapDisabled = (HBITMAP) ::CreateCompatibleBitmap (memDCSrc,
									iBitmapWidth,
									iBitmapHeight);
	if (m_hBitmapDisabled == NULL)
	{
		memDCSrc.SelectObject (hOldBitmapSrc);
		return FALSE;
	}

	//------------------------------
	// Create memory destination DC:
	//------------------------------
	CDC memDCDst;
	memDCDst.CreateCompatibleDC (&memDCSrc);

	HBITMAP hOldBitmapDst = (HBITMAP) memDCDst.SelectObject (m_hBitmapDisabled);
	if (hOldBitmapDst == NULL)
	{
		memDCSrc.SelectObject (hOldBitmapSrc);
		::DeleteObject (m_hBitmapDisabled);
		m_hBitmapDisabled = NULL;
		return FALSE;
	}

	//-----------------------------
	// Copy original bitmap to new:
	//-----------------------------
	memDCDst.BitBlt (0, 0, iBitmapWidth, iBitmapHeight,
				&memDCSrc, 0, 0, SRCCOPY);
		
	//------------------------------------
	// Change "3dface" colors to White:
	//------------------------------------
	for (int x = 0; x < iBitmapWidth; x ++)
	{
		for (int y = 0; y < iBitmapHeight; y ++)
		{
			COLORREF clrOrig = ::GetPixel (memDCDst, x, y);
			COLORREF clrBtnFace;
			clrBtnFace = ::GetSysColor(COLOR_BTNFACE);
			COLORREF clrNew = (clrOrig == clrBtnFace) ?
				RGB (255, 255, 255) : clrOrig;

			if (clrOrig != clrNew)
			{
				::SetPixel (memDCDst, x, y, clrNew);
			}
		}
	}
	
	memDCDst.SelectObject (hOldBitmapDst);
	memDCSrc.SelectObject (hOldBitmapSrc);

	return TRUE;
}
//******************************************************************************
void CCoolButton::OnEnable(BOOL bEnable) 
{
	if (!bEnable)
	{
		// control disabled
		m_bPushed = FALSE;
		m_bHighlighted = FALSE;
		
		if (m_bCaptured)
		{
			ReleaseCapture ();
			m_bCaptured = FALSE;
		}
	}
	
	CButton::OnEnable(bEnable);
}
