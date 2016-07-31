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

//FREQUENCY_THRESHOLD的设置尤为重要，DataStruct文本总共39000个词语，<=3的有35000,所以可取的是将FREQUENCY_THRESHOLD设置为3
//就是因为很多所谓的关键词其实频率非常低小于6的有70个，这些都不应该算是关键词的吧！！！
int FREQUENCY_THRESHOLD = 0;
int INDEPENDENT_THRESHOLD = 0;


//打印当前时间
void showTime(FILE *fp, char *str) {
	SYSTEMTIME sys;
	GetLocalTime( &sys ); 
	fprintf(fp, "%s\t%02d:%02d:%02d.%03d\n", str, sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
}

//fileName要进行编码转换的GB*码文本，uCodeTextLen转换后的长度，返回转换后的字符数组首地址
//使用完后需要手动释放该缓冲区。
//转换后的Unicode数组缓冲区长度包括开头的0xfeff，结尾没有\0。
wchar_t* g2u(int *uCodeTextLen, char *fileName) {
	char *gbCodeText;
	int gbCodeTextLen;
	const char *toCodeName= "utf16";
	const char *fromCodeName = "gb2312";	
	char *uCodeText;
	int ret;
	FILE *fp;
	
	//导入gb编码文件到内存中
	fp = fopen(fileName, "rb");
	fseek(fp, 0, SEEK_END);
	gbCodeTextLen = ftell(fp);
	rewind(fp);
	gbCodeText = new char[gbCodeTextLen];
	fread(gbCodeText, gbCodeTextLen, 1, fp);
	fclose(fp);
	//转换字符编码方式
	UErrorCode ErrorCode = U_ZERO_ERROR; 
	ret = ucnv_convert(toCodeName, fromCodeName, NULL, 0, gbCodeText, gbCodeTextLen, &ErrorCode);
	uCodeText = new char[ret];
	*uCodeTextLen = ret >> 1;
	ErrorCode = U_ZERO_ERROR; //每次使用前要置为U_ZERO_ERROR
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
	Text.reserve(250000); //初始25万个指针，共1M大小。
}


//析构函数
Extraction::~Extraction() {
	if (SortBuffer != NULL) delete[] SortBuffer;
	SortBuffer = NULL;
	if (pView != NULL) delete[] pView;
	pView = NULL;
}

//检查文件是否是ANSI格式，并且设置相应的阈值
int Extraction::PreProcess(char *fileName) {
	long fileSize;
	unsigned char ch[3];
	FILE *fp = fopen(fileName, "rb");
	fread(ch, sizeof(ch), 1, fp);
	
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fclose(fp);
	//对于频率的限制非常有必要，因为小概率的词语非常多，一些很长的短语频率只有1
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
	remove("池文振.txt");
	FILE *fp = fopen("池文振.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //若fileName不是ANSI编码格式的文档，返回-1错误
	showTime(fp, fileName);
	showTime(fp, "1 开始分词         ");
	if (WordSegmentation(fileName) == -1) return -2; //分词出错
	showTime(fp, "2 开始统计频率     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {

		showTime(fp,"3 开始计算C-value   ");
		CalcC_value();

		showTime(fp,"4 后期处理          ");
		PostProcess();
		QuickSortByTermhood( 0, SortBufferSize-1); //按照termhood从大到小排序
	}
	showTime(fp,"5 运行结束          ");
	fclose(fp);
	return 0;
}

//C+L
int Extraction::ProcessCL(char *fileName) {
	remove("池文振.txt");
	FILE *fp = fopen("池文振.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //若fileName不是ANSI编码格式的文档，返回-1错误
	showTime(fp, fileName);
	showTime(fp, "1 开始分词         ");
	if (WordSegmentation(fileName) == -1) return -2; //分词出错
	showTime(fp, "2 开始统计频率     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {
		
		showTime(fp,"3 开始计算C-value   ");
		CalcC_value();

		showTime(fp,"4 开始计算局部重现度");
		CalcLocalRe_occurrence();
		
		showTime(fp,"5 后期处理          ");
		PostProcess();
		QuickSortByTermhood( 0, SortBufferSize-1); //按照termhood从大到小排序
	}
	showTime(fp,"6 运行结束          ");
	fclose(fp);
	return 0;
}


//C+F
int Extraction::ProcessCF(char *fileName) {
	remove("池文振.txt");
	FILE *fp = fopen("池文振.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //若fileName不是ANSI编码格式的文档，返回-1错误
	showTime(fp, fileName);
	showTime(fp, "1 开始分词         ");
	if (WordSegmentation(fileName) == -1) return -2; //分词出错
	showTime(fp, "2 开始统计频率     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {
		
		showTime(fp,"3 开始计算C-value   ");
		CalcC_value();
		
		showTime(fp,"4 开始计算有效频率");
		CalcAggregateFrequency();
		
		showTime(fp,"5 后期处理          ");
		PostProcess();
		QuickSortByTermhood( 0, SortBufferSize-1); //按照termhood从大到小排序
	}
	showTime(fp,"6 运行结束          ");
	fclose(fp);
	return 0;
}

//C+G
int Extraction::ProcessCG(char *fileName) {
	remove("池文振.txt");
	FILE *fp = fopen("池文振.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //若fileName不是ANSI编码格式的文档，返回-1错误
	showTime(fp, fileName);
	showTime(fp, "1 开始分词         ");
	if (WordSegmentation(fileName) == -1) return -2; //分词出错
	showTime(fp, "2 开始统计频率     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {
		
		
		showTime(fp,"3 开始计算C-value   ");
		CalcC_value();

		showTime(fp,"4 开始计算全局重现度");
		CalcGlobalRe_occurrence();
		
		showTime(fp,"5 后期处理          ");
		PostProcess();
		QuickSortByTermhood( 0, SortBufferSize-1); //按照termhood从大到小排序
	}
	showTime(fp,"10 运行结束          ");
	fclose(fp);
	return 0;
}

//C+L+G
int Extraction::ProcessCLG(char *fileName) {
	remove("池文振.txt");
	FILE *fp = fopen("池文振.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //若fileName不是ANSI编码格式的文档，返回-1错误
	showTime(fp, fileName);
	showTime(fp, "1 开始分词         ");
	if (WordSegmentation(fileName) == -1) return -2; //分词出错
	showTime(fp, "2 开始统计频率     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {

		showTime(fp,"3 开始计算C-value   ");
		CalcC_value();
		
		showTime(fp,"4 开始计算局部重现度");
		CalcLocalRe_occurrence();
		
		showTime(fp,"5 开始计算全局重现度");
		CalcGlobalRe_occurrence();
		
		showTime(fp,"6 后期处理          ");
		PostProcess();
 		QuickSortByTermhood( 0, SortBufferSize-1); //按照termhood从大到小排序



// 		int num = 400;
// 		for (int i=0; i<num; i++)
// 		{
// 			SortBuffer[i]->termhood = 0.0; //选出前num个termhood重新归零，并且用C+G重新赋值排列
// 		}
// 		SortBufferSize = num;
// 		showTime(fp,"重新计算C-value   ");
// 		CalcC_value();
// 		showTime(fp,"重新计算全局重现度");
// 		CalcGlobalRe_occurrence();
// 		QuickSortByTermhood( 0, num-1); //按照termhood从大到小排序（前400个）
		

	}
	showTime(fp,"7 运行结束          ");
	fclose(fp);
	return 0;
}

//C+F+G
int Extraction::ProcessCFG(char *fileName) {
	remove("池文振.txt");
	FILE *fp = fopen("池文振.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //若fileName不是ANSI编码格式的文档，返回-1错误
	showTime(fp, fileName);
	showTime(fp, "1 开始分词         ");
	if (WordSegmentation(fileName) == -1) return -2; //分词出错
	showTime(fp, "2 开始统计频率     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {
		
		showTime(fp,"3 开始计算C-value   ");
		CalcC_value();
		
		showTime(fp,"4 开始计算有效频率");
		CalcAggregateFrequency();
		
		showTime(fp,"5 开始计算全局重现度");
		CalcGlobalRe_occurrence();
		
		showTime(fp,"6 后期处理          ");
		PostProcess();
		QuickSortByTermhood( 0, SortBufferSize-1); //按照termhood从大到小排序
		
		
		
		// 		int num = 400;
		// 		for (int i=0; i<num; i++)
		// 		{
		// 			SortBuffer[i]->termhood = 0.0; //选出前num个termhood重新归零，并且用C+G重新赋值排列
		// 		}
		// 		SortBufferSize = num;
		// 		showTime(fp,"重新计算C-value   ");
		// 		CalcC_value();
		// 		showTime(fp,"重新计算全局重现度");
		// 		CalcGlobalRe_occurrence();
		// 		QuickSortByTermhood( 0, num-1); //按照termhood从大到小排序（前400个）
		
		
	}
	showTime(fp,"7 运行结束          ");
	fclose(fp);
	return 0;
}

//C+L+G+S
int Extraction::ProcessCLGS(char *fileName) {
	remove("池文振.txt");
	FILE *fp = fopen("池文振.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //若fileName不是ANSI编码格式的文档，返回-1错误
	showTime(fp, fileName);
	showTime(fp, "1 开始分词         ");
	if (WordSegmentation(fileName) == -1) return -2; //分词出错
	showTime(fp, "2 开始统计频率     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {
		
		showTime(fp,"3 开始计算C-value   ");
		CalcC_value();
		
		showTime(fp,"4 开始计算局部重现度");
		CalcLocalRe_occurrence();
		
		showTime(fp,"5 开始计算全局重现度");
		CalcGlobalRe_occurrence();
		
		showTime(fp,"6 开始计算平常度    ");
		CalcSingularity();
		
		showTime(fp,"7 后期处理          ");
		PostProcess();
		QuickSortByTermhood( 0, SortBufferSize-1); //按照termhood从大到小排序


// 		int num = 400;
// 		for (int i=0; i<num; i++)
// 		{
// 			SortBuffer[i]->termhood = 0.0; //选出前num个termhood重新归零，并且用C+G重新赋值排列
// 		}
// 		SortBufferSize = num;
// 		showTime(fp,"重新计算C-value   ");
// 		CalcC_value();
// 		showTime(fp,"重新计算全局重现度");
// 		CalcGlobalRe_occurrence();
// 		QuickSortByTermhood( 0, num-1); //按照termhood从大到小排序（前400个）


	}
	showTime(fp,"8 运行结束          ");
	fclose(fp);
	return 0;
}


//C+F+G+S
int Extraction::ProcessCFGS(char *fileName) {
	remove("池文振.txt");
	FILE *fp = fopen("池文振.txt", "a");
	if (PreProcess(fileName) == -1) return -1; //若fileName不是ANSI编码格式的文档，返回-1错误
	showTime(fp, fileName);
	showTime(fp, "1 开始分词         ");
	if (WordSegmentation(fileName) == -1) return -2; //分词出错
	showTime(fp, "2 开始统计频率     ");
	CalcFrequency();
	if ( LoadToBuffer() == 0 ) {
		
		showTime(fp,"3 开始计算C-value   ");
		CalcC_value();
		
		showTime(fp,"4 开始计算有效频率");
		CalcAggregateFrequency();
		
		showTime(fp,"5 开始计算全局重现度");
		CalcGlobalRe_occurrence();
		
		showTime(fp,"6 开始计算平常度    ");
		CalcSingularity();
		
		showTime(fp,"7 后期处理          ");
		PostProcess();
		QuickSortByTermhood( 0, SortBufferSize-1); //按照termhood从大到小排序
		
		
		// 		int num = 400;
		// 		for (int i=0; i<num; i++)
		// 		{
		// 			SortBuffer[i]->termhood = 0.0; //选出前num个termhood重新归零，并且用C+G重新赋值排列
		// 		}
		// 		SortBufferSize = num;
		// 		showTime(fp,"重新计算C-value   ");
		// 		CalcC_value();
		// 		showTime(fp,"重新计算全局重现度");
		// 		CalcGlobalRe_occurrence();
		// 		QuickSortByTermhood( 0, num-1); //按照termhood从大到小排序（前400个）
		
		
	}
	showTime(fp,"8 运行结束          ");
	fclose(fp);
	return 0;
}

/***********************************对文本分词************************************/
int Extraction::WordSegmentation(char *fileName) {
	if(!ICTCLAS_Init()) return -1;
	ICTCLAS_FileProcess(fileName, "temp.txt", 1);
	ICTCLAS_Exit();
	return 0;
}

/***********************************计算文本中词串的频率值************************************/
void Extraction::CalcFrequency(void) {
	Unit *p;
	int r, s, indexWord, indexChar, iChatacterCount;
	uSentenceNo = 1; 
	uParagraphNo = 1;
	wordNo = 0; 
	indexWord = 0;
	indexChar = 0;
	
	//分词结果放在temp.txt，下面将其导入到内存中，用pView指向它
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

		/*读出以空格隔开的词语，并依次放到wordSequence这个字符串数组中*/
			
		wchar_t *wordSequence[50] = {0};
		wchar_t POS[50] = {0};
		for ( indexWord=0; indexWord<50; indexWord++ ) {
			wordSequence[indexWord] = &pView[i];
			while ( pView[i] != 0x002f ) i++; //只要没有到'/',这个词语就还没有结束
			pView[i++] = 0x0000; //将'/'设为0，那么词语就有一个结束符，后面可以直接使用该词
			if ( IsBadPOS(pView[i]) || data.IsInStopList(&wordSequence[indexWord], 1) ) { //如果是无意义词性或停用词则舍弃
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
			while ( IsLowerCaseOrSpace(pView[i])) i++; //如果下一个字还是空格或英文小写字母的话，就继续往下找，直到下一个词语的开头
			if ( !IsChineseCharacter(pView[i]) ) break; //如果不是汉字，说明这个segment到头了，开始下一轮segment的寻找
		}
		//求出wordSequence中所有子串, 并插入哈希表
		for ( r=0; r<=indexWord; r++ ) {
				for ( s=r; s<=indexWord; s++ ) {
					//短语中词语数目过多
					if ( s - r  > 4 ) break;
					
					//非名词的单字，或只有一个形容词，舍弃
					if ( s - r == 0 
						&& ( (POS[r]==ADJ) || (::wcslen(wordSequence[r])==1 && POS[r] != NOUN) )) continue; 
					//副词+形容词 舍弃，这个确实能去无用词
					//fv或da舍弃，fv舍弃其实不怎么好，但是fv的关键词只是少部分（外排，内部归并，内部排序）
					if ( s - r == 1 
						&& ( (POS[r]==ORNT && POS[s]==VERB) || (POS[r]==ADV && POS[r+1]==ADJ)))  continue;
					//量词开头或方位词、副词、区别词结尾舍弃
					if ( POS[r]==CLAS 
						|| POS[s]==DIS 
						|| POS[s]==ORNT 
						|| POS[s]==ADV
						|| POS[s]==NUM) continue;

					//后缀是一些需要剔除的字眼如来、去、于、到、有等等
					if ( ::wcslen(wordSequence[s])==1 && Postfix(*(wordSequence[s])) ) continue;

					//后缀是一些需要剔除的字眼如将、要、如
					if ( ::wcslen(wordSequence[r])==1 && Prefix(*(wordSequence[r])) ) continue;
					
					if ( data.IsQuantifier( &wordSequence[r], s - r + 1) ) continue; //数量词，舍弃
					if ( data.IsInStopList(&wordSequence[r], s - r + 1) ) continue; //停用词，舍弃

					wordNo++; //词语个数增加

					if ( hashTable.Find(&wordSequence[r], s - r + 1, &p) ) {//该短语已经出现过
						p->frequency++;
						//处理以多个句子为划分标准的簇
						if( (uSentenceNo - p->uLastSentenceNo) > MULTI_SENTENCE_THRESHOLD ) {
							p->uMultiSentenceLoneM++;
							p->bMultiSentenceNowNotLone = false;
						}
						else {
							if(p->bMultiSentenceNowNotLone == false ) {
								p->uMultiStClusterStM++;///全局
								p->uMultiSentenceLoneM--;
								p->bMultiSentenceNowNotLone = true;
							}
							p->uMultiStClusterStM += uSentenceNo - p->uLastSentenceNo;///全局
						}
						
						//处理以单个段落为划分标准的簇
						if( p->uLastParagraphNo != uParagraphNo ) {
							//该字串上次出现的段号和这次不同，即不同段
							p->uParagraphLoneM++;
							p->bParagraphNowNotLone = false;
						}
						else { //该字串上次出现的段号和这次相同，即同段
							if( p->bParagraphNowNotLone == false ) {
								//当前词是该簇第二次出现
								p->uParaClusterParaM++;///全局
								p->uParagraphLoneM--;
								p->bParagraphNowNotLone = true;
							}
								p->uParaClusterParaM += uParagraphNo - p->uLastParagraphNo;
						}

						//处理以多个段落为划分标准的簇
						if( (uParagraphNo - p->uLastParagraphNo) > MULTI_PARAGRAPH_THRESHOLD ) {
							p->uMultiParagraphLoneM++;
							p->bMultiParagraphNowNotLone = false;
						}
						else {
							if( p->bMultiParagraphNowNotLone == false ) {
								p->uMultiParaClusterParaM++;///全局
								p->uMultiParagraphLoneM--;
								p->bMultiParagraphNowNotLone = true;
							}
							p->uMultiParaClusterParaM += uParagraphNo - p->uLastParagraphNo;///全局
						}
					}
					else { //该短语首次出现,p为可以插入哈希表的位置
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



/***********************************将哈希表中每个有效数据的unit指针存入SortBuffer************************************/
int Extraction::LoadToBuffer(void) {
	Unit *p;
	int indexSize = 0;
	SortBufferSize = hashTable.Size();
	SortBuffer = new Unit*[SortBufferSize];

	while ( ( p = hashTable.GetNext() ) != NULL ) { 
		if ( p->frequency > FREQUENCY_THRESHOLD ) SortBuffer[indexSize++] = p; //只保留频数大于阈值的词串
	}
	SortBufferSize = indexSize;
	if (SortBufferSize > 0) return 0;
	else return 1; //没有数据被存入SortBuffer，返回1
}


/***********************************计算C-value值************************************/
void Extraction::CalcC_value(void) {
	int i, j, r, s, uOrder;
	Unit *pUnit;
	wchar_t **pTerm;
	double LOG2 = log(2.0);
	QuickSortByLength(0, SortBufferSize - 1); //按照长度从大到小排序（长度指所含单词个数）
	
	//找出每个词串的子串，并更新子串的被嵌套概率(nestedFreq)
	for ( i = 0; i < SortBufferSize; i++ ) {
		pTerm = SortBuffer[i]->pTerm;
		for ( r=0; r<SortBuffer[i]->wordCount; r++ ) {
			for ( s=r; s<SortBuffer[i]->wordCount; s++ ) {
				if (s - r + 1 < SortBuffer[i]->wordCount) { //本身不计
					if ( hashTable.Find(&pTerm[r], s - r + 1, &pUnit) ) {
						pUnit->nesterCount++;
						pUnit->nestedFreq += SortBuffer[i]->frequency - SortBuffer[i]->nestedFreq;
					}
				}
			}
		}
	}
	
	for ( i = 0; i < SortBufferSize; i++ ) {

		//计算词串包含的字数
		int charCount = 0;
		for (j=0; j<SortBuffer[i]->wordCount; j++) {
			charCount = charCount + wcslen(SortBuffer[i]->pTerm[j]);
		}
		SortBuffer[i]->charCount = charCount;

		//计算C-value
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
	for (i=0; i<SortBufferSize; i++) {//记录C-value排名
		SortBuffer[i]->cvalueRank = i;
	}
}

/***********************************计算聚合频率F************************************/
void Extraction::CalcAggregateFrequency(void) {
	int i, uOrder;
	double LOG2 = log(2.0);

	//多个句子的F
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
	
	for (i=0; i<SortBufferSize; i++) {//记录F1排名
		SortBuffer[i]->F1Rank = i;
	}


	//单个段落的F
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
	for (i=0; i<SortBufferSize; i++) {//记录F2排名
		SortBuffer[i]->F2Rank = i;
	}


	//多个段落的F
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
	for (i=0; i<SortBufferSize; i++) {//记录F3排名
		SortBuffer[i]->F3Rank = i;
	}

}


/***********************************计算局部重现度LR************************************/
void Extraction::CalcLocalRe_occurrence(void) {
	int i, uOrder;
	
	//多个句子的LR
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
	
	for (i=0; i<SortBufferSize; i++) {//记录LR1排名
		SortBuffer[i]->LR1Rank = i;
	}
	
	
	//单个段落的LR
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
	for (i=0; i<SortBufferSize; i++) {//记录LR2排名
		SortBuffer[i]->LR2Rank = i;
	}
	
	
	//多个段落的LR
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
	for (i=0; i<SortBufferSize; i++) {//记录LR3排名
		SortBuffer[i]->LR3Rank = i;
	}
	
}

/***********************************计算全局重现度GR************************************/
void Extraction::CalcGlobalRe_occurrence(void) {
	int i, uOrder;
	double LOGMAX = log(5.0+1.0);

	//多个句子的GR
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
	for (i=0; i<SortBufferSize; i++) {//记录GR1排名
		SortBuffer[i]->GR1Rank = i;
	}

	//单个段落的GR
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
	for (i=0; i<SortBufferSize; i++) {//记录GR2名
		SortBuffer[i]->GR2Rank = i;
	}

	//多个段落的GR
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
	for (i=0; i<SortBufferSize; i++) {//记录GR3排名
		SortBuffer[i]->GR3Rank = i;
	}
}




/*后期处理，剔除所抽取的关键词中独立频率过低，或termhood太小的关键词*/
int	 Extraction::PostProcess() { 
	int i, RefineBufferSize = 0;
	Unit **RefineBuffer = new Unit*[SortBufferSize];
	for (i=0; i<SortBufferSize; i++) {
		int idpFreq = SortBuffer[i]->frequency - SortBuffer[i]->nestedFreq;
		//独立频率大于阈值，termhood>1.0，奇异度>1.0
//		if ( idpFreq > INDEPENDENT_THRESHOLD && SortBuffer[i]->termhood > 1.0 && SortBuffer[i]->strange > 0.4) { 
		if ( idpFreq > INDEPENDENT_THRESHOLD && SortBuffer[i]->termhood > 1.0 ) { 
			RefineBuffer[RefineBufferSize++] = SortBuffer[i];
		}
	}
	for (i=0; i<RefineBufferSize; i++) {
		SortBuffer[i] = RefineBuffer[i];
	}
	SortBufferSize = RefineBufferSize; //剔除一些坏的关键词后重新定义SortBufferSize
	delete[] RefineBuffer;
	return RefineBufferSize;
}


/***********************************计算奇异度S************************************/
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
	for (i=0; i<SortBufferSize; i++) {//记录singularity排名
		SortBuffer[i]->sinRank = i;
	}
}

/***********************************结果存入数据库************************************/
void Extraction::StoreIntoDatabase(wchar_t view[]) { //将抽取出来的关键词存到数据库
	data.StoreIntoDatabase(SortBuffer, SortBufferSize, view);
}


//按照value从小到大排序
void Extraction::QuickSortByValue( int iBegin, int iEnd) {
	int k;
	while ( iBegin < iEnd ) {
		k = PartitionByValue(iBegin,iEnd);
		if( k-iBegin < iEnd - k) { //前部分的记录比较少，先排序
			if( ( k - iBegin )>= 2 ) QuickSortByValue(iBegin, k-1 );
			iBegin = k+1;
		}
		else {                     //后部分的记录比较少，先排序
			if( ( iEnd - k ) >=2 ) QuickSortByValue(k+1, iEnd );
			iEnd = k -1;
		}
	}
	return;
}

//按照词串的单词个数从多到少排序
void Extraction::QuickSortByLength( int iBegin, int iEnd) {
	int k;
	while ( iBegin < iEnd ) {
		k = PartitionByLength(iBegin,iEnd);
		if( k-iBegin < iEnd - k) { //前部分的记录比较少，先排序
			if( ( k - iBegin )>= 2 ) QuickSortByLength(iBegin, k-1 );
			iBegin = k+1;
		}
		else {                     //后部分的记录比较少，先排序
			if( ( iEnd - k ) >=2 ) QuickSortByLength(k+1, iEnd );
			iEnd = k -1;
		}
	}
	return;
}

//按照termhood从大到小排序
void Extraction::QuickSortByTermhood( int iBegin, int iEnd) {
	int k;
	while ( iBegin < iEnd ) {
		k = PartitionByTermhood(iBegin,iEnd);
		if( k-iBegin < iEnd - k) { //前部分的记录比较少，先排序
			if( ( k - iBegin )>= 2 ) QuickSortByTermhood(iBegin, k-1 );
			iBegin = k+1;
		}
		else {                     //后部分的记录比较少，先排序
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




