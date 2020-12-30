// STTStarDoc.h : interface of the CSTTStarDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STTSTARDOC_H__1F6BDE7E_964F_4173_8F2D_9E2E4AF7D76A__INCLUDED_)
#define AFX_STTSTARDOC_H__1F6BDE7E_964F_4173_8F2D_9E2E4AF7D76A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSTTStarDoc : public CDocument
{
protected: // create from serialization only
	CSTTStarDoc();
	DECLARE_DYNCREATE(CSTTStarDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTTStarDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSTTStarDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSTTStarDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STTSTARDOC_H__1F6BDE7E_964F_4173_8F2D_9E2E4AF7D76A__INCLUDED_)
