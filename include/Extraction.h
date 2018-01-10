/*类的功能：计算词语的各种特征值，并综合这些值来抽取关键词*/

#ifndef EXTRACTION_H
#define EXTRACTION_H
#include <iostream>
#include <string>
#include "Data.h"
using namespace std;
#define MULTI_SENTENCE_THRESHOLD 2		//短语在2句内是局部聚合的
#define MULTI_PARAGRAPH_THRESHOLD 3		//短语在3段内是局部聚合的

#define LOCAL_WEIGHT_1		0.15		//多个句子的局部重现度权重
#define LOCAL_WEIGHT_2		0.10		//单个段落的局部重现度权重
#define LOCAL_WEIGHT_3		0.05		//多个段落的局部重现度权重这个原来是1.0改成0.0好像也没有什么影响
#define GLOBAL_WEIGHT_1		0.15		//多个句子的全局重现度权重
#define GLOBAL_WEIGHT_2		0.10		//单个段落的全局重现度权重
#define GLOBAL_WEIGHT_3		0.05		//多个段落的全局重现度权重
#define C_VALUE_WEIGHT		0.20		//C-value值权重
#define COMMON_WEIGHT		0.10		//普通程度值权重

class Extraction {
private:
	
	HashTable hashTable;
	wchar_t *pView; //指向分词完后的Unicode文本
	int uSentenceNo; 
	int uParagraphNo;
	int wordNo;
	vector<Unit*> Text;

	int	 PartitionByValue( int low, int high);
	int	 PartitionByLength( int low, int high);
	int	 PartitionByTermhood( int low, int high); 
	void QuickSortByValue( int iBegin, int iEnd); //将短语Unit按照value排序
	void QuickSortByLength( int iBegin, int iEnd); //将短语Unit按照所含词语个数排序
	void QuickSortByTermhood( int iBegin, int iEnd); //将短语Unit按照termhood排序
	int  WordSegmentation(char *fileName); //分词
	void CalcFrequency(void); //计算各词串的频率
	void CalcAggregateFrequency(void); //计算词串的有效频率
	void CalcLocalRe_occurrence(void); //计算词串的局部重现度
	void CalcGlobalRe_occurrence(void); //计算词串的全局重现度
	int	 LoadToBuffer(void); //将哈希表中的有效字串单元导入到缓存中以供操作
	void CalcC_value(void); //计算词串的C-value值

	int  PreProcess(char *fileName);
	int	 PostProcess(); //后期处理，剔除所抽取的关键词中独立频率过低，或termhood太小的关键词
	void CalcSingularity();
public:
	Data data; //数据检查工具，用于检查停用词、量词等等
	Unit **SortBuffer;
	int SortBufferSize; 
	
	void StoreIntoDatabase(wchar_t view[]); //将计算结果存入数据库
	Extraction(); //通过一个词典构造一个关键词抽取器
	~Extraction();
	int ProcessC(char *fileName); //处理文本（分词、统计频率、局部全局重现度、C-value、写入数据库）
	int ProcessCL(char *fileName); //处理文本（分词、统计频率、局部全局重现度、C-value、写入数据库）
	int ProcessCF(char *fileName); //处理文本（分词、统计频率、有效频率、C-value、写入数据库）
	int ProcessCG(char *fileName); //处理文本（分词、统计频率、局部全局重现度、C-value、写入数据库）
	int ProcessCLG(char *fileName); //处理文本（分词、统计频率、局部全局重现度、C-value、写入数据库）
	int ProcessCFG(char *fileName); //处理文本（分词、统计频率、有效频率、C-value、写入数据库）
	int ProcessCLGS(char *fileName); //处理文本（分词、统计频率、局部全局重现度、C-value、写入数据库）
	int ProcessCFGS(char *fileName); //处理文本（分词、统计频率、有效频率、C-value、写入数据库）
};

#endif