#include "stdafx.h"
#include "HashTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int HashTable::MAX_SIZE = HASH_TABLE_LEN;

HashTable::HashTable() {//构造函数
	size = 0;
	for (int i=0; i<HASH_TABLE_LEN; i++) arrUnit[i] = NULL;
	StaticBufIndex = 0;
	DynamicBuf = (Unit *)malloc(sizeof(Unit) *100);
	DynamicBufIndex = 0;
	DynamicBufMaxSize = 100;
	currentUnitIndex = -1;
} 


HashTable::~HashTable() {//析构函数，逐个删除arrUnit中nedw出来的结点
	free(DynamicBuf);
}

//设词串str1的长度（指词语个数）为len1，比较str1和str2的前len2个词串是否相同
bool HashTable::Compare(wchar_t *str1[], int len1, wchar_t *str2[], int len2) { //注：str1和str2数组元素是一个词语的首地址，所以str1是一个词语数组
	int i;
	wchar_t wStr1[50] = {0};
	wchar_t wStr2[50] = {0};
	for ( i = 0; i < len1; i++ ) { wcscat(wStr1, str1[i]); }//先根据每个词语地址构造出完整的词串
	for ( i = 0; i < len2; i++ ) { wcscat(wStr2, str2[i]); }
	if ( wcsncmp(wStr1, wStr2, len2) != 0 ) return false;
	else return true;
}

//key为词语数组，len为词语个数；当返回值为true时，pos中存放所找到的Unit地址；当返回false时，pos中存放可以插入新单元的地址
bool HashTable::Find(wchar_t *key[], int len, Unit **pos) {
	int i, charCount = 0;
	Unit *newUnit;
	int hash = Hash(key, len);
	if ( arrUnit[hash] == NULL ) {
		newUnit = GetAvailableUnit();
		memcpy((void *)(newUnit->pTerm), (void *)key, len * sizeof(wchar_t *));		
		newUnit->wordCount		= len;
		for (i=0; i<len; i++) {
			charCount = charCount + wcslen(key[i]);
		}

		newUnit->charCount = charCount;
		newUnit->frequency		= 1;
		newUnit->nestedFreq		= 0;
		newUnit->nesterCount	= 0;
		newUnit->termhood		= 0.0;
		newUnit->value			= 0.0;
		newUnit->strange		= 0.0;
		newUnit->next			= NULL;
		*pos = newUnit;
		arrUnit[hash] = newUnit;
		size++; //新的结点，size加一
		return false;
	}
	else {//如果已经有结点占用此处了
		Unit *p, *q;
		p = arrUnit[hash]; 
		q = p;
		do{ //循环比较是否该词串已存在
			if (Compare(p->pTerm, p->wordCount, key, len)) { 
				*pos = p;
				return true; //找到该词串
			}
			else { q = p; p = p->next; }
		}while(p != NULL);

		newUnit = GetAvailableUnit();
		memcpy((void *)(newUnit->pTerm), (void *)key, len * sizeof(wchar_t *));		
		newUnit->wordCount		= len;
		newUnit->frequency		= 1;
		newUnit->nestedFreq		= 0;
		newUnit->nesterCount	= 0;
		newUnit->termhood		= 0.0;
		newUnit->value			= 0.0;
		newUnit->strange		= 0.0;
		newUnit->next			= NULL;
		*pos = newUnit;
		q->next = newUnit;
		size++;
		return false;
	}
}


//哈希函数,str为词语数组，len为长度
int HashTable::Hash(wchar_t *str[], int len) {
	wchar_t *p;
	int seed = 33;
	int hash = 0;
	for ( int i=0; i<len; i++ ){
		p = str[i];
		while (*p != 0x0000) {
			hash = hash * seed + *p++;
		}
	}
	return ( (hash&0x7fffffff) % HASH_TABLE_LEN);
}


Unit *HashTable::GetAvailableUnit() {
	//栈缓存中还有则使用固定的内存资源
	if (StaticBufIndex < STATIC_BUF_LEN) return &StaticBuf[StaticBufIndex++];
	else { //否则是否动态申请内存
		if (DynamicBufIndex < DynamicBufMaxSize) {
			return &DynamicBuf[DynamicBufIndex++];
		}
		else {
			DynamicBuf = (Unit *)realloc(DynamicBuf, DynamicBufMaxSize+2048); //每次累加128
			DynamicBufMaxSize += 2048;
			return &DynamicBuf[DynamicBufIndex++];
		}
	}
}

void HashTable::Clear() {
	size=0; 
	free(DynamicBuf);
	for (int i=0; i<HASH_TABLE_LEN; i++) arrUnit[i] = NULL;
	StaticBufIndex = 0;
	DynamicBuf = (Unit *)malloc(sizeof(Unit) *100);
	DynamicBufIndex = 0;
	DynamicBufMaxSize = 100;
	currentUnitIndex = -1;
}

void HashTable::GetAll(Unit **buf) {
	int i;
	for (i=0; i<StaticBufIndex; i++) {
		*buf++ = &StaticBuf[i];
	}
	for (i=0; i<DynamicBufIndex; i++) {
		*buf++ = &DynamicBuf[i];
	}
}

Unit * HashTable::GetNext() {
	currentUnitIndex++;
	if (currentUnitIndex < size) {
		if (currentUnitIndex < STATIC_BUF_LEN) return &StaticBuf[currentUnitIndex];
		else return &DynamicBuf[currentUnitIndex - STATIC_BUF_LEN];
	}
	else { currentUnitIndex--; return NULL; }
}
