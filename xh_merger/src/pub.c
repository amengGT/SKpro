#include "format.h"
#include <ctype.h>
#include <sys/times.h>
#include <time.h>
		
char *LTrim(char *str) {
	
	return str;
}

char *RTrim(char *str) {
	if(str == NULL || str[0] == '\0') {
		return str;
	}
	int len = strlen(str);
	int i;
	for(i=len-1; i >= 0; --i)
	{
		if(((str[i]!=' ') && (str[i]!='\t')))
		{
			str[i + 1] = '\0';
			break;
		}else if(0 == i)
		{
			str[0]='\0';
		}
	}
	return str;
}

int isDigit(char *str, int len)
{
	int i;
	for(i=0;i<len;i++) {
		if(!isdigit(str[i]))
			return FAIL;
	}
	return SUCCESS;
}

char *getSysDate( char *sDateStr) {
	time_t tm;
	struct tm *t_loc;

	time(&tm);
	strftime( sDateStr, 5, "%Y", localtime(&tm));
	strftime( sDateStr+4, 5, "%m%d", localtime(&tm));
	return sDateStr;
}

int getSystime(char *buftime)
{
    time_t  ttime;
    struct tm  *timenow; 
    time(&ttime);
    timenow = localtime(&ttime);
    sprintf(buftime,"%02d:%02d:%02d",timenow->tm_hour,timenow->tm_min,timenow->tm_sec);
    
    return 0;
}

int getDateTime(char *dt)
{
	char sDate[8+1] = {0};
	time_t tm;
    struct tm  *timenow; 
	time(&tm);
	timenow = localtime(&tm);
	strftime( sDate, 5, "%Y", timenow);
	strftime( sDate+4, 5, "%m%d", timenow);
    sprintf(dt,"%s%02d%02d%02d",sDate,timenow->tm_hour,timenow->tm_min,timenow->tm_sec);
	
	return 0;
}

