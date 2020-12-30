// STTStarDoc.cpp : implementation of the CSTTStarDoc class
//

#include "stdafx.h"
#include "STTStar.h"

#include "STTStarDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTTStarDoc

IMPLEMENT_DYNCREATE(CSTTStarDoc, CDocument)

BEGIN_MESSAGE_MAP(CSTTStarDoc, CDocument)
	//{{AFX_MSG_MAP(CSTTStarDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTTStarDoc construction/destruction

CSTTStarDoc::CSTTStarDoc()
{
	// TODO: add one-time construction code here

}

CSTTStarDoc::~CSTTStarDoc()
{
}

BOOL CSTTStarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSTTStarDoc serialization

void CSTTStarDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSTTStarDoc diagnostics

#ifdef _DEBUG
void CSTTStarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSTTStarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSTTStarDoc commands
