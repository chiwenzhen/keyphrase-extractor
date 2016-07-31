/*��Ĺ��ܣ�ͨ�������ݿ����ӣ�����������ʣ�ͣ�ô��Լ�������������ݿ�*/
#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>
#include <afxdb.h>
#include <hash_map>
#include "HashTable.h"
using namespace std;

#define CJK_BEGIN ( (unsigned short)0x4e00 ) //���ֵ�UNICODE�������ʼ�߽�
#define CJK_END   ( (unsigned short)0x9fbf ) //���ֵ�UNICODE����Ľ����߽�
#define IsChineseCharacter(x) ( (CJK_BEGIN<=x) && (x<=CJK_END) )
//�����Ű���                   ��               ��               ��               !                ?                ��               ��   
#define IsPunctuation(x) ( ((x)==0x3002) || ((x)==0xff1f) || ((x)==0xff01) || ((x)==0x0021) || ((x)==0x003f) || ((x)==0xff0c) || ((x)==0xff1a)) 
#define STOP_LIST 0
#define MEASURE_LIST 1
//��Ӧ��������:��,һ,��,��,��,��,��,��,
//             ��,��,ʮ,��,ǧ,��,��,��,��
//             ��,��,ĳ,ÿ,��,��,��,��,��,��,��
#define IsDigitChar(x) (((x)==0x96f6) || ((x)==0x4e00) || ((x)==0x4e8c) || ((x)==0x4e09) || ((x)==0x56db) || ((x)==0x4e94) || ((x)==0x516d) || ((x)==0x4e03)\
	|| ((x)==0x516b) || ((x)==0x4e5d) || ((x)==0x5341) || ((x)==0x767e) || ((x)==0x5343) || ((x)==0x4e07) || ((x)==0x4ebf) || ((x)==0x4e24) || ((x)==0x54ea)\
	|| ((x)==0x591a) || ((x)==0x5c11) || ((x)==0x67d0) || ((x)==0x6bcf) || ((x)==0x8fd9) || ((x)==0x90a3) || ((x)==0x6b64) || ((x)==0x8be5) || ((x)==0x6574)\
	|| ((x)==0x51e0))

//Сд��ĸ
#define IsLowerCaseOrSpace(x) ( ((x)==0x0020) || ((x)>=0x0061 && (x)<=0x007a) ) 

//����ǰ׺
#define Prefix(x) ( (x)==0x5982 || (x)==0x6765 || (x)==0x8981 || (x)==0x5230 || (x)==0x5c06 || (x)==0x9700 || (x)==0x53ef\
	|| (x)==0x8ba9 )

//���˺�׺
#define Postfix(x) ( (x)==0x65f6 || (x)==0x6765 || (x)==0x53bb || (x)==0x5230 || (x)==0x4e0a || (x)==0x4e0b || (x)==0x6709\
	|| (x)==0x662f || (x)==0x5728 || (x)==0x4e8e || (x)==0x6210 || (x)==0x4e2d || (x)==0x5982 || (x)==0x9700 || (x)==0x975e )

#define ADJ		0x0061 //���ݴ�
#define DIS		0x0062 //�����
#define CONJ	0x0063 //���Ӵ�
#define ADV		0x0064 //����
#define INT		0x0065 //��̾��
#define ORNT	0x0066 //��λ��
#define PREFIX	0x0068 //ǰ׺
#define SUFFIX	0x006b //��׺
#define NUM		0x006d //����
#define NOUN	0x006e //����
#define ECHO	0x006f //������
#define PREP 	0x0070 //���
#define CLAS	0x0071 //����
#define PRON	0x0072 //����
#define LOC		0x0073 //������
#define TIME	0x0074 //ʱ���
#define AUX		0x0075 //����
#define VERB	0x0076 //����
#define PUNC	0x0077 //������
#define STR		0x0078 //�ַ���
#define MODAL	0x0079 //������
#define STAT	0x007a //״̬��

#define IsBadPOS(x) ((x)==CONJ || (x)==AUX || (x)==PREP || (x)==PRON || (x)==TIME || (x)==INT || (x)==MODAL )

class Data {
private:
	_ConnectionPtr	m_pConnection;
	_RecordsetPtr	m_pRecordset;
	_CommandPtr		m_pCommand;
	
	hash_map<wstring, int> CommWordTable;
	wchar_t (*StopList)[5]; //�ϸ��ͣ�ôʱ������ݿ⵼��
	wchar_t (*MeasureWordList)[5];
	int iStopWordCount ;
	int iMeasureWordCount ;
	int WideStrCmpn(wchar_t * wStr1,int uStrLen1,wchar_t * wStr2, int uStrLen2);
	int Partition( wchar_t (*StopList)[5], int low, int high);
	bool IsInBadList(int whichList, wchar_t *wStr, int strLen); //�鿴str�Ƿ���ͣ�ôʱ�/���ʱ���
	void QuickSort( wchar_t (*StopList)[5], int iBegin, int iEnd);
	void SortDesc(wchar_t columnName[]); //����columnName����У���������
	Unit **buffer;
	int len;
public:
	Data();
	~Data();
	double GetFrequency(wchar_t *str);
	bool IsQuantifier( wchar_t **str, int len );
	bool IsInStopList(wchar_t **str, int len);
	void StoreIntoDatabase(Unit *buffer[], int len, wchar_t view[]);
	double Quality(wchar_t view[], int len);
	void Analysis(wchar_t view[]);
	void GetStatistics(wchar_t view[], int nIntercept, float *precision, float *recall, float *quality);
};

#endif