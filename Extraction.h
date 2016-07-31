/*��Ĺ��ܣ��������ĸ�������ֵ�����ۺ���Щֵ����ȡ�ؼ���*/

#ifndef EXTRACTION_H
#define EXTRACTION_H
#include <iostream>
#include <string>
#include "Data.h"
using namespace std;
#define MULTI_SENTENCE_THRESHOLD 2		//������2�����Ǿֲ��ۺϵ�
#define MULTI_PARAGRAPH_THRESHOLD 3		//������3�����Ǿֲ��ۺϵ�

#define LOCAL_WEIGHT_1		0.15		//������ӵľֲ����ֶ�Ȩ��
#define LOCAL_WEIGHT_2		0.10		//��������ľֲ����ֶ�Ȩ��
#define LOCAL_WEIGHT_3		0.05		//�������ľֲ����ֶ�Ȩ�����ԭ����1.0�ĳ�0.0����Ҳû��ʲôӰ��
#define GLOBAL_WEIGHT_1		0.15		//������ӵ�ȫ�����ֶ�Ȩ��
#define GLOBAL_WEIGHT_2		0.10		//���������ȫ�����ֶ�Ȩ��
#define GLOBAL_WEIGHT_3		0.05		//��������ȫ�����ֶ�Ȩ��
#define C_VALUE_WEIGHT		0.20		//C-valueֵȨ��
#define COMMON_WEIGHT		0.10		//��ͨ�̶�ֵȨ��

class Extraction {
private:
	
	HashTable hashTable;
	wchar_t *pView; //ָ��ִ�����Unicode�ı�
	int uSentenceNo; 
	int uParagraphNo;
	int wordNo;
	vector<Unit*> Text;

	int	 PartitionByValue( int low, int high);
	int	 PartitionByLength( int low, int high);
	int	 PartitionByTermhood( int low, int high); 
	void QuickSortByValue( int iBegin, int iEnd); //������Unit����value����
	void QuickSortByLength( int iBegin, int iEnd); //������Unit�������������������
	void QuickSortByTermhood( int iBegin, int iEnd); //������Unit����termhood����
	int  WordSegmentation(char *fileName); //�ִ�
	void CalcFrequency(void); //������ʴ���Ƶ��
	void CalcAggregateFrequency(void); //����ʴ�����ЧƵ��
	void CalcLocalRe_occurrence(void); //����ʴ��ľֲ����ֶ�
	void CalcGlobalRe_occurrence(void); //����ʴ���ȫ�����ֶ�
	int	 LoadToBuffer(void); //����ϣ���е���Ч�ִ���Ԫ���뵽�������Թ�����
	void CalcC_value(void); //����ʴ���C-valueֵ

	int  PreProcess(char *fileName);
	int	 PostProcess(); //���ڴ����޳�����ȡ�Ĺؼ����ж���Ƶ�ʹ��ͣ���termhood̫С�Ĺؼ���
	void CalcSingularity();
public:
	Data data; //���ݼ�鹤�ߣ����ڼ��ͣ�ôʡ����ʵȵ�
	Unit **SortBuffer;
	int SortBufferSize; 
	
	void StoreIntoDatabase(wchar_t view[]); //���������������ݿ�
	Extraction(); //ͨ��һ���ʵ乹��һ���ؼ��ʳ�ȡ��
	~Extraction();
	int ProcessC(char *fileName); //�����ı����ִʡ�ͳ��Ƶ�ʡ��ֲ�ȫ�����ֶȡ�C-value��д�����ݿ⣩
	int ProcessCL(char *fileName); //�����ı����ִʡ�ͳ��Ƶ�ʡ��ֲ�ȫ�����ֶȡ�C-value��д�����ݿ⣩
	int ProcessCF(char *fileName); //�����ı����ִʡ�ͳ��Ƶ�ʡ���ЧƵ�ʡ�C-value��д�����ݿ⣩
	int ProcessCG(char *fileName); //�����ı����ִʡ�ͳ��Ƶ�ʡ��ֲ�ȫ�����ֶȡ�C-value��д�����ݿ⣩
	int ProcessCLG(char *fileName); //�����ı����ִʡ�ͳ��Ƶ�ʡ��ֲ�ȫ�����ֶȡ�C-value��д�����ݿ⣩
	int ProcessCFG(char *fileName); //�����ı����ִʡ�ͳ��Ƶ�ʡ���ЧƵ�ʡ�C-value��д�����ݿ⣩
	int ProcessCLGS(char *fileName); //�����ı����ִʡ�ͳ��Ƶ�ʡ��ֲ�ȫ�����ֶȡ�C-value��д�����ݿ⣩
	int ProcessCFGS(char *fileName); //�����ı����ִʡ�ͳ��Ƶ�ʡ���ЧƵ�ʡ�C-value��д�����ݿ⣩
};

#endif