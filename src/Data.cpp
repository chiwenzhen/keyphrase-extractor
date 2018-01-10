#include "stdafx.h"
#include "Data.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

/************************************************************************/
/* ���캯��                                                             */
/************************************************************************/
Data::Data() {
	int i;
	_variant_t var;
	CString strValue;
	try {
		// ��ʼ��
		AfxOleInit();
		m_pConnection.CreateInstance("ADODB.Connection");
		m_pConnection-> CursorLocation=adUseClient; //�α�����,���ÿ��Զ�ȡ��¼������������Ļ���¼������-1
		m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=TermExtraction.mdb","","",adModeUnknown);
		m_pRecordset.CreateInstance("ADODB.Recordset");

		//��ȡStopList
		i= 0;
		m_pRecordset->Open("select * from StopList", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		iStopWordCount = m_pRecordset->RecordCount;
		StopList = new wchar_t[iStopWordCount][5];
		if(!m_pRecordset->BOF) m_pRecordset->MoveFirst();
		while(!m_pRecordset->adoEOF) {
			var = m_pRecordset->GetCollect("����");
			if(var.vt != VT_NULL) strValue = (LPCWSTR)_bstr_t(var);
			wcscpy(StopList[i++], strValue.GetBuffer(0));
			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
		QuickSort( StopList, 0, iStopWordCount - 1 );

		//��ȡMeasureWordList
		i = 0;
		m_pRecordset->Open("select * from MeasureWordList", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		iMeasureWordCount = m_pRecordset->RecordCount;
		MeasureWordList = new wchar_t[iMeasureWordCount][5];
		if(!m_pRecordset->BOF) m_pRecordset->MoveFirst();
		while(!m_pRecordset->adoEOF) {
			var = m_pRecordset->GetCollect("����");
			if(var.vt != VT_NULL) strValue = (LPCWSTR)_bstr_t(var);
			wcscpy(MeasureWordList[i++], strValue.GetBuffer(0));
			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
		QuickSort( MeasureWordList, 0, iMeasureWordCount - 1 );

		//��ȡͨ�ôʻ���Լ�Ƶ��
		m_pRecordset->Open("select * from �ʻ��", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		if(!m_pRecordset->BOF) m_pRecordset->MoveFirst();
		while(!m_pRecordset->adoEOF) {
			var = m_pRecordset->GetCollect("����");
			strValue = (LPCWSTR)_bstr_t(var);
			var = m_pRecordset->GetCollect("���ִ���");
			CommWordTable[strValue.GetBuffer(0)] = var.intVal; //�����ϣ��
			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
	}
	catch (_com_error e){  
		::MessageBox(NULL,e.ErrorMessage(),_T("���ݿ��ȡ����"),MB_ICONEXCLAMATION);
	}
}

/************************************************************************/
/* ������ͨ�����Ͽ��е�Ƶ��                                             */
/************************************************************************/
double Data::GetFrequency(wchar_t *str) {
	hash_map<wstring, int>::iterator it, iend = CommWordTable.end();
	if ((it=CommWordTable.find(str)) != iend) {
		return (double)(it->second);
	}
	else return 0.1;
}

/************************************************************************/
/* ��������                                                             */
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
/* �Ƚ�wStr1��wStr2�Ĵ�С,��ͬ����0��wStr1>wStr2����1�����򷵻�-1       */     
/************************************************************************/
int Data::WideStrCmpn(wchar_t * wStr1,int uStrLen1,wchar_t * wStr2, int uStrLen2) { //�ַ����Ƚ�
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
/* ����str�Ƿ�Ϊ������                                                  */
/************************************************************************/
bool Data::IsQuantifier( wchar_t **str, int len ) { //�Ƿ���������
	int i, strLen;
	wchar_t phrase[30] = {0};
	for ( i = 0; i < len; i++) {
		wcscat(phrase, str[i]);
	}
	strLen = wcslen(phrase);
	if ( strLen == 1 ) {
		if (IsDigitChar(phrase[0]) || IsInBadList(MEASURE_LIST, &phrase[strLen -1], 1)) return true; //���ֻ��һ�����֣�û�����壬����
		else return false;
	}
	else {
		//��������ڶ����������֣����ҵ�����һ�������ʣ���ô������ﱻ��Ϊ��һ�������ʶ���
//		if ( IsDigitChar(phrase[strLen -2]) && IsInBadList(MEASURE_LIST, &phrase[strLen -1], 1) ) return true;
		if ( IsDigitChar(phrase[0]) && IsInBadList(MEASURE_LIST, &phrase[1], 1) ) return true;
		else return false;
	}
}

/************************************************************************/
/* ����str�Ƿ�Ϊͣ�ô�                                                  */
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
/* ����str�Ƿ���ͣ�ôʱ�������ʱ���                                    */
/************************************************************************/
bool Data::IsInBadList(int whichList, wchar_t *wStr, int strLen) {//�ö��ַ���StopList/MearsureList�в��Ҹ����Ĵ����Ƿ���ͣ�ô�
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

int Data::Partition( wchar_t (*StopList)[5], int low, int high) { //���������Partition
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

void Data::QuickSort( wchar_t (*StopList)[5], int iBegin, int iEnd) { //��������
	int pivotLoc;
	if ( iBegin < iEnd) {
		pivotLoc = Partition(StopList, iBegin, iEnd);
		QuickSort(StopList, iBegin, pivotLoc - 1);
		QuickSort(StopList, pivotLoc + 1, iEnd);
	}
	return;
}

/************************************************************************/
/* �����ݴ������ݿ���                                                   */
/************************************************************************/
void Data::StoreIntoDatabase(Unit *buffer[], int len, wchar_t view[]) {
	int i, j;
	CString sql;
	wchar_t deleteCmd[100];
	this->buffer = buffer;
	this->len = len;
	try {
		m_pCommand.CreateInstance("ADODB.Command");
		m_pCommand->ActiveConnection = m_pConnection;  // �������Ӹ�����
		wsprintf(deleteCmd, _T("delete * from %s"), view);
		m_pCommand->CommandText = deleteCmd;  // SQL���
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
		::MessageBox(NULL,e.ErrorMessage(),_T("���ݿ�д�����"),MB_ICONEXCLAMATION);
	}

}


//nIntercept��ȡ�ļ�¼����keywordNum��ȡ���Ĺؼ�������stKeywordNum��׼�ؼ���������precision��ȷ�ʣ�recall�ٻ��ʣ�sortColumnName�������������
void Data::GetStatistics(wchar_t view[], int nIntercept, float *precision, float *recall, float *quality) {
	_variant_t var, ra;
	CString sql;
	int keywordTotal, keywordFound;
	try {
		//termרҵ�ؼ��ʱ��еĹؼ����ܸ���
		sql.Format(_T("select count(*) as ��׼�������� from term"));
		m_pRecordset = m_pConnection->Execute((_bstr_t)sql,&ra,adCmdText);
		var = m_pRecordset->GetCollect("��׼��������");
		keywordTotal = var.intVal;

		//�ж��ٺ�ѡ�ؼ���
		sql.Format(_T("select count(*) as ��ѡ�������� from %s"), view);
		m_pRecordset = m_pConnection->Execute((_bstr_t)sql,&ra,adCmdText);
		var = m_pRecordset->GetCollect("��ѡ��������");
		
		//ǰnIntercept��ѡ�ؼ������ж���ʵ�ʹؼ���
		sql.Format(_T("select count(����) as ���ҵ��ؼ����� from (select top %d ���� from %s order by Termhood desc) as ncwz, term where ����=term.����"), nIntercept, view);
		m_pRecordset = m_pConnection->Execute((_bstr_t)sql,&ra,adCmdText); 
		var = m_pRecordset->GetCollect("���ҵ��ؼ�����");
		keywordFound = var.intVal;

		//��ȷ��
		*precision = 1.0 * keywordFound / nIntercept;

		//�ٻ���
		*recall = 1.0 * keywordFound / keywordTotal;

		//��ȡ����
		*quality = Quality(view, nIntercept);
	}
	catch (_com_error e){  
		::MessageBox(NULL,e.ErrorMessage(),_T("���ݿ��ȡ����"),MB_ICONEXCLAMATION);
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
		//�ƽ�����
		for (i=0; i<nIntercept; i++)
			weightVector1[i] = 1.0/(double)(i+6);
		//��ȡ������ʼ��Ϊ-1.0
		for (i=0; i<nIntercept; i++)
			weightVector2[i] = -1.0;
		
		for ( i = 0; i < nIntercept; i++ ) {
			wchar_t term[50] = {0};
			for ( j = 0; j < buffer[i]->wordCount; j++ ) wcscat(term, buffer[i]->pTerm[j]);
			sql.Format(_T("select * from %s, term where %s.����=term.���� and %s.����='%s'"), view, view, view, term);
			m_pRecordset = m_pConnection->Execute((_bstr_t)sql,&ra,adCmdText);
			if (!m_pRecordset->adoEOF) {
				//��buffer�е�i(��0��ʼ)�������ǻƽ���������Ȩֵ�ŵ�r(��1��ʼ)�Ĺؼ���,������weightVector2[r-1]=1/(i+5)
				var = m_pRecordset->GetCollect("����");
				int rank = var.intVal; 
				weightVector2[rank-1] = 1.0/(double)(i+5);
			}
		}
		
		//��ȡ��������Ȩֵ��Ϊ-1.0����û�б���ȡ���Ĺؼ���,����Ȩ������Ϊ0.0
		for (i=0; i<nIntercept; i++) {
			if (weightVector2[i] < 0.0) 
				weightVector2[i] = 0.0;
		}

		
		//����ƽ������ķ���
		double value = 0.0;
		for (i=0; i<nIntercept; i++)
			value += weightVector1[i]*weightVector1[i];
		weightVectorMo1 = sqrt(value);
		
		//�����ȡ�����ķ���
		value = 0.0;
		for (i=0; i<nIntercept; i++)
			value += weightVector2[i]*weightVector2[i];
		weightVectorMo2 = sqrt(value);
		if (weightVectorMo2 == 0.0) { //ֱ�ӷ���0.0����Ϊ��ȡ��һ����û��
			return 0.0;
		}
		
		//���������������ڻ�
		for (i=0; i<nIntercept; i++)
			vectorProduct += weightVector1[i]*weightVector2[i];
		
		//�����������ƶ�
		return vectorProduct / (weightVectorMo1*weightVectorMo2);
	}
	catch (_com_error e){  
		::MessageBox(NULL,e.ErrorMessage(),_T("���ݿ��ȡ����"),MB_ICONEXCLAMATION);
	}
}


void Data::Analysis(wchar_t view[]) {
	int i;
	int nIntercept, keywordFound, keywordTotal;
	float precision, recall, quality;
	float statistics[6][3] = {0.0}; //6����ȡ����3������
	wchar_t sortColumnName[]=_T("Termhood");
	FILE *fp = fopen("Statistics.txt","a");
//	fprintf(fp, "     10              30              50              100             200            400 (��/��/�� �ٷֱ�%)\n");
	
	GetStatistics(view, 10, &statistics[0][0], &statistics[0][1], &statistics[0][2]);
	
	GetStatistics(view, 30, &statistics[1][0], &statistics[1][1], &statistics[1][2]);
	
	GetStatistics(view, 50, &statistics[2][0], &statistics[2][1], &statistics[2][2]);

	GetStatistics(view, 100, &statistics[3][0], &statistics[3][1], &statistics[3][2]);

	GetStatistics(view, 200, &statistics[4][0], &statistics[4][1], &statistics[4][2]);

	GetStatistics(view, 400, &statistics[5][0], &statistics[5][1], &statistics[5][2]);

	fprintf(fp, "����������������������������������������������������������+����������������������������������������������+����������������������������������������������+����������������������������������������������+����������������������������������������������+����������������������������������������������+\n");

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
