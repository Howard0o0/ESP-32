// STTStar.h : main header file for the STTSTAR application
//

#if !defined(AFX_STTSTAR_H__76A03E35_3C2F_4D8E_B0DB_DE03B1C84CD7__INCLUDED_)
#define AFX_STTSTAR_H__76A03E35_3C2F_4D8E_B0DB_DE03B1C84CD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSTTStarApp:
// See STTStar.cpp for the implementation of this class
//

class CSTTStarApp : public CWinApp
{
public:
	CSTTStarApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTTStarApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSTTStarApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STTSTAR_H__76A03E35_3C2F_4D8E_B0DB_DE03B1C84CD7__INCLUDED_)
