// KeyphraseExtraction.h : main header file for the KEYPHRASEEXTRACTION application
//

#if !defined(AFX_KEYPHRASEEXTRACTION_H__5579A735_867E_41DF_9C6B_D0683BA78691__INCLUDED_)
#define AFX_KEYPHRASEEXTRACTION_H__5579A735_867E_41DF_9C6B_D0683BA78691__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKeyphraseExtractionApp:
// See KeyphraseExtraction.cpp for the implementation of this class
//

class CKeyphraseExtractionApp : public CWinApp
{
public:
	CKeyphraseExtractionApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyphraseExtractionApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CKeyphraseExtractionApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYPHRASEEXTRACTION_H__5579A735_867E_41DF_9C6B_D0683BA78691__INCLUDED_)
