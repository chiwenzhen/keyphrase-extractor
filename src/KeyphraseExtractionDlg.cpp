// KeyphraseExtractionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KeyphraseExtraction.h"
#include "KeyphraseExtractionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyphraseExtractionDlg dialog

CKeyphraseExtractionDlg::CKeyphraseExtractionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyphraseExtractionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyphraseExtractionDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	memset(strFilePath, 0, sizeof(strFilePath));
	strcpy(strFilePath, "DataStructure.txt");
	lastSelectedColumn = 11;
	wcscpy(curSelectedColumnName, _T("termhood")); //初始化按照termhood 排序
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeyphraseExtractionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyphraseExtractionDlg)1
	DDX_Control(pDX, IDC_LIST, m_cListCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKeyphraseExtractionDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyphraseExtractionDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_FILE, OnOpenFile)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LISTCTRL, OnColumnclickListctrl)
	ON_BN_CLICKED(IDC_EXTRACT_C, OnExtractC)
	ON_BN_CLICKED(IDC_EXTRACT_CL, OnExtractCL)
	ON_BN_CLICKED(IDC_EXTRACT_CG, OnExtractCG)
	ON_BN_CLICKED(IDC_EXTRACT_CLG, OnExtractCLG)
	ON_BN_CLICKED(IDC_EXTRACT_CLGS, OnExtractCLGS)
	ON_BN_CLICKED(IDC_EXTRACT_CF, OnExtractCF)
	ON_BN_CLICKED(IDC_EXTRACT_CFG, OnExtractCFG)
	ON_BN_CLICKED(IDC_EXTRACT_CFGS, OnExtractCFGS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CKeyphraseExtractionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKeyphraseExtractionDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKeyphraseExtractionDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CKeyphraseExtractionDlg::OnOpenFile() 
{
	CString csfilePath;
	CFileDialog fileDlg(TRUE, NULL,NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, _T("文本文件 (*.txt)|*.txt||"), NULL);
	if (fileDlg.DoModal() == IDOK) {
		csfilePath = fileDlg.GetPathName();
		WideCharToMultiByte(CP_ACP, 0, csfilePath.GetBuffer(0), -1, strFilePath, 256, NULL, NULL);
	}
}



/////////////////////////////////////////////////////////////////////////////
// CKeyphraseExtractionDlg message handlers

BOOL CKeyphraseExtractionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	//初始化ListCtrl
	CRect rect;	
	DWORD exstyle = m_cListCtrl.GetExtendedStyle();
    m_cListCtrl.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	m_cListCtrl.ShowWindow(SW_SHOW);
	m_cListCtrl.GetClientRect(&rect);
	int weight = rect.Width();
	int height = rect.Height();

	CString strHeadings;
	strHeadings.Format(_T("序号,%d;词语,%d;频率,%d;被嵌套频率,%d;长度,%d;多句孤独簇,%d;\
		单段孤独簇,%d;多段孤独簇,%d;句子数(多句),%d;句子数(单段),%d;句子数(多段),%d;Termhood,%d;"), 
		weight * 40.0 / 960, weight * 100.0 / 960, weight * 50.0 / 960, weight * 80.0 / 960,
		weight * 60.0 / 960, weight * 95.0 / 960, weight * 95.0 / 960, weight * 95.0 / 960, 
		weight * 95.0 / 960, weight * 95.0 / 960, weight * 95.0 / 960, weight * 70.0 / 960);
	m_cListCtrl.SetHeadings(strHeadings);    //设置列头
	defaultBkColor = m_cListCtrl.GetBkColor();
	defaultTextColor = m_cListCtrl.GetTextColor();//保存默认色彩

	//初始化mschart
	m_Chart.Create(_T("mschart"), WS_CHILD, rect, this, ID_MSCHART);
//	InitChart();

	//将x轴步长选项隐藏
//	GetDlgItem(IDC_XSTEP)->ShowWindow(FALSE);
//	GetDlgItem(IDC_STATIC_XSTEP)->ShowWindow(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}




int CKeyphraseExtractionDlg::UpdateListCtrl(Unit ** SortBuffer, int SortBufferSize)
{
	int i, j;
	
	m_cListCtrl.DeleteAllItems();
	for (i=0; i<SortBufferSize; i++) {	
		wchar_t str[50] = {0};
		wchar_t strValue[11][30] = {0};
		for ( j = 0; j < SortBuffer[i]->wordCount; j++ ) wcscat(str, SortBuffer[i]->pTerm[j]);

		m_cListCtrl.AddItem(
			_itow(i+1, strValue[0], 10), 
			str, 
			_itow(SortBuffer[i]->frequency, strValue[1], 10),
			_itow(SortBuffer[i]->nestedFreq, strValue[2], 10), 
			_itow(SortBuffer[i]->charCount, strValue[3], 10), //字数
			_itow(SortBuffer[i]->uMultiSentenceLoneM, strValue[4], 10), 
			_itow(SortBuffer[i]->uParagraphLoneM, strValue[5], 10),
			_itow(SortBuffer[i]->uMultiParagraphLoneM, strValue[6], 10), 
			_itow(SortBuffer[i]->uMultiStClusterStM, strValue[7], 10),
			_itow(SortBuffer[i]->uParaClusterParaM, strValue[8], 10), 
			_itow(SortBuffer[i]->uMultiParaClusterParaM, strValue[9], 10),
			_itow((int)(SortBuffer[i]->termhood), strValue[10], 10));	
	}
	return 0;
}

void CKeyphraseExtractionDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if (m_cListCtrl.GetSafeHwnd() != NULL) {
		m_cListCtrl.SetRedraw(FALSE); 
		int weight;
		CRect rect;
		this->GetClientRect(&rect);
		m_cListCtrl.SetWindowPos(NULL, 180, 60, rect.Width()-180, rect.Height()-60, NULL);
		weight = rect.Width() - 180; //右边留200像素，显示统计数据
		
		m_cListCtrl.SetColumnWidth(0,  weight * 45.0  / 960);
		m_cListCtrl.SetColumnWidth(1,  weight * 110.0 / 960);
		m_cListCtrl.SetColumnWidth(2,  weight * 45.0  / 960);
		m_cListCtrl.SetColumnWidth(3,  weight * 90.0  / 960);
		m_cListCtrl.SetColumnWidth(4,  weight * 60.0  / 960);
		m_cListCtrl.SetColumnWidth(5,  weight * 90.0  / 960);
		m_cListCtrl.SetColumnWidth(6,  weight * 90.0  / 960);
		m_cListCtrl.SetColumnWidth(7,  weight * 90.0  / 960);
		m_cListCtrl.SetColumnWidth(8,  weight * 90.0  / 960);
		m_cListCtrl.SetColumnWidth(9,  weight * 90.0  / 960);
		m_cListCtrl.SetColumnWidth(10, weight * 90.0  / 960);
		m_cListCtrl.SetColumnWidth(11, weight * 70.0  / 960);
		m_cListCtrl.SetRedraw(TRUE); 
	}
	if(m_Chart.GetSafeHwnd()) {
		m_Chart.MoveWindow( 180, 60, cx-180, cy-60 ); 
	}
}


