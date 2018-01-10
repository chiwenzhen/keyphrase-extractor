#include "stdafx.h"
#include "Data.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

/************************************************************************/
/* 构造函数                                                             */
/************************************************************************/
Data::Data() {
	int i;
	_variant_t var;
	CString strValue;
	try {
		// 初始化
		AfxOleInit();
		m_pConnection.CreateInstance("ADODB.Connection");
		m_pConnection-> CursorLocation=adUseClient; //游标类型,设置可以读取记录个数，否则不设的话记录数总是-1
		m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=TermExtraction.mdb","","",adModeUnknown);
		m_pRecordset.CreateInstance("ADODB.Recordset");

		//读取StopList
		i= 0;
		m_pRecordset->Open("select * from StopList", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		iStopWordCount = m_pRecordset->RecordCount;
		StopList = new wchar_t[iStopWordCount][5];
		if(!m_pRecordset->BOF) m_pRecordset->MoveFirst();
		while(!m_pRecordset->adoEOF) {
			var = m_pRecordset->GetCollect("词语");
			if(var.vt != VT_NULL) strValue = (LPCWSTR)_bstr_t(var);
			wcscpy(StopList[i++], strValue.GetBuffer(0));
			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
		QuickSort( StopList, 0, iStopWordCount - 1 );

		//读取MeasureWordList
		i = 0;
		m_pRecordset->Open("select * from MeasureWordList", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		iMeasureWordCount = m_pRecordset->RecordCount;
		MeasureWordList = new wchar_t[iMeasureWordCount][5];
		if(!m_pRecordset->BOF) m_pRecordset->MoveFirst();
		while(!m_pRecordset->adoEOF) {
			var = m_pRecordset->GetCollect("词语");
			if(var.vt != VT_NULL) strValue = (LPCWSTR)_bstr_t(var);
			wcscpy(MeasureWordList[i++], strValue.GetBuffer(0));
			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
		QuickSort( MeasureWordList, 0, iMeasureWordCount - 1 );

		//读取通用词汇表以及频率
		m_pRecordset->Open("select * from 词汇表", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		if(!m_pRecordset->BOF) m_pRecordset->MoveFirst();
		while(!m_pRecordset->adoEOF) {
			var = m_pRecordset->GetCollect("词语");
			strValue = (LPCWSTR)_bstr_t(var);
			var = m_pRecordset->GetCollect("出现次数");
			CommWordTable[strValue.GetBuffer(0)] = var.intVal; //存入哈希表
			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
	}
	catch (_com_error e){  
		::MessageBox(NULL,e.ErrorMessage(),_T("数据库读取错误"),MB_ICONEXCLAMATION);
	}
}

/************************************************************************/
/* 词语在通用语料库中的频率                                             */
/************************************************************************/
double Data::GetFrequency(wchar_t *str) {
	hash_map<wstring, int>::iterator it, iend = CommWordTable.end();
	if ((it=CommWordTable.find(str)) != iend) {
		return (double)(it->second);
	}
	else return 0.1;
}

/************************************************************************/
/* 析构函数                                                             */
/************************************************************************/
Data::~Data(){ 
	if(m_pConnection->State) m_pConnection->Close(); 
	m_pConnection= NULL;
	if (StopList != NULL) delete[] StopList; 
	StopList = NULL;
	if (MeasureWordList != NULL) delete[] MeasureWordList;
	MeasureWordList = NULL;
}

/************************************************************************/
/* 比较wStr1和wStr2的大小,相同返回0，wStr1>wStr2返回1，否则返回-1       */     
/************************************************************************/
int Data::WideStrCmpn(wchar_t * wStr1,int uStrLen1,wchar_t * wStr2, int uStrLen2) { //字符串比较
	UINT i,uMinLen;
	uMinLen = (uStrLen1<uStrLen2)? uStrLen1 : uStrLen2;
	for( i = 0; i< uMinLen; i++ ) {
		if( wStr1[i] < wStr2[i] ) return -1;
		if( wStr1[i] > wStr2[i] ) return 1;
	}
	if( uStrLen1 < uStrLen2 ) return -1;
	if( uStrLen1 > uStrLen2 ) return 1;
	return 0;
}

/************************************************************************/
/* 词组str是否为数量词                                                  */
/************************************************************************/
bool Data::IsQuantifier( wchar_t **str, int len ) { //是否是数量词
	int i, strLen;
	wchar_t phrase[30] = {0};
	for ( i = 0; i < len; i++) {
		wcscat(phrase, str[i]);
	}
	strLen = wcslen(phrase);
	if ( strLen == 1 ) {
		if (IsDigitChar(phrase[0]) || IsInBadList(MEASURE_LIST, &phrase[strLen -1], 1)) return true; //如果只是一个数字，没有意义，舍弃
		else return false;
	}
	else {
		//如果倒数第二个字是数字，而且倒数第一字是量词，那么这个短语被认为是一个数量词短语
//		if ( IsDigitChar(phrase[strLen -2]) && IsInBadList(MEASURE_LIST, &phrase[strLen -1], 1) ) return true;
		if ( IsDigitChar(phrase[0]) && IsInBadList(MEASURE_LIST, &phrase[1], 1) ) return true;
		else return false;
	}
}

/************************************************************************/
/* 词组str是否为停用词                                                  */
/************************************************************************/
bool Data::IsInStopList(wchar_t **str, int len) {
	int i;
	wchar_t phrase[30] = {0};
	for ( i = 0; i < len; i++) {
		wcscat(phrase, str[i]);
	}
	return IsInBadList(STOP_LIST, phrase, wcslen(phrase));
}

/************************************************************************/
/* 词组str是否在停用词表或数量词表中                                    */
/************************************************************************/
bool Data::IsInBadList(int whichList, wchar_t *wStr, int strLen) {//用二分法在StopList/MearsureList中查找给定的词语是否是停用词
	int i, iWordCount, low, mid, high;
	wchar_t (*List)[5];
	if ( wStr == NULL ) return false;
	List = ( (whichList == STOP_LIST) ? StopList : MeasureWordList );
	iWordCount = ( (whichList == STOP_LIST) ? iStopWordCount : iMeasureWordCount );

	low = 0 ;
	high = iWordCount - 1;
	while( low <=high ) {
		mid = (low+high)/2;
		i=WideStrCmpn( wStr, strLen, List[mid], wcslen(List[mid]));
		if( i == 0 ) return true;
		if( i < 0 ) high = mid - 1;
		else low = mid + 1;
	}	
	return false;
}

int Data::Partition( wchar_t (*StopList)[5], int low, int high) { //快速排序的Partition
	wchar_t pivot[5] = {0};
	wcscpy(pivot, StopList[low]);

	while ( low < high) {
		while ( low < high && 
			WideStrCmpn( StopList[high], wcslen(StopList[high]), pivot, wcslen(pivot) ) >= 0 ) 
			high--;
		wcscpy(StopList[low], StopList[high]);

		while ( low < high && 
			WideStrCmpn(  StopList[low], wcslen(StopList[low]), pivot, wcslen(pivot) ) <= 0 ) 
			low++;
		wcscpy(StopList[high], StopList[low]);
	}
	wcscpy(StopList[low], pivot);
	return low;
}

void Data::QuickSort( wchar_t (*StopList)[5], int iBegin, int iEnd) { //快速排序
	int pivotLoc;
	if ( iBegin < iEnd) {
		pivotLoc = Partition(StopList, iBegin, iEnd);
		QuickSort(StopList, iBegin, pivotLoc - 1);
		QuickSort(StopList, pivotLoc + 1, iEnd);
	}
	return;
}

/************************************************************************/
/* 将数据存入数据库中                                                   */
/************************************************************************/
void Data::StoreIntoDatabase(Unit *buffer[], int len, wchar_t view[]) {
	int i, j;
	CString sql;
	wchar_t deleteCmd[100];
	this->buffer = buffer;
	this->len = len;
	try {
		m_pCommand.CreateInstance("ADODB.Command");
		m_pCommand->ActiveConnection = m_pConnection;  // 将库连接赋于它
		wsprintf(deleteCmd, _T("delete * from %s"), view);
		m_pCommand->CommandText = deleteCmd;  // SQL语句
		m_pCommand->Execute(NULL, NULL,adCmdText);

		for ( i = 0; i < len; i++ ) {
			wchar_t term[50] = {0};
			for ( j = 0; j < buffer[i]->wordCount; j++ ) wcscat(term, buffer[i]->pTerm[j]);
			sql.Format(_T("insert into %s values ('%s', '%s',  %d, %d, %d, %d, %d, %d, %d, %d, %f, %f,  %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)"), 
				view,
				term, 
				_T(" "),
				buffer[i]->frequency,
				buffer[i]->nestedFreq,
				buffer[i]->uMultiSentenceLoneM,
				buffer[i]->uParagraphLoneM,
				buffer[i]->uMultiParagraphLoneM,
				buffer[i]->uMultiStClusterStM,
				buffer[i]->uParaClusterParaM,
				buffer[i]->uMultiParaClusterParaM,
				buffer[i]->strange,	
				buffer[i]->termhood,
				buffer[i]->cvalueRank,
				buffer[i]->F1Rank,
				buffer[i]->F2Rank,
				buffer[i]->F3Rank,
				buffer[i]->LR1Rank,
				buffer[i]->LR2Rank,
				buffer[i]->LR3Rank,
				buffer[i]->GR1Rank,
				buffer[i]->GR2Rank,
				buffer[i]->GR3Rank,
				buffer[i]->sinRank);
			m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText); 			
		}
	}
	catch (_com_error e){  
		::MessageBox(NULL,e.ErrorMessage(),_T("数据库写入错误"),MB_ICONEXCLAMATION);
	}

}


//nIntercept截取的记录数，keywordNum截取到的关键词数，stKeywordNum标准关键词总数，precision精确率，recall召回率，sortColumnName进行排序的列名
void Data::GetStatistics(wchar_t view[], int nIntercept, float *precision, float *recall, float *quality) {
	_variant_t var, ra;
	CString sql;
	int keywordTotal, keywordFound;
	try {
		//term专业关键词表中的关键词总个数
		sql.Format(_T("select count(*) as 标准词语总数 from term"));
		m_pRecordset = m_pConnection->Execute((_bstr_t)sql,&ra,adCmdText);
		var = m_pRecordset->GetCollect("标准词语总数");
		keywordTotal = var.intVal;

		//有多少候选关键词
		sql.Format(_T("select count(*) as 候选词语总数 from %s"), view);
		m_pRecordset = m_pConnection->Execute((_bstr_t)sql,&ra,adCmdText);
		var = m_pRecordset->GetCollect("候选词语总数");
		
		//前nIntercept候选关键词中有多少实际关键词
		sql.Format(_T("select count(词语) as 已找到关键词数 from (select top %d 词语 from %s order by Termhood desc) as ncwz, term where 词语=term.词组"), nIntercept, view);
		m_pRecordset = m_pConnection->Execute((_bstr_t)sql,&ra,adCmdText); 
		var = m_pRecordset->GetCollect("已找到关键词数");
		keywordFound = var.intVal;

		//精确率
		*precision = 1.0 * keywordFound / nIntercept;

		//召回率
		*recall = 1.0 * keywordFound / keywordTotal;

		//抽取质量
		*quality = Quality(view, nIntercept);
	}
	catch (_com_error e){  
		::MessageBox(NULL,e.ErrorMessage(),_T("数据库读取错误"),MB_ICONEXCLAMATION);
	}
}


double Data::Quality(wchar_t view[], int nIntercept) {
	int i, j;
	_variant_t var, ra;
	CString sql;
	double weightVector1[2000];
	double weightVector2[2000];
	double weightVectorMo1=0.0;
	double weightVectorMo2=0.0;
	double vectorProduct=0.0;
	try {
		//黄金向量
		for (i=0; i<nIntercept; i++)
			weightVector1[i] = 1.0/(double)(i+6);
		//抽取向量初始化为-1.0
		for (i=0; i<nIntercept; i++)
			weightVector2[i] = -1.0;
		
		for ( i = 0; i < nIntercept; i++ ) {
			wchar_t term[50] = {0};
			for ( j = 0; j < buffer[i]->wordCount; j++ ) wcscat(term, buffer[i]->pTerm[j]);
			sql.Format(_T("select * from %s, term where %s.词语=term.词组 and %s.词语='%s'"), view, view, view, term);
			m_pRecordset = m_pConnection->Execute((_bstr_t)sql,&ra,adCmdText);
			if (!m_pRecordset->adoEOF) {
				//若buffer中第i(从0开始)个词语是黄金向量表中权值排第r(从1开始)的关键词,则设置weightVector2[r-1]=1/(i+5)
				var = m_pRecordset->GetCollect("排名");
				int rank = var.intVal; 
				weightVector2[rank-1] = 1.0/(double)(i+5);
			}
		}
		
		//抽取向量表中权值还为-1.0的是没有被抽取到的关键词,将其权重设置为0.0
		for (i=0; i<nIntercept; i++) {
			if (weightVector2[i] < 0.0) 
				weightVector2[i] = 0.0;
		}

		
		//计算黄金向量的范数
		double value = 0.0;
		for (i=0; i<nIntercept; i++)
			value += weightVector1[i]*weightVector1[i];
		weightVectorMo1 = sqrt(value);
		
		//计算抽取向量的范数
		value = 0.0;
		for (i=0; i<nIntercept; i++)
			value += weightVector2[i]*weightVector2[i];
		weightVectorMo2 = sqrt(value);
		if (weightVectorMo2 == 0.0) { //直接返回0.0，因为抽取的一个都没中
			return 0.0;
		}
		
		//计算两个向量的内积
		for (i=0; i<nIntercept; i++)
			vectorProduct += weightVector1[i]*weightVector2[i];
		
		//计算余弦相似度
		return vectorProduct / (weightVectorMo1*weightVectorMo2);
	}
	catch (_com_error e){  
		::MessageBox(NULL,e.ErrorMessage(),_T("数据库读取错误"),MB_ICONEXCLAMATION);
	}
}


void Data::Analysis(wchar_t view[]) {
	int i;
	int nIntercept, keywordFound, keywordTotal;
	float precision, recall, quality;
	float statistics[6][3] = {0.0}; //6个截取数，3个参数
	wchar_t sortColumnName[]=_T("Termhood");
	FILE *fp = fopen("Statistics.txt","a");
//	fprintf(fp, "     10              30              50              100             200            400 (精/召/质 百分比%)\n");
	
	GetStatistics(view, 10, &statistics[0][0], &statistics[0][1], &statistics[0][2]);
	
	GetStatistics(view, 30, &statistics[1][0], &statistics[1][1], &statistics[1][2]);
	
	GetStatistics(view, 50, &statistics[2][0], &statistics[2][1], &statistics[2][2]);

	GetStatistics(view, 100, &statistics[3][0], &statistics[3][1], &statistics[3][2]);

	GetStatistics(view, 200, &statistics[4][0], &statistics[4][1], &statistics[4][2]);

	GetStatistics(view, 400, &statistics[5][0], &statistics[5][1], &statistics[5][2]);

	fprintf(fp, "—————————————————————————————+———————————————————————+———————————————————————+———————————————————————+———————————————————————+———————————————————————+\n");

	char table[20] = {0};
	WideCharToMultiByte(CP_ACP,   0,   view,   wcslen(view),   table,   20,   NULL,   NULL);   
	int len = strlen(table);
	table[len] = ':';
	table[len+1] = '\0';
	fprintf(fp, "%-8s", table);
	for (i=0; i<6; i++) {
		fprintf(fp, "%5.1f  %5.1f  %5.1f  |  ", statistics[i][0]*100, statistics[i][1]*100, statistics[i][2]*100);
	}
	fprintf(fp, "\n");

	fclose(fp);
}
