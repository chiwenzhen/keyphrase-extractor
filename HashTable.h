/*��Ĺ��ܣ��������ٵĶ�д����ȡ�ĺ�ѡ�ؼ��ʵ�������Ϣ*/

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "Unit.h"
#define HASH_TABLE_LEN 8192
#define STATIC_BUF_LEN 24576
class HashTable{
private:
	int Hash(wchar_t *str[], int len);
	Unit *arrUnit[HASH_TABLE_LEN]; //��Ž��
	int size; //��ϣ��ʵ�ʴ�С������ͬ�Ĵʴ�����
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
	bool Find(wchar_t *key[], int len, Unit **pos);//���ݵ�ַ�Ƚ��ַ���
	int Size() { return size; }; //���ع�ϣ��ǰ��С
	bool Compare(wchar_t *str1[], int len1, wchar_t *str2[], int len2); //�Ƚ�str1��str2�Ƿ���ͬ
	void Clear(); //���ù�ϣ��
	void GetAll(Unit **buf);
	void MoveFirst() { currentUnitIndex = -1; } //���������Ƶ���ͷ
	Unit *GetNext();
};
#endif
