#include "stdafx.h"
#include "HashTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int HashTable::MAX_SIZE = HASH_TABLE_LEN;

HashTable::HashTable() {//���캯��
	size = 0;
	for (int i=0; i<HASH_TABLE_LEN; i++) arrUnit[i] = NULL;
	StaticBufIndex = 0;
	DynamicBuf = (Unit *)malloc(sizeof(Unit) *100);
	DynamicBufIndex = 0;
	DynamicBufMaxSize = 100;
	currentUnitIndex = -1;
} 


HashTable::~HashTable() {//�������������ɾ��arrUnit��nedw�����Ľ��
	free(DynamicBuf);
}

//��ʴ�str1�ĳ��ȣ�ָ���������Ϊlen1���Ƚ�str1��str2��ǰlen2���ʴ��Ƿ���ͬ
bool HashTable::Compare(wchar_t *str1[], int len1, wchar_t *str2[], int len2) { //ע��str1��str2����Ԫ����һ��������׵�ַ������str1��һ����������
	int i;
	wchar_t wStr1[50] = {0};
	wchar_t wStr2[50] = {0};
	for ( i = 0; i < len1; i++ ) { wcscat(wStr1, str1[i]); }//�ȸ���ÿ�������ַ����������Ĵʴ�
	for ( i = 0; i < len2; i++ ) { wcscat(wStr2, str2[i]); }
	if ( wcsncmp(wStr1, wStr2, len2) != 0 ) return false;
	else return true;
}

//keyΪ�������飬lenΪ���������������ֵΪtrueʱ��pos�д�����ҵ���Unit��ַ��������falseʱ��pos�д�ſ��Բ����µ�Ԫ�ĵ�ַ
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
		size++; //�µĽ�㣬size��һ
		return false;
	}
	else {//����Ѿ��н��ռ�ô˴���
		Unit *p, *q;
		p = arrUnit[hash]; 
		q = p;
		do{ //ѭ���Ƚ��Ƿ�ôʴ��Ѵ���
			if (Compare(p->pTerm, p->wordCount, key, len)) { 
				*pos = p;
				return true; //�ҵ��ôʴ�
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


//��ϣ����,strΪ�������飬lenΪ����
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
	//ջ�����л�����ʹ�ù̶����ڴ���Դ
	if (StaticBufIndex < STATIC_BUF_LEN) return &StaticBuf[StaticBufIndex++];
	else { //�����Ƿ�̬�����ڴ�
		if (DynamicBufIndex < DynamicBufMaxSize) {
			return &DynamicBuf[DynamicBufIndex++];
		}
		else {
			DynamicBuf = (Unit *)realloc(DynamicBuf, DynamicBufMaxSize+2048); //ÿ���ۼ�128
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
