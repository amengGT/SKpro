#include "format.h"
#define BK_BC	"01042900"
#define BK_ICBC	"01022900"
int transAcct(struct acct_inf* acct, struct acctinfo* vpt_acct);
int main(int argc, char *argv[])
{
	/** store the record **/
	int  result = 0;
	int lrec = 0;
	char fname[128] = {0};
	char sDate[8+1] = {0};
    char buff[MAXSIZE] = {0};	
		
	FILE *fpa_bc_err = NULL;
	FILE *fpa_bc = NULL;
	FILE *fpa_icbc = NULL;
	FILE *fp_log = NULL;
	
	struct acctinfo ainfo;
	struct acct_inf acct;
	memset(&ainfo,0x00,sizeof(ainfo));
	memset(&acct,0x00,sizeof(acct));
	
	/*char time[128+1] = {0};
	getDateTime(time);*/
	getSysDate(sDate);

	sprintf(fname, "%s/log/acct_result_%.8s.log", getenv("MERGE_HOME"), BK_BC);
    fp_log = fopen(fname, "w");
	if(fp_log == NULL) {
		fprintf(stderr, "Open Log file[%s] err!\n", fname);
		return FAIL;
	}
	
	sprintf(fname, "%s/outfile/ACCT-%.8s-MERGEREQ-%.8s-ERR", getenv("MERGE_HOME"), BK_BC, sDate);
	fpa_bc_err = fopen(fname, "a");
	if(fpa_bc_err == NULL) {
		fprintf(fp_log, "open acct-bc-err out file for write error[%s]\n", fname);
		return FAIL;
	}

	sprintf(fname, "%s/outfile/ACCT-%.8s-MERGEREQ-%.8s", getenv("MERGE_HOME"), BK_BC, sDate);
	fpa_bc = fopen(fname, "w");
	if(fpa_bc == NULL) {
		fprintf(fp_log, "open acct-bc out file for write error[%s]\n", fname);
		return FAIL;
	}
	
	sprintf(fname, "%s/outfile/ACCT-%.8s-MERGEREQ-%.8s", getenv("MERGE_HOME"), BK_ICBC, sDate);
	fpa_icbc = fopen(fname, "w");
	if(fpa_icbc == NULL) {
		fprintf(fp_log, "open acct-icbc out file for write error[%s]\n", fname);
		return FAIL;
	}
	/** ���ݿ����� **/
    result = DbConnect();
	if(result) {
		fprintf(fp_log, "Db connect Failed[%d]!\n", result);
		return FAIL;
	}

    /**��ʱ�˻����˻��α����**/
	/*�����α�*/
    result=tblMergereq(DB_CUR, &ainfo);
    if(result) 
    {
		fprintf(fp_log, "define cursor for tblMergereq error[%d]\n", result);
		return FAIL;
	}

    /*���α�*/
    result=tblMergereq(DB_OPEN, &ainfo);
    if(result) 
    {
		fprintf(fp_log, "open cursor for tblMergereq error[%d]\n", result);
		return FAIL;
	}

	/*��ѯ��ʱ��*/  
    while(1) 
    {
		++lrec;
        memset(&ainfo, 0x00, sizeof(ainfo));
		memset(&acct, 0x00, sizeof(acct));
        result = tblMergereq(DB_FETCH, &ainfo);
        if(result!=0 && result!=SQLNOTFOUND && result!=SQLPARTFOUND )
        {
            fprintf(stdout, "fetch %dth record from tblMergereq error[%d]\n", lrec, result);
			break;
        }
        else if(result ==SQLNOTFOUND )
        {
            fprintf(stdout, "fetch record end. Total:%d\n", --lrec);
			break;
        }else if(result==SQLPARTFOUND)
		{
			fprintf(stdout, "fetched column value is NULL:%d\n", lrec);
			break;
		}

		/*fprintf(stdout, "++[%s]++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n", time);*/
		#ifndef xxx
		/**>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>**/
		if('E' == ainfo.ErrCode[0])
		{/*��ӡ���˻���Ϣ����ʧ��Ӧ���ļ�ACCT-$$$$$$$$-MERGEREQ-YYYYMMDD-ERR(����)*/
			fprintf(fpa_bc_err, "%.8s%08d%.2s%c%.2s%.20s%.30s%c%.11s%.8s%012d%.14s%.11s%.100s%.200s\n",
				ainfo.Bank, ainfo.LineNum, ainfo.ErrCode, ainfo.Flag, 
				ainfo.IdType, ainfo.IdNo, ainfo.Name, ainfo.Gender, ainfo.HospId, ainfo.BankId, 
				ainfo.Balance, ainfo.Stime, ainfo.TelNo, ainfo.Addr, ainfo.TxnMsg);
		}
		else if(ainfo.ErrCode[0] == ' ')
		{
			memcpy(acct.IdNo, ainfo.IdNo, sizeof(acct.IdNo) - 1);
			result=tblAcct(DB_SELECT, &acct);
			if(0 != result && SQLNOTFOUND != result && result!=SQLPARTFOUND)
			{
				fprintf(stdout, "DB_SELECT record from tblAcct error[%d]\n", result);
				break;
			}else if(result == SQLPARTFOUND)
			{
				fprintf(stdout, "tblAcct(DB_SELECT, &acct):fetched column value is NULL:%d\n", lrec);
				break;
			}
			else if(result == SQLNOTFOUND )/*��������δ�ҵ����˻�*/
			{
				/*��ӡ���˻���Ϣ�������ļ�ACCT-$$$$$$$$-MERGERRSP-YYYYMMDD(����)*/
				memset(buff, 0, MAXSIZE);
				sprintf(buff, "%012d", ainfo.Balance);
				fprintf(fpa_icbc, "%.2s%.20s%.30s%c%.11s%.8s%012d%.14s%.11s%.100s%.200s\n",
					ainfo.IdType, ainfo.IdNo, ainfo.Name, ainfo.Gender, ainfo.HospId, ainfo.BankId, 
					buff, ainfo.Stime, ainfo.TelNo, ainfo.Addr, ainfo.TxnMsg);
				
				/*�������������˻���¼*/
				transAcct(&acct, &ainfo);/*ainfo --> acct;*/
				memcpy(acct.BankId, BK_ICBC, sizeof(acct.BankId));/*********************************************************/
				result = tblAcct(DB_INSERT, &acct);
				if(0 != result)
				{
					fprintf(stdout, "===tblAcct(DB_INSERT, &acct)===ERROR[%d]=========\n",result);
					break;
				}
				/*���A3*/
				memcpy(ainfo.ErrCode, "A3", sizeof(ainfo.ErrCode)-1);
				result = tblMergereq(DB_UPDATE, &ainfo);
				if(result!=0)  
				{
					fprintf(stdout, "===tblMergereq(DB_UPDATE, &ainfo);===ERROR[%d]=========\n",result);
					break;
				}
			}
			else{/*���������ҵ����˻�*/
				/*��ӡ���˻���Ϣ�������ļ�ACCT-$$$$$$$$-MERGERRSP-YYYYMMDD(����)*/
				memset(buff, 0, MAXSIZE);
				sprintf(buff, "%012d", ainfo.Balance);
				fprintf(fpa_bc, "%.2s%.20s%.30s%c%.11s%.8s%012d%.14s%.11s%.100s%.200s\n",
					ainfo.IdType, ainfo.IdNo, ainfo.Name, ainfo.Gender, ainfo.HospId, ainfo.BankId, 
					buff, ainfo.Stime, ainfo.TelNo, ainfo.Addr, ainfo.TxnMsg);
				if(0 == memcmp(acct.BankId, BK_ICBC, sizeof(acct.BankId)))
				{
					memcpy(ainfo.ErrCode, "A1", sizeof(ainfo.ErrCode)-1);/*���A1*/
				}else{
					memcpy(ainfo.ErrCode, "A2", sizeof(ainfo.ErrCode)-1);/*���A2*/
				}
				result = tblMergereq(DB_UPDATE, &ainfo);
				if(result!=0)  
				{
					fprintf(stdout, "===tblMergereq(DB_UPDATE, &ainfo);===ERROR[%d]=========\n",result);
					break;
				}
			}
		
		}
		#endif
		if(0 == lrec%300)
		{
			if(0 != (result = DbCommit()))
				fprintf(stdout, "DbCommit Failed[%d] record[%d]!\n", result,lrec);
			else
				fprintf(stdout, "DbCommit Success[%d] record[%d]!\n", result,lrec);
        }
		/**<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<**/	
	}
	if(0 != lrec%300)
	{
		if(0 != (result = DbCommit()))
			fprintf(stdout, "DbCommit Error[%d] record[%d]!\n", result,lrec);
		else
			fprintf(stdout, "DbCommit record[%d]!\n", lrec);
    }

	/*�ر��α�*/
	result = tblMergereq(DB_CLOSE, &ainfo);
    if(result!=0 && result!=SQLPARTFOUND )  
    {
        fprintf(fp_log, "close tblMergereq erro[%d]\n",result);
    }
   
    /* �ر����ݿ⡢�ļ� */
    result = DbClose();
	if(result) 
    {
		fprintf(fp_log, "DbClose Failed[%d]!\n", result);
	}  
  
	/*�ر��ļ�ָ��*/
	fclose(fpa_bc_err);
	fclose(fpa_bc);
	fclose(fpa_icbc);
	fclose(fp_log);
	fprintf(stderr, "close file\n"); 
	
	return 0;
}

int transAcct(struct acct_inf* acct, struct acctinfo* vpt_acct)
{
	memcpy(acct->IdType, vpt_acct->IdType, sizeof(acct->IdType));
	memcpy(acct->IdNo , vpt_acct->IdNo, sizeof(acct->IdNo));
	memcpy(acct->BankId, vpt_acct->BankId, sizeof(acct->BankId));
	memcpy(acct->unitno, vpt_acct->HospId, sizeof(acct->unitno));
	memcpy(acct->name, vpt_acct->Name, sizeof(acct->name));
	acct->gender[0] = vpt_acct->Gender;
	memcpy(acct->telno, vpt_acct->TelNo, sizeof(acct->telno));
	memcpy(acct->addr, vpt_acct->Addr, sizeof(acct->addr));
	memcpy(acct->open_date, vpt_acct->Stime, sizeof(acct->open_date));
	/*memcpy(acct->last_mod_date, "00000000", sizeof(acct->last_mod_date));*/
    acct->state[0] = 'A';
	return SUCCESS;
}