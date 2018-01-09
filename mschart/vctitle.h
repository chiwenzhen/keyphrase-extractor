#if !defined(AFX_VCTITLE_H__FD71D8BE_9A6A_459A_B927_ACBB11EC4DDC__INCLUDED_)
#define AFX_VCTITLE_H__FD71D8BE_9A6A_459A_B927_ACBB11EC4DDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CVcTextLayout;
class CVcLocation;
class CVcBackdrop;
class CVcFont;
class COleFont;

/////////////////////////////////////////////////////////////////////////////
// CVcTitle wrapper class

class CVcTitle : public COleDispatchDriver
{
public:
	CVcTitle() {}		// Calls COleDispatchDriver default constructor
	CVcTitle(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcTitle(const CVcTitle& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CVcTextLayout GetTextLayout();
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	CVcLocation GetLocation();
	CVcBackdrop GetBackdrop();
	CVcFont GetVtFont();
	short GetTextLength();
	void SetRefFont(LPDISPATCH newValue);
	COleFont GetFont();
	void Select();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCTITLE_H__FD71D8BE_9A6A_459A_B927_ACBB11EC4DDC__INCLUDED_)