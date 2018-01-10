#include "stdafx.h"
#include "Unit.h"

Unit::Unit(wchar_t *_term[], int len) {
	pTerm[0] = 0x0000;
	wordCount = len;
	memcpy((void *)pTerm, (void *)_term, len * sizeof(wchar_t *));
	frequency = 1;
	nestedFreq = 0;
	nesterCount = 0;
	termhood = 0.0;
	value = 0.0;
	next = NULL;
	strange = 0.0;


	cvalueRank = 0;
	F1Rank = 0;
	F2Rank = 0;
	F3Rank = 0;
	LR1Rank = 0;
	LR2Rank = 0;
	LR3Rank = 0;
	GR1Rank = 0;
	GR2Rank = 0;
	GR3Rank = 0;
	sinRank = 0;
}

Unit::Unit() {
	pTerm[0] = 0x0000;
	frequency = 1;
	nestedFreq = 0;
	nesterCount = 0;
	termhood = 0.0;
	value = 0.0;
	next = NULL;
	strange = 0.0;


	cvalueRank = 0;
	F1Rank = 0;
	F2Rank = 0;
	F3Rank = 0;
	LR1Rank = 0;
	LR2Rank = 0;
	LR3Rank = 0;
	GR1Rank = 0;
	GR2Rank = 0;
	GR3Rank = 0;
	sinRank = 0;
}
