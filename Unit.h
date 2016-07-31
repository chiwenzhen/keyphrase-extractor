/*��Ĺ��ܣ���ʾ����ĵ�Ԫ���õ�Ԫ������һ������ĸ�������ֵ����Щ��Ԫ�����ϣ���й����ٶ�ȡ*/

#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <string.h>
using namespace std;
#define MAX_WORD_NUM 6   //���������6������

//һ�����������������Ϣ�������Ƶ�ʣ����һ��Unit��Ԫ��ע���ڱ������дʴ�������ִ�������˵������һ���ġ�
class Unit {
public:
	wchar_t	*pTerm[MAX_WORD_NUM+1]; //ָ�����飬ÿ��ָ��ָ��pView�е�һ��λ�ã�������Щָ��ָ����ַ�������������term
	int		wordCount; //�ʴ��д���ĸ���
	int		charCount; //�ʴ����ֵĸ���
	int		frequency; //�ôʴ����ֵ���Ƶ��
	int		nestedFreq; //��Ƕ���������ʴ��е�Ƶ�� ���� frequency - nestedFreq = �ʴ��������ֵĸ���
	int		nesterCount; //�����ôʴ��ĸ����ʴ��ĸ���
	double	termhood; //�ʴ�����Ϊ�ؼ��ʵĶ���ָ��
	double	value; //���ڴ�Ÿ��ּ�����
	double	strange;
	Unit	*next; //��ϣ��������������һ��Unit

	int	uMultiSentenceLoneM;	//���¶��ظ���
	int	uParagraphLoneM;		//���ι¶��ظ���
	int	uMultiParagraphLoneM;	//��ι¶��ظ���

	int	uMultiStClusterStM;		//���ۺϴصľ�����
	int	uParaClusterParaM;		//���ξۺϴصĶ�����
	int	uMultiParaClusterParaM;	//��ξۺϴصĶ�����

	bool			bMultiSentenceNowNotLone;
	bool			bParagraphNowNotLone;
	bool			bMultiParagraphNowNotLone;

	int	uLastSentenceNo;		//���ִ���һ�γ���ʱ�ľ��Ӻ�
	int	uLastParagraphNo;		//���ִ���һ�γ���ʱ�Ķκ�

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