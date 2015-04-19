// CntrItem.h : interface of the CDakotaAlphaSimCntrItem class
//

#if !defined(AFX_CNTRITEM_H__0356AA05_5FD1_44E4_98E8_3C0E93FC43E8__INCLUDED_)
#define AFX_CNTRITEM_H__0356AA05_5FD1_44E4_98E8_3C0E93FC43E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDakotaAlphaSimDoc;
class CDakotaAlphaSimView;

class CDakotaAlphaSimCntrItem : public COleClientItem
{
	DECLARE_SERIAL(CDakotaAlphaSimCntrItem)

// Constructors
public:
	CDakotaAlphaSimCntrItem(CDakotaAlphaSimDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CDakotaAlphaSimDoc* GetDocument()
		{ return (CDakotaAlphaSimDoc*)COleClientItem::GetDocument(); }
	CDakotaAlphaSimView* GetActiveView()
		{ return (CDakotaAlphaSimView*)COleClientItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDakotaAlphaSimCntrItem)
	public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();
	protected:
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CDakotaAlphaSimCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__0356AA05_5FD1_44E4_98E8_3C0E93FC43E8__INCLUDED_)