void CKeyphraseExtractionDlg::OnColumnclickListctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_cListCtrl.GetItemCount() > 0) {
		int i;
		TCHAR columnName[256];
		HDITEM hdi; 
		hdi.mask = HDI_TEXT; 
		hdi.pszText = columnName; 
		hdi.cchTextMax = 256;
		int itemCount = m_cListCtrl.GetItemCount();
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

		//获取列标题
		m_cListCtrl.GetHeaderCtrl()->GetItem(pNMListView->iSubItem, &hdi);//获取列标题句柄hdi
		wcscpy(curSelectedColumnName, columnName);

		if (wcscmp(curSelectedColumnName, _T("序号")) != 0) { //序号这一列，不进行排序
			//排序
			m_cListCtrl.SetRedraw(FALSE); 
			m_cListCtrl.Sort(pNMListView->iSubItem, FALSE);
			
			//设置颜色
			for (i=0; i<itemCount; i++) {
				m_cListCtrl.SetItemColor(i, lastSelectedColumn, defaultTextColor, defaultBkColor); //上一次排序的列置为初始颜色
				m_cListCtrl.SetItemText(i, 0, _itow(i+1, columnName, 10));
			}
			for (i=0; i<itemCount; i++) {
				m_cListCtrl.SetItemColor(i, pNMListView->iSubItem, RGB(0, 0, 64), RGB(152, 226, 69));
			}
			lastSelectedColumn = pNMListView->iSubItem;
			m_cListCtrl.SetRedraw(TRUE); 
			//计算精确度和召回率
			UpdateData(TRUE);
			UpdateData(FALSE);
//			DrawChart();
		}
	}
	*pResult = 0;
}


