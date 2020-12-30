// BmpButton.cpp : implementation file
//

#include "stdafx.h"
#include "STTStar.h"
#include "BmpButton.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBmpButton

CBmpButton::CBmpButton()
{	
	m_pImage=NULL;
	m_pBmi=(LPBITMAPINFO)new BYTE[sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256];	
}

CBmpButton::~CBmpButton()
{
}


BEGIN_MESSAGE_MAP(CBmpButton, CButton)
	//{{AFX_MSG_MAP(CBmpButton)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBmpButton message handlers

void CBmpButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{

	int i;
	CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	ASSERT_VALID(pDC);
	CRect rect=lpDrawItemStruct->rcItem;	

	m_pBmi->bmiHeader.biSize=40L;
	m_pBmi->bmiHeader.biWidth=m_dwWidth;
	m_pBmi->bmiHeader.biHeight=m_dwHeight;
	m_pBmi->bmiHeader.biPlanes=1L;
	m_pBmi->bmiHeader.biBitCount=8L;
	m_pBmi->bmiHeader.biCompression=0L;
	m_pBmi->bmiHeader.biSizeImage=(long)(m_dwWidth*m_dwHeight);
	m_pBmi->bmiHeader.biXPelsPerMeter=0xbc;
	m_pBmi->bmiHeader.biYPelsPerMeter=0xbc;
	m_pBmi->bmiHeader.biClrUsed=(long)256L;
	m_pBmi->bmiHeader.biClrImportant=0L;
	for(i=0;i<256;i++)
	{
		m_pBmi->bmiColors[i].rgbRed=i;//255-32*i;
		m_pBmi->bmiColors[i].rgbGreen=i;//255-32*i;
		m_pBmi->bmiColors[i].rgbBlue=i;//255-32*i;
		m_pBmi->bmiColors[i].rgbReserved=0;
	}
	if(m_pImage!=NULL)
	{
		StretchDIBits( pDC->m_hDC, 0, 0, rect.Width(), rect.Height(),
			0, 0, m_dwWidth, m_dwHeight, m_pImage, (BITMAPINFO *) m_pBmi,
			BI_RGB, SRCCOPY );
	/*	StretchDIBits( pDC->m_hDC, 0, 0, m_dwWidth, m_dwHeight,
			0, 0, m_dwWidth, m_dwHeight, m_pImage, (BITMAPINFO *) m_pBmi,
			BI_RGB, SRCCOPY );
	*/
	}	
}

void CBmpButton::PreSubclassWindow() 
{

	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

int CBmpButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;	
		
	return 0;
}

void CBmpButton::OnDestroy() 
{
	CButton::OnDestroy();
	
	delete[] m_pBmi;	
}
