#include "DataProcessFun.h"
#include "stdio.h"
#include "string.h"


static char* readLine_By0A(char* dst, char* *src)
{
	char *ret = dst;
	if (**src == '\0')
	{
		return NULL;
	}

	while((*dst++ = *((*src)++)) != '\n');
		
	*--dst = '\0';		//'\0'替换'\n'
	return ret;
}

int ProcessingData(char* *pFileText, char* *strData)
{
	int phraseCount = 0;				//指令计数，用于判断每包的指令数是否是完整6条
	char arrlActual[200] = {0};
	char* pFileTextPre;
	if (**pFileText == '\0')
	{
		return -1;
	}

	*strData[0] = '\0';					//用于strcat函数的字符串末尾识别
	memset(arrlActual, 0, 200);
	pFileTextPre = *pFileText;
	readLine_By0A(arrlActual, pFileText);
	if(arrlActual[0] != '\0')
	{
		if(strncmp(arrlActual, "$GNGGA", 6) == 0)
		{
			sprintf(arrlActual, "%s\r\n", arrlActual);
			strcat(*strData, arrlActual);
			++phraseCount;
		}
		else
		{
			*pFileText = pFileTextPre;
		}
	}

	memset(arrlActual, 0, 200);
	pFileTextPre = *pFileText;
	readLine_By0A(arrlActual, pFileText);
	if(arrlActual[0] != '\0')
	{
		if(strncmp(arrlActual, "$GNGSA", 6) == 0)
		{
			sprintf(arrlActual, "%s\r\n", arrlActual);
			strcat(*strData, arrlActual);
			++phraseCount;
		}
		else
		{
			*pFileText = pFileTextPre;
		}
	}
		
	memset(arrlActual, 0, 200);
	pFileTextPre = *pFileText;
	readLine_By0A(arrlActual, pFileText);
	if(arrlActual[0] != '\0')
	{
		if(strncmp(arrlActual, "$GNGSA", 6) == 0)
		{
			sprintf(arrlActual, "%s\r\n", arrlActual);
			strcat(*strData, arrlActual);
			++phraseCount;
		}
		else
		{
			*pFileText = pFileTextPre;
		}
	}
	
	memset(arrlActual, 0, 200);
	pFileTextPre = *pFileText;
	readLine_By0A(arrlActual, pFileText);
	if(arrlActual[0] != '\0')
	{
		if(strncmp(arrlActual, "$GNRMC", 6) == 0)
		{
			sprintf(arrlActual, "%s\r\n", arrlActual);
			strcat(*strData, arrlActual);
			++phraseCount;
		}
		else
		{
			*pFileText = pFileTextPre;
		}
	}
		
	memset(arrlActual, 0, 200);
	pFileTextPre = *pFileText;
	readLine_By0A(arrlActual, pFileText);
	if(arrlActual[0] != '\0')
	{
		if(strncmp(arrlActual, "$GNVTG", 6) == 0)
		{
			sprintf(arrlActual, "%s\r\n", arrlActual);
			strcat(*strData, arrlActual);
			++phraseCount;
		}
		else
		{
			*pFileText = pFileTextPre;
		}
	}
	
	memset(arrlActual, 0, 200);
	pFileTextPre = *pFileText;
	readLine_By0A(arrlActual, pFileText);
	if(arrlActual[0] != '\0')
	{
		if(strncmp(arrlActual, "$GNDHV", 6) == 0)
		{
			sprintf(arrlActual, "%s\r\n", arrlActual);
			strcat(*strData, arrlActual);
			++phraseCount;
		}
		else
		{
			*pFileText = pFileTextPre;
		}
	}

	if (0 == phraseCount)			//当前语句非法，需要额外处理，跳过这一行!!!!!!
	{
		memset(arrlActual, 0, 200);
		readLine_By0A(arrlActual, pFileText);
	}
	
	return phraseCount;
}

int ReadOnePack(char* pStrPack, int len, FIL* fp)
{
	int phraseCount = 0;				//指令计数，用于判断每包的指令数是否是完整6条
	char arrlActual[200] = {0};
	DWORD	dPrefptr = 0;
	if (pStrPack == '\0')
	{
		return -1;
	}
	if(f_eof(fp) == 1)
	{
		return -2;
	}
	
	memset(pStrPack, 0, len);
	memset(arrlActual, 0, 200);
	dPrefptr = f_tell(fp);
	if(f_gets(arrlActual, 200, fp))
	{
		if(strncmp(arrlActual, "$GNGGA", 6) == 0)
		{
			strcat(pStrPack, arrlActual);
			++phraseCount;
		}
		else
		{
			f_lseek(fp, dPrefptr);
		}
	}

	memset(arrlActual, 0, 200);
	dPrefptr = f_tell(fp);
	if(f_gets(arrlActual, 200, fp))
	{
		if(strncmp(arrlActual, "$GNGSA", 6) == 0)
		{
			strcat(pStrPack, arrlActual);
			++phraseCount;
		}
		else
		{
			f_lseek(fp, dPrefptr);
		}
	}
		
	memset(arrlActual, 0, 200);
	dPrefptr = f_tell(fp);
	if(f_gets(arrlActual, 200, fp))
	{
		if(strncmp(arrlActual, "$GNGSA", 6) == 0)
		{
			strcat(pStrPack, arrlActual);
			++phraseCount;
		}
		else
		{
			f_lseek(fp, dPrefptr);
		}
	}
	
	memset(arrlActual, 0, 200);
	dPrefptr = f_tell(fp);
	if(f_gets(arrlActual, 200, fp))
	{
		if(strncmp(arrlActual, "$GNRMC", 6) == 0)
		{
			strcat(pStrPack, arrlActual);
			++phraseCount;
		}
		else
		{
			f_lseek(fp, dPrefptr);
		}
	}
		
	memset(arrlActual, 0, 200);
	dPrefptr = f_tell(fp);
	if(f_gets(arrlActual, 200, fp))
	{
		if(strncmp(arrlActual, "$GNVTG", 6) == 0)
		{
			strcat(pStrPack, arrlActual);
			++phraseCount;
		}
		else
		{
			f_lseek(fp, dPrefptr);
		}
	}
	
	memset(arrlActual, 0, 200);
	dPrefptr = f_tell(fp);
	if(f_gets(arrlActual, 200, fp))
	{
		if(strncmp(arrlActual, "$GNDHV", 6) == 0)
		{
			strcat(pStrPack, arrlActual);
			++phraseCount;
		}
		else
		{
			f_lseek(fp, dPrefptr);
		}
	}

	if (0 == phraseCount)			//当前语句非法，需要额外处理，跳过这一行!!!!!!
	{
		memset(arrlActual, 0, 200);
		f_gets(arrlActual, 200, fp);
	}

	return phraseCount;
}
