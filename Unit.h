/*类的功能：表示短语的单元，该单元包含了一个短语的各种特征值，这些单元存入哈希表中供快速读取*/

#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <string.h>
using namespace std;
#define MAX_WORD_NUM 6   //短语中最多6个词语

//一个短语和这个短语的信息（如出现频率）组成一个Unit单元【注：在本程序中词串、短语、字串这三个说法都是一样的】
class Unit {
public:
	wchar_t	*pTerm[MAX_WORD_NUM+1]; //指针数组，每个指针指向pView中的一个位置，这样这些指针指向的字符串连起来就是term
	int		wordCount; //词串中词语的个数
	int		charCount; //词串中字的个数
	int		frequency; //该词串出现的总频率
	int		nestedFreq; //被嵌套在其他词串中的频率 所以 frequency - nestedFreq = 词串独立出现的概率
	int		nesterCount; //包含该词串的更长词串的个数
	double	termhood; //词串被视为关键词的度量指标
	double	value; //用于存放各种计算结果
	double	strange;
	Unit	*next; //哈希表中用于连接下一个Unit

	int	uMultiSentenceLoneM;	//多句孤独簇个数
	int	uParagraphLoneM;		//单段孤独簇个数
	int	uMultiParagraphLoneM;	//多段孤独簇个数

	int	uMultiStClusterStM;		//多句聚合簇的句子数
	int	uParaClusterParaM;		//单段聚合簇的段落数
	int	uMultiParaClusterParaM;	//多段聚合簇的段落数

	bool			bMultiSentenceNowNotLone;
	bool			bParagraphNowNotLone;
	bool			bMultiParagraphNowNotLone;

	int	uLastSentenceNo;		//该字串上一次出现时的句子号
	int	uLastParagraphNo;		//该字串上一次出现时的段号

	int cvalueRank;
	int F1Rank;
	int F2Rank;
	int F3Rank;
	int LR1Rank;
	int LR2Rank;
	int LR3Rank;
	int GR1Rank;
	int GR2Rank;
	int GR3Rank;
	int sinRank;

	Unit();
	Unit(wchar_t *_term[], int len);
};

#endif