#ifndef _DATAPROCESS_H_
#define _DATAPROCESS_H_
#include "stm32f10x.h"
#include "ff.h"

int ProcessingData(char* *pFileText, char* *strData);
int ReadOnePack(char* pStrPack, int len, FIL* fp);


#endif
