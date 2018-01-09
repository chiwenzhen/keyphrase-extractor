#if !defined(AFX_VCBRUSH_H__A5A31C7D_A589_48A0_89C9_BFFB0FFB27F2__INCLUDED_)
#define AFX_VCBRUSH_H__A5A31C7D_A589_48A0_89C9_BFFB0FFB27F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CVcColor;

/////////////////////////////////////////////////////////////////////////////
// CVcBrush wrapper class

class CVcBrush : public COleDispatchDriver
{
public:
	CVcBrush() {}		// Calls COleDispatchDriver default constructor
	CVcBrush(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcBrush(const CVcBrush& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetStyle();
	void SetStyle(long nNewValue);
	CVcColor GetFillColor();
	CVcColor GetPatternColor();
	long GetIndex();
	void SetIndex(long nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCBRUSH_H__A5A31C7D_A589_48A0_89C9_BFFB0FFB27F2__INCLUDED_)