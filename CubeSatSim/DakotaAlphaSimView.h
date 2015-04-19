// DakotaAlphaSimView.h : interface of the CDakotaAlphaSimView class
//
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES

#if !defined(AFX_DAKOTAALPHASIMVIEW_H__1BE3F520_B704_45BF_98AA_FFAA6C2B1BC6__INCLUDED_)
#define AFX_DAKOTAALPHASIMVIEW_H__1BE3F520_B704_45BF_98AA_FFAA6C2B1BC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HeliumSim.h"

class CDakotaAlphaSimCntrItem;

class CDakotaAlphaSimView : public CFormView
{
protected: // create from serialization only
	CDakotaAlphaSimView();
	DECLARE_DYNCREATE(CDakotaAlphaSimView)

public:
	//{{AFX_DATA(CDakotaAlphaSimView)
	enum { IDD = IDD_DAKOTAALPHASIM_FORM };
	CMSComm	m_comm1;
	UINT	m_BeaconInterval;
	UINT	m_BeaconIntervalCounter;
	BOOL	m_BeaconSent;
	CString	m_Attitude;
	CString	m_Orbit;
	CString	m_Power;
	CString	m_Temperature;
	CString	m_Message;
	BOOL	m_TransmitterOff;
	BOOL	m_TransmitterOn;
	CString	m_XaxisAcc;
	CString	m_YaxisAcc;
	CString	m_ZaxisAcc;
	CString	m_XaxisMag;
	CString	m_YaxisMag;
	CString	m_ZaxisMag;
	//}}AFX_DATA

// Attributes
public:
	CDakotaAlphaSimDoc* GetDocument();
	// m_pSelection holds the selection to the current CDakotaAlphaSimCntrItem.
	// For many applications, such a member variable isn't adequate to
	//  represent a selection, such as a multiple selection or a selection
	//  of objects that are not CDakotaAlphaSimCntrItem objects.  This selection
	//  mechanism is provided just to help you get started.

	// TODO: replace this selection mechanism with one appropriate to your app.
	CDakotaAlphaSimCntrItem* m_pSelection;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDakotaAlphaSimView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL IsSelected(const CObject* pDocItem) const;// Container support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDakotaAlphaSimView();
	void SendCommandToGroundStation(CString);
	void TransmitToGroundStation(CString data_out);
	void ProcessGroundStationCommand(char ch);
	void ParseHeader( uint8_t *pucData, heliumPacket *DakotaAlphaPacket );
	void ProcessRequest(char*);
    void vCalculateChecksum( uint8_t *pucData, uint16_t usSize, uint8_t *pucSumA, uint8_t *pucSumB );
	bool bBeaconStatus;

	
	

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDakotaAlphaSimView)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInsertObject();
	afx_msg void OnCancelEditCntr();
	afx_msg void OnCom1();
	afx_msg void OnPaint();
	afx_msg void OnButtonSend();
	afx_msg void OnBeaconOn();
	afx_msg void OnBeaconOff();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonSendImage();
	afx_msg void OnTransmitterOff();
	afx_msg void OnTransmitterOn();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DakotaAlphaSimView.cpp
inline CDakotaAlphaSimDoc* CDakotaAlphaSimView::GetDocument()
   { return (CDakotaAlphaSimDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAKOTAALPHASIMVIEW_H__1BE3F520_B704_45BF_98AA_FFAA6C2B1BC6__INCLUDED_)
