// KeyphraseExtractionDlg.h : header file
//
//{{AFX_INCLUDES()
#include "mschart\mschart.h"
//}}AFX_INCLUDES

#if !defined(AFX_KEYPHRASEEXTRACTIONDLG_H__8FDF0FD7_9DA5_4089_89F9_35A672F19018__INCLUDED_)
#define AFX_KEYPHRASEEXTRACTIONDLG_H__8FDF0FD7_9DA5_4089_89F9_35A672F19018__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKeyphraseExtractionDlg dialog

#include "Extraction.h"
#include "SortListCtrl.h"
#include "mschart/mschart.h"
#include "mschart/VcPlot.h"
#include "mschart/VcAxis.h"
#include "mschart/VcValueScale.h"
#include "mschart/VcSeriesCollection.h"
#include "mschart/VcSeries.h"
#include "mschart/VcPen.h"
#include "mschart/VcCategoryScale.h"
#include "mschart/VcColor.h"
#include "mschart/VcDataGrid.h"
#include "mschart/VcBackdrop.h"
#include "mschart/VcFill.h"
#include "mschart/VcBrush.h"
#include "mschart/VcDataPoints.h"
#include "mschart/VcDataPoint.h"
#include "mschart/VcDataPointLabel.h"
#include "mschart/VcAxisTitle.h"
#include "mschart/vctextlayout.h"

class CKeyphraseExtractionDlg : public CDialog
{
private:
	CMSChart m_Chart;
	Extraction extract;
	char strFilePath[256]; //所要抽取关键词的文件名
	float precision[50];
	float recall[50];
	wchar_t curSelectedColumnName[64]; // 当前选中列的名称
	int lastSelectedColumn;
	COLORREF defaultBkColor;
	COLORREF defaultTextColor;
public:	
	int UpdateListCtrl(Unit ** SortBuffer, int SortBufferSize);

// Construction
public:
	CKeyphraseExtractionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CKeyphraseExtractionDlg)
	enum { IDD = IDD_KEYPHRASEEXTRACTION_DIALOG };
	CSortListCtrl	m_cListCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyphraseExtractionDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CKeyphraseExtractionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpenFile();
	afx_msg void OnExtract();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnColumnclickListctrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnExtractC();
	afx_msg void OnExtractCL();
	afx_msg void OnExtractCG();
	afx_msg void OnExtractCLG();
	afx_msg void OnExtractCLGS();
	afx_msg void OnExtractCF();
	afx_msg void OnExtractCFG();
	afx_msg void OnExtractCFGS();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYPHRASEEXTRACTIONDLG_H__8FDF0FD7_9DA5_4089_89F9_35A672F19018__INCLUDED_)
