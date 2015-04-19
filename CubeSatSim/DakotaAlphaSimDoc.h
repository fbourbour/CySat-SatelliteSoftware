// DakotaAlphaSimDoc.h : interface of the CDakotaAlphaSimDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAKOTAALPHASIMDOC_H__A88DF86B_98AF_48F8_96A6_716ABBDB1C3C__INCLUDED_)
#define AFX_DAKOTAALPHASIMDOC_H__A88DF86B_98AF_48F8_96A6_716ABBDB1C3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDakotaAlphaSimDoc : public COleDocument
{
protected: // create from serialization only
	CDakotaAlphaSimDoc();
	DECLARE_DYNCREATE(CDakotaAlphaSimDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDakotaAlphaSimDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDakotaAlphaSimDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDakotaAlphaSimDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAKOTAALPHASIMDOC_H__A88DF86B_98AF_48F8_96A6_716ABBDB1C3C__INCLUDED_)
