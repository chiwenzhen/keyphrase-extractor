/*类的功能：用来快速的读写所抽取的候选关键词的所有信息*/

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "Unit.h"
#define HASH_TABLE_LEN 8192
#define STATIC_BUF_LEN 24576
class HashTable{
private:
	int Hash(wchar_t *str[], int len);
	Unit *arrUnit[HASH_TABLE_LEN]; //存放结点
	int size; //哈希表实际大小，即不同的词串个数
	Unit StaticBuf[STATIC_BUF_LEN];
	int StaticBufIndex;
	Unit *DynamicBuf;
	int DynamicBufIndex;
	Unit *GetAvailableUnit();
	int DynamicBufMaxSize;
	int currentUnitIndex;
public:
	HashTable();
	~HashTable();
	static int MAX_SIZE;
	bool Find(wchar_t *key[], int len, Unit **pos);//根据地址比较字符串
	int Size() { return size; }; //返回哈希表当前大小
	bool Compare(wchar_t *str1[], int len1, wchar_t *str2[], int len2); //比较str1和str2是否相同
	void Clear(); //重置哈希表
	void GetAll(Unit **buf);
	void MoveFirst() { currentUnitIndex = -1; } //将迭代器移到开头
	Unit *GetNext();
};
#endif
