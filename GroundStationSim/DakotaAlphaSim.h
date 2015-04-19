// DakotaAlphaSim.h : main header file for the DAKOTAALPHASIM application
//

#if !defined(AFX_DAKOTAALPHASIM_H__442A0AE0_83C2_402E_A8C0_B58C78EEDD07__INCLUDED_)
#define AFX_DAKOTAALPHASIM_H__442A0AE0_83C2_402E_A8C0_B58C78EEDD07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDakotaAlphaSimApp:
// See DakotaAlphaSim.cpp for the implementation of this class
//

class CDakotaAlphaSimApp : public CWinApp
{
public:
	CDakotaAlphaSimApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDakotaAlphaSimApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDakotaAlphaSimApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAKOTAALPHASIM_H__442A0AE0_83C2_402E_A8C0_B58C78EEDD07__INCLUDED_)
