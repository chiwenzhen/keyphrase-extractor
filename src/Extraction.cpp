#include "stdafx.h"
#include <iostream>
#include <io.h>
#include <math.h>
#include "Extraction.h"
#include "ICTCLAS2011.h"
#include <unicode\ucnv.h>
using namespace std;
#pragma comment(lib, "icuuc.lib")  
#pragma comment(lib, "ICTCLAS2011.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//FREQUENCY_THRESHOLD��������Ϊ��Ҫ��DataStruct�ı��ܹ�39000�����<=3����35000,���Կ�ȡ���ǽ�FREQUENCY_THRESHOLD����Ϊ3
//������Ϊ�ܶ���ν�Ĺؼ�����ʵƵ�ʷǳ���С��6����70������Щ����Ӧ�����ǹؼ��ʵİɣ�����
int FREQUENCY_THRESHOLD = 0;
int INDEPENDENT_THRESHOLD = 0;


//��ӡ��ǰʱ��
void showTime(FILE *fp, char *str) {
	SYSTEMTIME sys;
	GetLocalTime( &sys ); 
	fprintf(fp, "%s\t%02d:%02d:%02d.%03d\n", str, sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
}

//fileNameҪ���б���ת����GB*���ı���uCodeTextLenת����ĳ��ȣ�����ת������ַ������׵�ַ
//ʹ�������Ҫ�ֶ��ͷŸû�������
//ת�����Unicode���黺�������Ȱ�����ͷ��0xfeff����βû��\0��
wchar_t* g2u(int *uCodeTextLen, char *fileName) {
	char *gbCodeText;
	int gbCodeTextLen;
	const char *toCodeName= "utf16";
	const char *fromCodeName = "gb2312";	
	char *uCodeText;
	int ret;
	FILE *fp;
	
	//����gb�����ļ����ڴ���
	fp = fopen(fileName, "rb");
	fseek(fp, 0, SEEK_END);
	gbCodeTextLen = ftell(fp);
	rewind(fp);
	gbCodeText = new char[gbCodeTextLen];
	fread(gbCodeText, gbCodeTextLen, 1, fp);
	fclose(fp);
	//ת���ַ����뷽ʽ
	UErrorCode ErrorCode = U_ZERO_ERROR; 
	ret = ucnv_convert(toCodeName, fromCodeName, NULL, 0, gbCodeText, gbCodeTextLen, &ErrorCode);
	uCodeText = new char[ret];
	*uCodeTextLen = ret >> 1;
	ErrorCode = U_ZERO_ERROR; //ÿ��ʹ��ǰҪ��ΪU_ZERO_ERROR
	ucnv_convert(toCodeName, fromCodeName, uCodeText, ret, gbCodeText, gbCodeTextLen, &ErrorCode);
	delete[] gbCodeText;
	return (wchar_t *)uCodeText;
}

Extraction::Extraction() {
	uSentenceNo = 1; 
	uParagraphNo = 1; 
	wordNo = 0; 
	SortBuffer = NULL;
	SortBufferSize = 0;
	pView = NULL;
	Text.reserve(250000); //��ʼ25���ָ�룬��1M��С��
}


//��������
Extraction::~Extraction() {
	if (SortBuffer != NULL) delete[] SortBuffer;
	SortBuffer = NULL;
	if (pView != NULL) delete[] pView;
	pView = NULL;
}

//����ļ��Ƿ���ANSI��ʽ������������Ӧ����ֵ
int Extraction::PreProcess(char *fileName) {
	long fileSize;
	unsigned char ch[3];
	FILE *fp = fopen(fileName, "rb");
	fread(ch, sizeof(ch), 1, fp);
	
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fclose(fp);
	//����Ƶ�ʵ����Ʒǳ��б�Ҫ����ΪС���ʵĴ���ǳ��࣬һЩ�ܳ��Ķ���Ƶ��ֻ��1
	if (fileSize < 5*1024) {
		FREQUENCY_THRESHOLD = 1;
		INDEPENDENT_THRESHOLD = 0;
	}
	else {
		FREQUENCY_THRESHOLD = 3;
		INDEPENDENT_THRESHOLD = 2;
	}
	if ( (ch[0]==0xfe&&ch[1]==0xff) || (ch[0]==0xff&&ch[1]==0xfe) || (ch[0]==0xef&&ch[1]==0xbb&&ch[2]==0xbf) ) return -1;
	else {
		delete[] SortBuffer;
		SortBuffer = NULL;
		SortBufferSize = 0;
		delete[] pView;
		pView = NULL;
		uSentenceNo = 1; 
		uParagraphNo = 1; 
		wordNo = 0; 
		Text.clear();
		hashTable.Clear();
		return 0;
	}
}

//C
int Extraction::ProcessC(char *fileName) {
	remove("������.txt");
	FILE *fp = fopen("������.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //��fileName����ANSI�����ʽ���ĵ�������-1����
	showTime(fp, fileName);
	showTime(fp, "1 ��ʼ�ִ�         ");
	if (WordSegmentation(fileName) == -1) return -2; //�ִʳ���
	showTime(fp, "2 ��ʼͳ��Ƶ��     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {

		showTime(fp,"3 ��ʼ����C-value   ");
		CalcC_value();

		showTime(fp,"4 ���ڴ���          ");
		PostProcess();
		QuickSortByTermhood( 0, SortBufferSize-1); //����termhood�Ӵ�С����
	}
	showTime(fp,"5 ���н���          ");
	fclose(fp);
	return 0;
}

//C+L
int Extraction::ProcessCL(char *fileName) {
	remove("������.txt");
	FILE *fp = fopen("������.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //��fileName����ANSI�����ʽ���ĵ�������-1����
	showTime(fp, fileName);
	showTime(fp, "1 ��ʼ�ִ�         ");
	if (WordSegmentation(fileName) == -1) return -2; //�ִʳ���
	showTime(fp, "2 ��ʼͳ��Ƶ��     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {
		
		showTime(fp,"3 ��ʼ����C-value   ");
		CalcC_value();

		showTime(fp,"4 ��ʼ����ֲ����ֶ�");
		CalcLocalRe_occurrence();
		
		showTime(fp,"5 ���ڴ���          ");
		PostProcess();
		QuickSortByTermhood( 0, SortBufferSize-1); //����termhood�Ӵ�С����
	}
	showTime(fp,"6 ���н���          ");
	fclose(fp);
	return 0;
}


//C+F
int Extraction::ProcessCF(char *fileName) {
	remove("������.txt");
	FILE *fp = fopen("������.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //��fileName����ANSI�����ʽ���ĵ�������-1����
	showTime(fp, fileName);
	showTime(fp, "1 ��ʼ�ִ�         ");
	if (WordSegmentation(fileName) == -1) return -2; //�ִʳ���
	showTime(fp, "2 ��ʼͳ��Ƶ��     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {
		
		showTime(fp,"3 ��ʼ����C-value   ");
		CalcC_value();
		
		showTime(fp,"4 ��ʼ������ЧƵ��");
		CalcAggregateFrequency();
		
		showTime(fp,"5 ���ڴ���          ");
		PostProcess();
		QuickSortByTermhood( 0, SortBufferSize-1); //����termhood�Ӵ�С����
	}
	showTime(fp,"6 ���н���          ");
	fclose(fp);
	return 0;
}

//C+G
int Extraction::ProcessCG(char *fileName) {
	remove("������.txt");
	FILE *fp = fopen("������.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //��fileName����ANSI�����ʽ���ĵ�������-1����
	showTime(fp, fileName);
	showTime(fp, "1 ��ʼ�ִ�         ");
	if (WordSegmentation(fileName) == -1) return -2; //�ִʳ���
	showTime(fp, "2 ��ʼͳ��Ƶ��     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {
		
		
		showTime(fp,"3 ��ʼ����C-value   ");
		CalcC_value();

		showTime(fp,"4 ��ʼ����ȫ�����ֶ�");
		CalcGlobalRe_occurrence();
		
		showTime(fp,"5 ���ڴ���          ");
		PostProcess();
		QuickSortByTermhood( 0, SortBufferSize-1); //����termhood�Ӵ�С����
	}
	showTime(fp,"10 ���н���          ");
	fclose(fp);
	return 0;
}

//C+L+G
int Extraction::ProcessCLG(char *fileName) {
	remove("������.txt");
	FILE *fp = fopen("������.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //��fileName����ANSI�����ʽ���ĵ�������-1����
	showTime(fp, fileName);
	showTime(fp, "1 ��ʼ�ִ�         ");
	if (WordSegmentation(fileName) == -1) return -2; //�ִʳ���
	showTime(fp, "2 ��ʼͳ��Ƶ��     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {

		showTime(fp,"3 ��ʼ����C-value   ");
		CalcC_value();
		
		showTime(fp,"4 ��ʼ����ֲ����ֶ�");
		CalcLocalRe_occurrence();
		
		showTime(fp,"5 ��ʼ����ȫ�����ֶ�");
		CalcGlobalRe_occurrence();
		
		showTime(fp,"6 ���ڴ���          ");
		PostProcess();
 		QuickSortByTermhood( 0, SortBufferSize-1); //����termhood�Ӵ�С����



// 		int num = 400;
// 		for (int i=0; i<num; i++)
// 		{
// 			SortBuffer[i]->termhood = 0.0; //ѡ��ǰnum��termhood���¹��㣬������C+G���¸�ֵ����
// 		}
// 		SortBufferSize = num;
// 		showTime(fp,"���¼���C-value   ");
// 		CalcC_value();
// 		showTime(fp,"���¼���ȫ�����ֶ�");
// 		CalcGlobalRe_occurrence();
// 		QuickSortByTermhood( 0, num-1); //����termhood�Ӵ�С����ǰ400����
		

	}
	showTime(fp,"7 ���н���          ");
	fclose(fp);
	return 0;
}

//C+F+G
int Extraction::ProcessCFG(char *fileName) {
	remove("������.txt");
	FILE *fp = fopen("������.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //��fileName����ANSI�����ʽ���ĵ�������-1����
	showTime(fp, fileName);
	showTime(fp, "1 ��ʼ�ִ�         ");
	if (WordSegmentation(fileName) == -1) return -2; //�ִʳ���
	showTime(fp, "2 ��ʼͳ��Ƶ��     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {
		
		showTime(fp,"3 ��ʼ����C-value   ");
		CalcC_value();
		
		showTime(fp,"4 ��ʼ������ЧƵ��");
		CalcAggregateFrequency();
		
		showTime(fp,"5 ��ʼ����ȫ�����ֶ�");
		CalcGlobalRe_occurrence();
		
		showTime(fp,"6 ���ڴ���          ");
		PostProcess();
		QuickSortByTermhood( 0, SortBufferSize-1); //����termhood�Ӵ�С����
		
		
		
		// 		int num = 400;
		// 		for (int i=0; i<num; i++)
		// 		{
		// 			SortBuffer[i]->termhood = 0.0; //ѡ��ǰnum��termhood���¹��㣬������C+G���¸�ֵ����
		// 		}
		// 		SortBufferSize = num;
		// 		showTime(fp,"���¼���C-value   ");
		// 		CalcC_value();
		// 		showTime(fp,"���¼���ȫ�����ֶ�");
		// 		CalcGlobalRe_occurrence();
		// 		QuickSortByTermhood( 0, num-1); //����termhood�Ӵ�С����ǰ400����
		
		
	}
	showTime(fp,"7 ���н���          ");
	fclose(fp);
	return 0;
}

//C+L+G+S
int Extraction::ProcessCLGS(char *fileName) {
	remove("������.txt");
	FILE *fp = fopen("������.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //��fileName����ANSI�����ʽ���ĵ�������-1����
	showTime(fp, fileName);
	showTime(fp, "1 ��ʼ�ִ�         ");
	if (WordSegmentation(fileName) == -1) return -2; //�ִʳ���
	showTime(fp, "2 ��ʼͳ��Ƶ��     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {
		
		showTime(fp,"3 ��ʼ����C-value   ");
		CalcC_value();
		
		showTime(fp,"4 ��ʼ����ֲ����ֶ�");
		CalcLocalRe_occurrence();
		
		showTime(fp,"5 ��ʼ����ȫ�����ֶ�");
		CalcGlobalRe_occurrence();
		
		showTime(fp,"6 ��ʼ����ƽ����    ");
		CalcSingularity();
		
		showTime(fp,"7 ���ڴ���          ");
		PostProcess();
		QuickSortByTermhood( 0, SortBufferSize-1); //����termhood�Ӵ�С����


// 		int num = 400;
// 		for (int i=0; i<num; i++)
// 		{
// 			SortBuffer[i]->termhood = 0.0; //ѡ��ǰnum��termhood���¹��㣬������C+G���¸�ֵ����
// 		}
// 		SortBufferSize = num;
// 		showTime(fp,"���¼���C-value   ");
// 		CalcC_value();
// 		showTime(fp,"���¼���ȫ�����ֶ�");
// 		CalcGlobalRe_occurrence();
// 		QuickSortByTermhood( 0, num-1); //����termhood�Ӵ�С����ǰ400����


	}
	showTime(fp,"8 ���н���          ");
	fclose(fp);
	return 0;
}


//C+F+G+S
int Extraction::ProcessCFGS(char *fileName) {
	remove("������.txt");
	FILE *fp = fopen("������.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //��fileName����ANSI�����ʽ���ĵ�������-1����
	showTime(fp, fileName);
	showTime(fp, "1 ��ʼ�ִ�         ");
	if (WordSegmentation(fileName) == -1) return -2; //�ִʳ���
	showTime(fp, "2 ��ʼͳ��Ƶ��     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {
		
		showTime(fp,"3 ��ʼ����C-value   ");
		CalcC_value();
		
		showTime(fp,"4 ��ʼ������ЧƵ��");
		CalcAggregateFrequency();
		
		showTime(fp,"5 ��ʼ����ȫ�����ֶ�");
		CalcGlobalRe_occurrence();
		
		showTime(fp,"6 ��ʼ����ƽ����    ");
		CalcSingularity();
		
		showTime(fp,"7 ���ڴ���          ");
		PostProcess();
		QuickSortByTermhood( 0, SortBufferSize-1); //����termhood�Ӵ�С����
		
		
		// 		int num = 400;
		// 		for (int i=0; i<num; i++)
		// 		{
		// 			SortBuffer[i]->termhood = 0.0; //ѡ��ǰnum��termhood���¹��㣬������C+G���¸�ֵ����
		// 		}
		// 		SortBufferSize = num;
		// 		showTime(fp,"���¼���C-value   ");
		// 		CalcC_value();
		// 		showTime(fp,"���¼���ȫ�����ֶ�");
		// 		CalcGlobalRe_occurrence();
		// 		QuickSortByTermhood( 0, num-1); //����termhood�Ӵ�С����ǰ400����
		
		
	}
	showTime(fp,"8 ���н���          ");
	fclose(fp);
	return 0;
}

/***********************************���ı��ִ�************************************/
int Extraction::WordSegmentation(char *fileName) {
	if(!ICTCLAS_Init()) return -1;
	ICTCLAS_FileProcess(fileName, "temp.txt", 1);
	ICTCLAS_Exit();
	return 0;
}

/***********************************�����ı��дʴ���Ƶ��ֵ************************************/
void Extraction::CalcFrequency(void) {
	Unit *p;
	int r, s, indexWord, indexChar, iChatacterCount;
	uSentenceNo = 1; 
	uParagraphNo = 1;
	wordNo = 0; 
	indexWord = 0;
	indexChar = 0;
	
	//�ִʽ������temp.txt�����潫�䵼�뵽�ڴ��У���pViewָ����
	pView = g2u(&iChatacterCount, "temp.txt");
//	remove("temp.txt");
	int i = 0;
	while (i < iChatacterCount) {
		while (!IsChineseCharacter(pView[i]) && i<iChatacterCount) {
			if ( pView[i] == 0x000D ) uParagraphNo++;
			if ( IsPunctuation(pView[i])) { 
				uSentenceNo++;
				if (Text.back() != 0) 
					Text.push_back((Unit *)0);
			}
			i++;
		}
		if (i >= iChatacterCount) break;

		/*�����Կո�����Ĵ�������ηŵ�wordSequence����ַ���������*/
			
		wchar_t *wordSequence[50] = {0};
		wchar_t POS[50] = {0};
		for ( indexWord=0; indexWord<50; indexWord++ ) {
			wordSequence[indexWord] = &pView[i];
			while ( pView[i] != 0x002f ) i++; //ֻҪû�е�'/',�������ͻ�û�н���
			pView[i++] = 0x0000; //��'/'��Ϊ0����ô�������һ�����������������ֱ��ʹ�øô�
			if ( IsBadPOS(pView[i]) || data.IsInStopList(&wordSequence[indexWord], 1) ) { //�������������Ի�ͣ�ô�������
				wordSequence[indexWord] = 0;
				indexWord--;
				break;
			}
			POS[indexWord] = pView[i++];
			if (POS[indexWord]==CLAS && POS[indexWord-1]==NUM) {
				wordSequence[indexWord] = 0;
				wordSequence[indexWord-1] = 0;
				indexWord -= 2;
				break;
			}
			while ( IsLowerCaseOrSpace(pView[i])) i++; //�����һ���ֻ��ǿո��Ӣ��Сд��ĸ�Ļ����ͼ��������ң�ֱ����һ������Ŀ�ͷ
			if ( !IsChineseCharacter(pView[i]) ) break; //������Ǻ��֣�˵�����segment��ͷ�ˣ���ʼ��һ��segment��Ѱ��
		}
		//���wordSequence�������Ӵ�, �������ϣ��
		for ( r=0; r<=indexWord; r++ ) {
				for ( s=r; s<=indexWord; s++ ) {
					//�����д�����Ŀ����
					if ( s - r  > 4 ) break;
					
					//�����ʵĵ��֣���ֻ��һ�����ݴʣ�����
					if ( s - r == 0 
						&& ( (POS[r]==ADJ) || (::wcslen(wordSequence[r])==1 && POS[r] != NOUN) )) continue; 
					//����+���ݴ� ���������ȷʵ��ȥ���ô�
					//fv��da������fv������ʵ����ô�ã�����fv�Ĺؼ���ֻ���ٲ��֣����ţ��ڲ��鲢���ڲ�����
					if ( s - r == 1 
						&& ( (POS[r]==ORNT && POS[s]==VERB) || (POS[r]==ADV && POS[r+1]==ADJ)))  continue;
					//���ʿ�ͷ��λ�ʡ����ʡ�����ʽ�β����
					if ( POS[r]==CLAS 
						|| POS[s]==DIS 
						|| POS[s]==ORNT 
						|| POS[s]==ADV
						|| POS[s]==NUM) continue;

					//��׺��һЩ��Ҫ�޳�������������ȥ���ڡ������еȵ�
					if ( ::wcslen(wordSequence[s])==1 && Postfix(*(wordSequence[s])) ) continue;

					//��׺��һЩ��Ҫ�޳��������罫��Ҫ����
					if ( ::wcslen(wordSequence[r])==1 && Prefix(*(wordSequence[r])) ) continue;
					
					if ( data.IsQuantifier( &wordSequence[r], s - r + 1) ) continue; //�����ʣ�����
					if ( data.IsInStopList(&wordSequence[r], s - r + 1) ) continue; //ͣ�ôʣ�����

					wordNo++; //�����������

					if ( hashTable.Find(&wordSequence[r], s - r + 1, &p) ) {//�ö����Ѿ����ֹ�
						p->frequency++;
						//�����Զ������Ϊ���ֱ�׼�Ĵ�
						if( (uSentenceNo - p->uLastSentenceNo) > MULTI_SENTENCE_THRESHOLD ) {
							p->uMultiSentenceLoneM++;
							p->bMultiSentenceNowNotLone = false;
						}
						else {
							if(p->bMultiSentenceNowNotLone == false ) {
								p->uMultiStClusterStM++;///ȫ��
								p->uMultiSentenceLoneM--;
								p->bMultiSentenceNowNotLone = true;
							}
							p->uMultiStClusterStM += uSentenceNo - p->uLastSentenceNo;///ȫ��
						}
						
						//�����Ե�������Ϊ���ֱ�׼�Ĵ�
						if( p->uLastParagraphNo != uParagraphNo ) {
							//���ִ��ϴγ��ֵĶκź���β�ͬ������ͬ��
							p->uParagraphLoneM++;
							p->bParagraphNowNotLone = false;
						}
						else { //���ִ��ϴγ��ֵĶκź������ͬ����ͬ��
							if( p->bParagraphNowNotLone == false ) {
								//��ǰ���Ǹôصڶ��γ���
								p->uParaClusterParaM++;///ȫ��
								p->uParagraphLoneM--;
								p->bParagraphNowNotLone = true;
							}
								p->uParaClusterParaM += uParagraphNo - p->uLastParagraphNo;
						}

						//�����Զ������Ϊ���ֱ�׼�Ĵ�
						if( (uParagraphNo - p->uLastParagraphNo) > MULTI_PARAGRAPH_THRESHOLD ) {
							p->uMultiParagraphLoneM++;
							p->bMultiParagraphNowNotLone = false;
						}
						else {
							if( p->bMultiParagraphNowNotLone == false ) {
								p->uMultiParaClusterParaM++;///ȫ��
								p->uMultiParagraphLoneM--;
								p->bMultiParagraphNowNotLone = true;
							}
							p->uMultiParaClusterParaM += uParagraphNo - p->uLastParagraphNo;///ȫ��
						}
					}
					else { //�ö����״γ���,pΪ���Բ����ϣ���λ��
						p->uMultiSentenceLoneM	= 1;
						p->uParagraphLoneM		= 1;
						p->uMultiParagraphLoneM	= 1;
						p->uMultiStClusterStM	= 0;	
						p->uMultiParaClusterParaM = 0;	
						p->uMultiParaClusterParaM = 0;
						p->bMultiSentenceNowNotLone = false;
						p->bParagraphNowNotLone		= false;
						p->bMultiParagraphNowNotLone= false;
					}
					Text.push_back(p);
					p->uLastSentenceNo = uSentenceNo;
					p->uLastParagraphNo = uParagraphNo;
			}
		}	
	}
}



/***********************************����ϣ����ÿ����Ч���ݵ�unitָ�����SortBuffer************************************/
int Extraction::LoadToBuffer(void) {
	Unit *p;
	int indexSize = 0;
	SortBufferSize = hashTable.Size();
	SortBuffer = new Unit*[SortBufferSize];

	while ( ( p = hashTable.GetNext() ) != NULL ) { 
		if ( p->frequency > FREQUENCY_THRESHOLD ) SortBuffer[indexSize++] = p; //ֻ����Ƶ��������ֵ�Ĵʴ�
	}
	SortBufferSize = indexSize;
	if (SortBufferSize > 0) return 0;
	else return 1; //û�����ݱ�����SortBuffer������1
}


/***********************************����C-valueֵ************************************/
void Extraction::CalcC_value(void) {
	int i, j, r, s, uOrder;
	Unit *pUnit;
	wchar_t **pTerm;
	double LOG2 = log(2.0);
	QuickSortByLength(0, SortBufferSize - 1); //���ճ��ȴӴ�С���򣨳���ָ�������ʸ�����
	
	//�ҳ�ÿ���ʴ����Ӵ����������Ӵ��ı�Ƕ�׸���(nestedFreq)
	for ( i = 0; i < SortBufferSize; i++ ) {
		pTerm = SortBuffer[i]->pTerm;
		for ( r=0; r<SortBuffer[i]->wordCount; r++ ) {
			for ( s=r; s<SortBuffer[i]->wordCount; s++ ) {
				if (s - r + 1 < SortBuffer[i]->wordCount) { //������
					if ( hashTable.Find(&pTerm[r], s - r + 1, &pUnit) ) {
						pUnit->nesterCount++;
						pUnit->nestedFreq += SortBuffer[i]->frequency - SortBuffer[i]->nestedFreq;
					}
				}
			}
		}
	}
	
	for ( i = 0; i < SortBufferSize; i++ ) {

		//����ʴ�����������
		int charCount = 0;
		for (j=0; j<SortBuffer[i]->wordCount; j++) {
			charCount = charCount + wcslen(SortBuffer[i]->pTerm[j]);
		}
		SortBuffer[i]->charCount = charCount;

		//����C-value
		if (SortBuffer[i]->nesterCount == 0) {
			SortBuffer[i]->value = log((double)(SortBuffer[i]->wordCount+1)) / LOG2 * SortBuffer[i]->frequency;
//			SortBuffer[i]->value = log((double)(SortBuffer[i]->charCount+5)) / LOG2 * SortBuffer[i]->frequency;
		} else {
			SortBuffer[i]->value = log((double)(SortBuffer[i]->wordCount+1)) / LOG2 * ((double)SortBuffer[i]->frequency - (double)SortBuffer[i]->nestedFreq / (double)SortBuffer[i]->nesterCount); 
//			SortBuffer[i]->value = log((double)(SortBuffer[i]->charCount)) / LOG2 * ((double)SortBuffer[i]->frequency - (double)SortBuffer[i]->nestedFreq / (double)SortBuffer[i]->nesterCount); 
//			SortBuffer[i]->value = log((double)(SortBuffer[i]->wordCount+1)) / LOG2 * (SortBuffer[i]->frequency - SortBuffer[i]->nestedFreq) * (SortBuffer[i]->frequency - SortBuffer[i]->nestedFreq) / SortBuffer[i]->frequency; 
//			SortBuffer[i]->value = log((double)(SortBuffer[i]->charCount+5)) / LOG2 * (SortBuffer[i]->frequency - SortBuffer[i]->nestedFreq) * (SortBuffer[i]->frequency - SortBuffer[i]->nestedFreq) / SortBuffer[i]->frequency; 
		}

	}
	QuickSortByValue( 0, SortBufferSize - 1);
	
	uOrder = 1;
	SortBuffer[0]->termhood += C_VALUE_WEIGHT * uOrder;
	for( i = 1; i < SortBufferSize; i++ ) {
		if( SortBuffer[i]->value != 0 && SortBuffer[i-1]->value / SortBuffer[i]->value  < 0.99 ) uOrder = i;
		SortBuffer[i]->termhood += C_VALUE_WEIGHT * uOrder;
	}
	for (i=0; i<SortBufferSize; i++) {//��¼C-value����
		SortBuffer[i]->cvalueRank = i;
	}
}

/***********************************����ۺ�Ƶ��F************************************/
void Extraction::CalcAggregateFrequency(void) {
	int i, uOrder;
	double LOG2 = log(2.0);

	//������ӵ�F
	for ( i=0; i<SortBufferSize; i++) {
		SortBuffer[i]->value = log((double)(SortBuffer[i]->wordCount+1)) / LOG2 * (SortBuffer[i]->frequency - SortBuffer[i]->uMultiSentenceLoneM);
	}
	QuickSortByValue( 0, SortBufferSize - 1);
	uOrder = 1;
	SortBuffer[0]->termhood += LOCAL_WEIGHT_1 * uOrder;
	for( i = 1; i < SortBufferSize; i++ ) {
      if( SortBuffer[i]->value != 0 && SortBuffer[i-1]->value / SortBuffer[i]->value  < 0.99 ) uOrder = i;
	  SortBuffer[i]->termhood += LOCAL_WEIGHT_1 * uOrder;
	}
	
	for (i=0; i<SortBufferSize; i++) {//��¼F1����
		SortBuffer[i]->F1Rank = i;
	}


	//���������F
	for ( i=0; i<SortBufferSize; i++) {
		SortBuffer[i]->value = log((double)(SortBuffer[i]->wordCount+1)) / LOG2 * (SortBuffer[i]->frequency - SortBuffer[i]->uParagraphLoneM);
	}
	QuickSortByValue( 0, SortBufferSize - 1);
	uOrder = 1;
	SortBuffer[0]->termhood += LOCAL_WEIGHT_2 * uOrder;
	for( i = 1; i < SortBufferSize; i++ ) {
      if( SortBuffer[i]->value != 0 && SortBuffer[i-1]->value / SortBuffer[i]->value  < 0.99 ) uOrder = i;
	  SortBuffer[i]->termhood += LOCAL_WEIGHT_2 * uOrder;
	}
	for (i=0; i<SortBufferSize; i++) {//��¼F2����
		SortBuffer[i]->F2Rank = i;
	}


	//��������F
	for ( i=0; i<SortBufferSize; i++) {
		SortBuffer[i]->value = log((double)(SortBuffer[i]->wordCount+1)) / LOG2 * (SortBuffer[i]->frequency - SortBuffer[i]->uMultiParagraphLoneM);
	}
	QuickSortByValue( 0, SortBufferSize - 1);
	uOrder = 1;
	SortBuffer[0]->termhood = LOCAL_WEIGHT_3 * uOrder;
	for( i = 1; i < SortBufferSize; i++ ) {
      if( SortBuffer[i]->value != 0 && SortBuffer[i-1]->value / SortBuffer[i]->value  < 0.99 ) uOrder = i;
	  SortBuffer[i]->termhood += LOCAL_WEIGHT_3 * uOrder;
	}
	for (i=0; i<SortBufferSize; i++) {//��¼F3����
		SortBuffer[i]->F3Rank = i;
	}

}


/***********************************����ֲ����ֶ�LR************************************/
void Extraction::CalcLocalRe_occurrence(void) {
	int i, uOrder;
	
	//������ӵ�LR
	for ( i=0; i<SortBufferSize; i++) {
		SortBuffer[i]->value = 1.0 - 1.0 * (SortBuffer[i]->uMultiSentenceLoneM) / (SortBuffer[i]->frequency);
	}
	QuickSortByValue( 0, SortBufferSize - 1);
	uOrder = 1;
	SortBuffer[0]->termhood += LOCAL_WEIGHT_1 * uOrder;
	for( i = 1; i < SortBufferSize; i++ ) {
		if( SortBuffer[i]->value != 0 && SortBuffer[i-1]->value / SortBuffer[i]->value  < 0.99 ) uOrder = i;
		SortBuffer[i]->termhood += LOCAL_WEIGHT_1 * uOrder;
	}
	
	for (i=0; i<SortBufferSize; i++) {//��¼LR1����
		SortBuffer[i]->LR1Rank = i;
	}
	
	
	//���������LR
	for ( i=0; i<SortBufferSize; i++) {
		SortBuffer[i]->value = 1.0 - 1.0 * (SortBuffer[i]->uParagraphLoneM) / (SortBuffer[i]->frequency);
	}
	QuickSortByValue( 0, SortBufferSize - 1);
	uOrder = 1;
	SortBuffer[0]->termhood += LOCAL_WEIGHT_2 * uOrder;
	for( i = 1; i < SortBufferSize; i++ ) {
		if( SortBuffer[i]->value != 0 && SortBuffer[i-1]->value / SortBuffer[i]->value  < 0.99 ) uOrder = i;
		SortBuffer[i]->termhood += LOCAL_WEIGHT_2 * uOrder;
	}
	for (i=0; i<SortBufferSize; i++) {//��¼LR2����
		SortBuffer[i]->LR2Rank = i;
	}
	
	
	//��������LR
	for ( i=0; i<SortBufferSize; i++) {
		SortBuffer[i]->value = 1.0 - 1.0 * (SortBuffer[i]->uMultiParagraphLoneM) / (SortBuffer[i]->frequency);
	}
	QuickSortByValue( 0, SortBufferSize - 1);
	uOrder = 1;
	SortBuffer[0]->termhood = LOCAL_WEIGHT_3 * uOrder;
	for( i = 1; i < SortBufferSize; i++ ) {
		if( SortBuffer[i]->value != 0 && SortBuffer[i-1]->value / SortBuffer[i]->value  < 0.99 ) uOrder = i;
		SortBuffer[i]->termhood += LOCAL_WEIGHT_3 * uOrder;
	}
	for (i=0; i<SortBufferSize; i++) {//��¼LR3����
		SortBuffer[i]->LR3Rank = i;
	}
	
}

/***********************************����ȫ�����ֶ�GR************************************/
void Extraction::CalcGlobalRe_occurrence(void) {
	int i, uOrder;
	double LOGMAX = log(5.0+1.0);

	//������ӵ�GR
	for ( i=0; i<SortBufferSize; i++) {
		SortBuffer[i]->value =  1.0 * log((double)(SortBuffer[i]->wordCount+1)) / LOGMAX * (SortBuffer[i]->uMultiStClusterStM + SortBuffer[i]->uMultiSentenceLoneM) / uSentenceNo;
	}
	QuickSortByValue( 0, SortBufferSize - 1);
	uOrder = 1;
	SortBuffer[0]->termhood += GLOBAL_WEIGHT_1 * uOrder;
	for( i = 1; i < SortBufferSize; i++ ) {
      if( SortBuffer[i]->value != 0 && SortBuffer[i-1]->value / SortBuffer[i]->value  < 0.99 ) uOrder = i;
	  SortBuffer[i]->termhood += GLOBAL_WEIGHT_1 * uOrder;
	}
	for (i=0; i<SortBufferSize; i++) {//��¼GR1����
		SortBuffer[i]->GR1Rank = i;
	}

	//���������GR
	for ( i=0; i<SortBufferSize; i++) {
		SortBuffer[i]->value = 1.0 * log((double)(SortBuffer[i]->wordCount+1)) / LOGMAX * (SortBuffer[i]->uParaClusterParaM + SortBuffer[i]->uParagraphLoneM) / uSentenceNo;
	}
	QuickSortByValue( 0, SortBufferSize - 1);
	uOrder = 1;
	SortBuffer[0]->termhood += GLOBAL_WEIGHT_2 * uOrder;
	for( i = 1; i < SortBufferSize; i++ ) {
      if( SortBuffer[i]->value != 0 && SortBuffer[i-1]->value / SortBuffer[i]->value  < 0.99 ) uOrder = i;
	  SortBuffer[i]->termhood += GLOBAL_WEIGHT_2 * uOrder;
	}
	for (i=0; i<SortBufferSize; i++) {//��¼GR2��
		SortBuffer[i]->GR2Rank = i;
	}

	//��������GR
	for ( i=0; i<SortBufferSize; i++) {
		SortBuffer[i]->value = 1.0 * log((double)(SortBuffer[i]->wordCount+1)) / LOGMAX * (SortBuffer[i]->uMultiParaClusterParaM + SortBuffer[i]->uMultiParagraphLoneM) / uSentenceNo;
	}
	QuickSortByValue( 0, SortBufferSize - 1);
	uOrder = 1;
	SortBuffer[0]->termhood = GLOBAL_WEIGHT_3 * uOrder;
	for( i = 1; i < SortBufferSize; i++ ) {
      if( SortBuffer[i]->value != 0 && SortBuffer[i-1]->value / SortBuffer[i]->value  < 0.99 ) uOrder = i;
	  SortBuffer[i]->termhood += GLOBAL_WEIGHT_3 * uOrder;
	}
	for (i=0; i<SortBufferSize; i++) {//��¼GR3����
		SortBuffer[i]->GR3Rank = i;
	}
}




/*���ڴ����޳�����ȡ�Ĺؼ����ж���Ƶ�ʹ��ͣ���termhood̫С�Ĺؼ���*/
int	 Extraction::PostProcess() { 
	int i, RefineBufferSize = 0;
	Unit **RefineBuffer = new Unit*[SortBufferSize];
	for (i=0; i<SortBufferSize; i++) {
		int idpFreq = SortBuffer[i]->frequency - SortBuffer[i]->nestedFreq;
		//����Ƶ�ʴ�����ֵ��termhood>1.0�������>1.0
//		if ( idpFreq > INDEPENDENT_THRESHOLD && SortBuffer[i]->termhood > 1.0 && SortBuffer[i]->strange > 0.4) { 
		if ( idpFreq > INDEPENDENT_THRESHOLD && SortBuffer[i]->termhood > 1.0 ) { 
			RefineBuffer[RefineBufferSize++] = SortBuffer[i];
		}
	}
	for (i=0; i<RefineBufferSize; i++) {
		SortBuffer[i] = RefineBuffer[i];
	}
	SortBufferSize = RefineBufferSize; //�޳�һЩ���Ĺؼ��ʺ����¶���SortBufferSize
	delete[] RefineBuffer;
	return RefineBufferSize;
}


/***********************************���������S************************************/
void Extraction::CalcSingularity() {
	int i, j, uOrder;
	for ( i=0; i<SortBufferSize; i++) {
		wstring wstr;
		for (j=0; j<SortBuffer[i]->wordCount; j++) wstr.append(SortBuffer[i]->pTerm[j]);
		SortBuffer[i]->value = 1.0 * SortBuffer[i]->frequency / data.GetFrequency((wchar_t *)wstr.c_str());
		SortBuffer[i]->strange = SortBuffer[i]->value;
	}
	QuickSortByValue( 0, SortBufferSize - 1);
	uOrder = 1;
	SortBuffer[0]->termhood += GLOBAL_WEIGHT_1 * uOrder;
	for( i = 1; i < SortBufferSize; i++ ) {
		if( SortBuffer[i]->value != 0 && SortBuffer[i-1]->value / SortBuffer[i]->value  < 0.99 ) uOrder = i;
		SortBuffer[i]->termhood += GLOBAL_WEIGHT_1 * uOrder;
	}
	for (i=0; i<SortBufferSize; i++) {//��¼singularity����
		SortBuffer[i]->sinRank = i;
	}
}

/***********************************����������ݿ�************************************/
void Extraction::StoreIntoDatabase(wchar_t view[]) { //����ȡ�����Ĺؼ��ʴ浽���ݿ�
	data.StoreIntoDatabase(SortBuffer, SortBufferSize, view);
}


//����value��С��������
void Extraction::QuickSortByValue( int iBegin, int iEnd) {
	int k;
	while ( iBegin < iEnd ) {
		k = PartitionByValue(iBegin,iEnd);
		if( k-iBegin < iEnd - k) { //ǰ���ֵļ�¼�Ƚ��٣�������
			if( ( k - iBegin )>= 2 ) QuickSortByValue(iBegin, k-1 );
			iBegin = k+1;
		}
		else {                     //�󲿷ֵļ�¼�Ƚ��٣�������
			if( ( iEnd - k ) >=2 ) QuickSortByValue(k+1, iEnd );
			iEnd = k -1;
		}
	}
	return;
}

//���մʴ��ĵ��ʸ����Ӷൽ������
void Extraction::QuickSortByLength( int iBegin, int iEnd) {
	int k;
	while ( iBegin < iEnd ) {
		k = PartitionByLength(iBegin,iEnd);
		if( k-iBegin < iEnd - k) { //ǰ���ֵļ�¼�Ƚ��٣�������
			if( ( k - iBegin )>= 2 ) QuickSortByLength(iBegin, k-1 );
			iBegin = k+1;
		}
		else {                     //�󲿷ֵļ�¼�Ƚ��٣�������
			if( ( iEnd - k ) >=2 ) QuickSortByLength(k+1, iEnd );
			iEnd = k -1;
		}
	}
	return;
}

//����termhood�Ӵ�С����
void Extraction::QuickSortByTermhood( int iBegin, int iEnd) {
	int k;
	while ( iBegin < iEnd ) {
		k = PartitionByTermhood(iBegin,iEnd);
		if( k-iBegin < iEnd - k) { //ǰ���ֵļ�¼�Ƚ��٣�������
			if( ( k - iBegin )>= 2 ) QuickSortByTermhood(iBegin, k-1 );
			iBegin = k+1;
		}
		else {                     //�󲿷ֵļ�¼�Ƚ��٣�������
			if( ( iEnd - k ) >=2 ) QuickSortByTermhood(k+1, iEnd );
			iEnd = k -1;
		}
	}
	return;
}

int Extraction::PartitionByValue( int low, int high) {
	Unit *pivot = SortBuffer[low];
	while ( low < high) {
		while ( low < high && (SortBuffer[high]->value >= pivot->value) ) high--;
		SortBuffer[low] = SortBuffer[high];
		while ( low < high && (SortBuffer[low]->value <= pivot->value) ) low++;
		SortBuffer[high] = SortBuffer[low];
	}
	SortBuffer[low] = pivot;
	return low;
}

int Extraction::PartitionByLength( int low, int high) {
	Unit *pivot = SortBuffer[low];
	while ( low < high) {
		while ( low < high && (SortBuffer[high]->wordCount <= pivot->wordCount) ) high--;
		SortBuffer[low] = SortBuffer[high];
		while ( low < high && (SortBuffer[low]->wordCount >= pivot->wordCount) ) low++;
		SortBuffer[high] = SortBuffer[low];
	}
	SortBuffer[low] = pivot;
	return low;
}

int Extraction::PartitionByTermhood( int low, int high) {
	Unit *pivot = SortBuffer[low];
	while ( low < high) {
		while ( low < high && (SortBuffer[high]->termhood <= pivot->termhood) ) high--;
		SortBuffer[low] = SortBuffer[high];
		while ( low < high && (SortBuffer[low]->termhood >= pivot->termhood) ) low++;
		SortBuffer[high] = SortBuffer[low];
	}
	SortBuffer[low] = pivot;
	return low;
}




