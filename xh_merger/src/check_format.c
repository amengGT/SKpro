#include "format.h"

#define BK_BC	"01042900"
#define BK_ICBC	"01022900"
#define HOSP_ID	"42502659900"
#define __log(out, fmt, ...) \
do{\
	fprintf(out, fmt, ##__VA_ARGS__ );\
	fprintf(stdout, "[%s][%d][%s]:", __FILE__,__LINE__, __FUNCTION__);\
	fprintf(stdout, fmt, ##__VA_ARGS__ );\
}while(0)
char bankno[8+1] = {0};
int check_acct_format(struct facctinf * acct_inf, char* errcode);
int check_rel_format(struct frelinf * rel_inf, char* errcode);

int main(int argc, char *argv[])
{
	/** store the record **/
	char buff[MAXSIZE];
	char fname[128];
	char sDate[8+1];
	char errc[2+1];
	char cflag;

	FILE *fp_in_a;
	FILE *fp_in_r;
	FILE *fp_out_a;
	FILE *fp_out_r;
	FILE *fp_err_a;
	FILE *fp_err_r;
	FILE *fp_log_a;
	FILE *fp_log_r;
	/** the ith record read **/
	int trec;
	int isok;
	int iRet;

	struct facctinf acct_inf;
	struct frelinf rel_inf;

	memset(&acct_inf,0x00,sizeof(acct_inf));
	memset(&rel_inf,0x00,sizeof(rel_inf));
	memset(bankno,0x00,sizeof(bankno));
	memset(sDate,0x00,sizeof(sDate));
	memset(errc,0x20,sizeof(errc));
	cflag = ' ';

	if(argc != 3) {
		fprintf(stderr, "Usage: %s <acct_file> <rel_file>\n", argv[0]);
		return FAIL;
	}
	memcpy(bankno, argv[1] + 5, sizeof(bankno)-1);
	getSysDate(sDate);

	/*** Open file for read and write **/
	sprintf(fname, "%s/log/acct_check_%s.log", getenv("MERGE_HOME"), bankno);
	fp_log_a = fopen(fname, "w");
	if(fp_log_a == NULL) {
		fprintf(stderr, "open acct err file for write error[%s]\n", fname);
		return FAIL;
	}

	sprintf(fname, "%s/log/rel_check_%s.log", getenv("MERGE_HOME"), bankno);
	fp_log_r = fopen(fname, "w");
	if(fp_log_r == NULL) {
		fprintf(stderr, "open rel err file for write error[%s]\n", fname);
		return FAIL;
	}

	sprintf(fname, "%s/rawfile/%s", getenv("MERGE_HOME"), argv[1]);
	fp_in_a = fopen(fname, "r");
	if(fp_in_a == NULL) {
		__log(fp_log_a, "open acct file for read error[%s]\n", fname);
		return FAIL;
	}

	sprintf(fname, "%s/rawfile/%s", getenv("MERGE_HOME"), argv[2]);
	fp_in_r = fopen(fname, "r");
	if(fp_in_r == NULL) {
		__log(fp_log_r, "open rel file for read error[%s]\n", fname);
		return FAIL;
	}

	sprintf(fname, "%s/prefile/ACCT-%s-MERGEREQ-%.8s-PRELOAD.dat", getenv("MERGE_HOME"), bankno, sDate);
	fp_out_a = fopen(fname, "w");
	if(fp_out_a == NULL) {
		__log(fp_log_a, "open acct inf out file for write error[%s]\n", fname);
		return FAIL;
	}

	sprintf(fname, "%s/prefile/ACCT-%s-MERGERELAREQ-%.8s-PRELOAD.dat", getenv("MERGE_HOME"), bankno, sDate);
	fp_out_r = fopen(fname, "w");
	if(fp_out_r == NULL) {
		__log(fp_log_r, "open rel inf out file for write error[%s]\n", fname);
		return FAIL;
	}

	sprintf(fname, "%s/outfile/ACCT-%s-MERGEREQ-%.8s-ERR", getenv("MERGE_HOME"), bankno, sDate);
	fp_err_a = fopen(fname, "w");
	if(fp_err_a == NULL) {
		__log(fp_log_a, "open acct err file for write error[%s]\n", fname);
		return FAIL;
	}
	
	sprintf(fname, "%s/outfile/ACCT-%s-MERGERELAREQ-%.8s-ERR", getenv("MERGE_HOME"), bankno, sDate);
	fp_err_r = fopen(fname, "w");
	if(fp_err_r == NULL) {
		__log(fp_log_r, "open rel err file for write error[%s]\n", fname);
		return FAIL;
	}
	
	/**  file handles associated success   **/
	fprintf(stdout, "processing [%s] file checking begin\n", bankno);

	/** process acct inf file **/
	trec = isok = 0;
	fprintf(fp_log_a, "process acct file check for [%s] begin\n", bankno);
	while(!feof(fp_in_a))
	{
		memset(buff,0x00,MAXSIZE);
		memset(errc,0x20,sizeof(errc));
		memset(&acct_inf,0x00,sizeof(acct_inf));
		if((fgets(buff,MAXSIZE,fp_in_a) == NULL) || (buff[0] == '\n')) 
		{
			continue;/**break;**/
		}
		++trec;
		/** trim tail \n **/
		if(buff[strlen(buff)-1] == '\n')
			buff[strlen(buff)-1] = '\0';
		if(strlen(buff) != sizeof(struct facctinf)-11) 
		{
			fprintf(fp_err_a, "%.8s%08d%.2s%c%s\n", bankno,trec,"E0",cflag,buff);
			__log(fp_log_a, "[ACCT-ERR:] ErrLine: %08d, ErrCode: %s]\n", trec, "E0");
			continue;
		}
		
		int mLens[11] = {2, 20, 30, 1, 11, 8, 12, 14, 11, 100, 200};
		iRet = fetchData(buff, (char*)&acct_inf, mLens, 11);
		if(iRet != SUCCESS) {
			__log(fp_log_a, "[ACCT-ERR:] ErrLine: %08d, Err: fetch error]\n", trec);
			continue;
		}
		/*** check procedure ***/
		iRet = check_acct_format(&acct_inf, errc);
		if(iRet) 
		{
			fprintf(fp_err_a, "%.8s%08d%.2s%c%s\n", bankno,trec,errc,cflag,buff);
			__log(fp_log_a, "[ACCT-ERR:] ErrLine: %08d, ErrCode: %s]\n", trec, errc);
			continue;
		}
		++isok;
		fprintf(fp_out_a, "%.8s%08d%.2s%c%s\n", bankno,trec,errc,cflag,buff);
	}
	/** all acct check result **/
	__log(fp_log_a, "ACCT TOTAL REC: %d, SUCCESS: %d, ERROR: %d\n",trec, isok, trec - isok);
	
	/***********************************************************************************************/
	/** process rel inf file **/
	trec = isok = 0;
	fprintf(fp_log_r, "process rel file check for [%s] begin\n", bankno);
	while(!feof(fp_in_r))
	{
		memset(buff,0x00,MAXSIZE);
		memset(errc,0x20,sizeof(errc));
		memset(&rel_inf,0x00,sizeof(rel_inf));
		if((fgets(buff,MAXSIZE,fp_in_r) == NULL) || (buff[0] == '\n')) 
		{
			continue;/**break;**/
		}
		++trec;
		/** trim tail \n **/
		if(buff[strlen(buff)-1] == '\n')
			buff[strlen(buff)-1] = '\0';
		if(strlen(buff) != sizeof(struct frelinf)-4) 
		{
			fprintf(fp_err_r, "%.8s%08d%.2s%c%s\n", bankno,trec,"E0",cflag,buff);
			__log(fp_log_r, "[REL-ERR:] ErrLine: %08d, ErrCode: %s]\n", trec, "E0");
			continue;
		}

		int mLens[4] = {8,2,20,30};
		iRet = fetchData(buff, (char*)&rel_inf, mLens, 4);
		if(iRet != SUCCESS) {
			__log(fp_log_r, "[REL-ERR:] ErrLine: %08d, Err: fetch error]\n", trec);
			continue;
		}
		/*** check procedure **/
		iRet = check_rel_format(&rel_inf, errc);
		if(0 != iRet) 
		{
			fprintf(fp_err_r, "%.8s%08d%.2s%c%s\n", bankno,trec,errc,cflag,buff);
			__log(fp_log_r, "[REL-ERR:] ErrLine: %08d, ErrCode: %s]\n", trec, errc);
			continue;
		}
		++isok;
		fprintf(fp_out_r, "%.8s%08d%.2s%c%s\n", bankno,trec,errc,cflag,buff);
	}	
	/** all rela check result **/
	__log(fp_log_r, "REL TOTAL REC: %d, SUCCESS: %d, ERROR: %d\n",trec, isok, trec - isok);

	fclose(fp_in_a);
	fclose(fp_in_r);
	fclose(fp_out_a);
	fclose(fp_out_r);
	fclose(fp_err_a);
	fclose(fp_err_r);
	fclose(fp_log_a);
	fclose(fp_log_r);
	
	return 0;
}

int openFile(	
	FILE **fp_in_a,
	FILE **fp_in_r,
	FILE **fp_out_a,
	FILE **fp_out_r,
	FILE **fp_err_a,
	FILE **fp_err_r,
	FILE **fp_log_a,
	FILE **fp_log_r)
{
	return 0;
}