BOOL CKeyphraseExtractionDlg::PreTranslateMessage(MSG* pMsg) {

	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) { 
		if (GetFocus()->GetDlgCtrlID() == IDC_INTERCEPT) { 
			UpdateData(TRUE);
			UpdateData(FALSE);
		}
		else if (GetFocus()->GetDlgCtrlID() == IDC_XSTEP) {
//			DrawChart();
		}
		return   TRUE; 
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CKeyphraseExtractionDlg::OnExtractC()
{
	UpdateData(true);
	m_cListCtrl.SetRedraw(FALSE); 
	if (strlen(strFilePath) != 0) {
		int ret = extract.ProcessC(strFilePath);
		if (ret == -1) {
			MessageBox(_T("要求文件编码为 ANSI 格式!"),  _T("提示"));
			return;
		}
		else if (ret == -2) {
			MessageBox(_T("ICTCLAS分词工具出错!"),  _T("错误"));
			return;
		}
		if (extract.SortBufferSize > 0) {
			UpdateListCtrl(extract.SortBuffer, extract.SortBufferSize); //显示所有抽取出来的关键词
			extract.StoreIntoDatabase(_T("C")); //关键词组存入数据库
			extract.data.Analysis(_T("C"));
		}
		else MessageBox(_T("文本过小，不适合进行关键词抽取!"), _T("提示"));
		
	}
	else MessageBox(_T("请先打开一个文件!"),  _T("提示"));
	m_cListCtrl.SetRedraw(TRUE); 
	UpdateData(false);
}

void CKeyphraseExtractionDlg::OnExtractCL() 
{
	UpdateData(true);
	m_cListCtrl.SetRedraw(FALSE); 
	if (strlen(strFilePath) != 0) {
		int ret = extract.ProcessCL(strFilePath);
		if (ret == -1) {
			MessageBox(_T("要求文件编码为 ANSI 格式!"),  _T("提示"));
			return;
		}
		else if (ret == -2) {
			MessageBox(_T("ICTCLAS分词工具出错!"),  _T("错误"));
			return;
		}
		if (extract.SortBufferSize > 0) {
			UpdateListCtrl(extract.SortBuffer, extract.SortBufferSize); //显示所有抽取出来的关键词
			extract.StoreIntoDatabase(_T("CL")); //关键词组存入数据库
			extract.data.Analysis(_T("CL"));
		}
		else MessageBox(_T("文本过小，不适合进行关键词抽取!"), _T("提示"));
		
	}
	else MessageBox(_T("请先打开一个文件!"),  _T("提示"));
	m_cListCtrl.SetRedraw(TRUE); 
	UpdateData(false);	
}

void CKeyphraseExtractionDlg::OnExtractCF() 
{
	UpdateData(true);
	m_cListCtrl.SetRedraw(FALSE); 
	if (strlen(strFilePath) != 0) {
		int ret = extract.ProcessCF(strFilePath);
		if (ret == -1) {
			MessageBox(_T("要求文件编码为 ANSI 格式!"),  _T("提示"));
			return;
		}
		else if (ret == -2) {
			MessageBox(_T("ICTCLAS分词工具出错!"),  _T("错误"));
			return;
		}
		if (extract.SortBufferSize > 0) {
			UpdateListCtrl(extract.SortBuffer, extract.SortBufferSize); //显示所有抽取出来的关键词
			extract.StoreIntoDatabase(_T("CF")); //关键词组存入数据库
			extract.data.Analysis(_T("CF"));
		}
		else MessageBox(_T("文本过小，不适合进行关键词抽取!"), _T("提示"));
		
	}
	else MessageBox(_T("请先打开一个文件!"),  _T("提示"));
	m_cListCtrl.SetRedraw(TRUE); 
	UpdateData(false);	
}

void CKeyphraseExtractionDlg::OnExtractCG() 
{
	UpdateData(true);
	m_cListCtrl.SetRedraw(FALSE); 
	if (strlen(strFilePath) != 0) {
		int ret = extract.ProcessCG(strFilePath);
		if (ret == -1) {
			MessageBox(_T("要求文件编码为 ANSI 格式!"),  _T("提示"));
			return;
		}
		else if (ret == -2) {
			MessageBox(_T("ICTCLAS分词工具出错!"),  _T("错误"));
			return;
		}
		if (extract.SortBufferSize > 0) {
			UpdateListCtrl(extract.SortBuffer, extract.SortBufferSize); //显示所有抽取出来的关键词
			extract.StoreIntoDatabase(_T("CG")); //关键词组存入数据库
			extract.data.Analysis(_T("CG"));
		}
		else MessageBox(_T("文本过小，不适合进行关键词抽取!"), _T("提示"));
		
	}
	else MessageBox(_T("请先打开一个文件!"),  _T("提示"));
	m_cListCtrl.SetRedraw(TRUE); 
	UpdateData(false);
	
}

void CKeyphraseExtractionDlg::OnExtractCLG() 
{
	UpdateData(true);
	m_cListCtrl.SetRedraw(FALSE); 
	if (strlen(strFilePath) != 0) {
		int ret = extract.ProcessCLG(strFilePath);
		if (ret == -1) {
			MessageBox(_T("要求文件编码为 ANSI 格式!"),  _T("提示"));
			return;
		}
		else if (ret == -2) {
			MessageBox(_T("ICTCLAS分词工具出错!"),  _T("错误"));
			return;
		}
		if (extract.SortBufferSize > 0) {
			UpdateListCtrl(extract.SortBuffer, extract.SortBufferSize); //显示所有抽取出来的关键词
			extract.StoreIntoDatabase(_T("CLG")); //关键词组存入数据库
			extract.data.Analysis(_T("CLG"));
		}
		else MessageBox(_T("文本过小，不适合进行关键词抽取!"), _T("提示"));
		
	}
	else MessageBox(_T("请先打开一个文件!"),  _T("提示"));
	m_cListCtrl.SetRedraw(TRUE); 
	UpdateData(false);
	
}


void CKeyphraseExtractionDlg::OnExtractCFG() 
{
	UpdateData(true);
	m_cListCtrl.SetRedraw(FALSE); 
	if (strlen(strFilePath) != 0) {
		int ret = extract.ProcessCFG(strFilePath);
		if (ret == -1) {
			MessageBox(_T("要求文件编码为 ANSI 格式!"),  _T("提示"));
			return;
		}
		else if (ret == -2) {
			MessageBox(_T("ICTCLAS分词工具出错!"),  _T("错误"));
			return;
		}
		if (extract.SortBufferSize > 0) {
			UpdateListCtrl(extract.SortBuffer, extract.SortBufferSize); //显示所有抽取出来的关键词
			extract.StoreIntoDatabase(_T("CFG")); //关键词组存入数据库
			extract.data.Analysis(_T("CFG"));
		}
		else MessageBox(_T("文本过小，不适合进行关键词抽取!"), _T("提示"));
		
	}
	else MessageBox(_T("请先打开一个文件!"),  _T("提示"));
	m_cListCtrl.SetRedraw(TRUE); 
	UpdateData(false);
	
}

void CKeyphraseExtractionDlg::OnExtractCLGS() 
{
	UpdateData(true);
	m_cListCtrl.SetRedraw(FALSE); 
	if (strlen(strFilePath) != 0) {
		int ret = extract.ProcessCLGS(strFilePath);
		if (ret == -1) {
			MessageBox(_T("要求文件编码为 ANSI 格式!"),  _T("提示"));
			return;
		}
		else if (ret == -2) {
			MessageBox(_T("ICTCLAS分词工具出错!"),  _T("错误"));
			return;
		}
		if (extract.SortBufferSize > 0) {
			UpdateListCtrl(extract.SortBuffer, extract.SortBufferSize); //显示所有抽取出来的关键词
			extract.StoreIntoDatabase(_T("CLGS")); //关键词组存入数据库
			extract.data.Analysis(_T("CLGS"));
		}
		else MessageBox(_T("文本过小，不适合进行关键词抽取!"), _T("提示"));
		
	}
	else MessageBox(_T("请先打开一个文件!"),  _T("提示"));
	m_cListCtrl.SetRedraw(TRUE); 
	UpdateData(false);
}


void CKeyphraseExtractionDlg::OnExtractCFGS() 
{
	UpdateData(true);
	m_cListCtrl.SetRedraw(FALSE); 
	if (strlen(strFilePath) != 0) {
		int ret = extract.ProcessCFGS(strFilePath);
		if (ret == -1) {
			MessageBox(_T("要求文件编码为 ANSI 格式!"),  _T("提示"));
			return;
		}
		else if (ret == -2) {
			MessageBox(_T("ICTCLAS分词工具出错!"),  _T("错误"));
			return;
		}
		if (extract.SortBufferSize > 0) {
			UpdateListCtrl(extract.SortBuffer, extract.SortBufferSize); //显示所有抽取出来的关键词
			extract.StoreIntoDatabase(_T("CFGS")); //关键词组存入数据库
			extract.data.Analysis(_T("CFGS"));
		}
		else MessageBox(_T("文本过小，不适合进行关键词抽取!"), _T("提示"));
		
	}
	else MessageBox(_T("请先打开一个文件!"),  _T("提示"));
	m_cListCtrl.SetRedraw(TRUE); 
	UpdateData(false);
}


