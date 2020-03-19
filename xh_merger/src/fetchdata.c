#include "format.h"
/**
目的结构体的成员变量必须为字符数组类型
**/
int fetchData(char* pSrc, char* pDst,  int* pLens, const int num)
{
	int index = 0;
	char* psrc = pSrc;
	char* ptmp = pSrc;
	char* pdst = pDst;
	int* plen = pLens;
	for(;plen < pLens + num; ++plen)
	{
		ptmp += *plen;
		while(psrc < ptmp)
		{
			*pdst++ = *psrc++;
		}
		*pdst++ = '\0';
	}
	return 0;
}
