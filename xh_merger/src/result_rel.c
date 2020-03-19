#include "format.h"
#define BK_BC	"01042900"
#define BK_ICBC	"01022900"
int main(int argc, char *argv[])
{
	/** store the record **/
	int  result = 0;
	int lrec = 0;
	char fname[128] = {0};
	char sDate[8+1] = {0};
    /* char time[128+1] = {0}; */
    char buff[MAXSIZE] = {0};

	FILE *fpr_bc_err = NULL;
	FILE *fpr_icbc = NULL;
	FILE *fp_log = NULL;
	
	struct relinfo rinfo;
	memset(&rinfo,0x00,sizeof(rinfo));
	struct acctinfo ainfo;
	memset(&ainfo,0x00,sizeof(ainfo));
	struct card_join_acct rela;
	memset(&rela,0x00,sizeof(rela));
	
	/* getSystime(time); */
	getSysDate(sDate);
	
	sprintf(fname, "%s/log/rel_result_%.8s.log", getenv("MERGE_HOME"), BK_BC);
    fp_log = fopen(fname, "w");
	if(fp_log == NULL) {
		fprintf(stderr, "Open Log file[%s] err!\n", fname);
		return FAIL;
	}
	
	sprintf(fname, "%s/outfile/ACCT-%s-MERGERELAREQ-%.8s-ERR", getenv("MERGE_HOME"), BK_BC, sDate);
	fpr_bc_err = fopen(fname, "a");
	if(fpr_bc_err == NULL) {
		fprintf(stdout, "open rel-bc out file for write error[%s]\n", fname);
		return FAIL;
	}
	
	sprintf(fname, "%s/outfile/ACCT-%s-MERGERELAREQ-%.8s", getenv("MERGE_HOME"), BK_ICBC, sDate);
	fpr_icbc = fopen(fname, "w");
	if(fpr_icbc == NULL) {
		fprintf(stdout, "open rel-icbc out file for write error[%s]\n", fname);
		return FAIL;
	}
	/** 数据库连接 **/
    result = DbConnect();
	if(result) {
		fprintf(stdout, "Db connect Failed[%d]!\n", result);
		return FAIL;
	}

    /**临时账户表账户游标操作**/
	/* 声明游标 */
    result = tblMergerelareq(DB_CUR, &rinfo);
    if(0 != result) 
    {
		fprintf(stdout, "define cursor for  tblMergerelareq error[%d]\n", result);
		return FAIL;
	}
    /* 打开游标 */
    result=tblMergerelareq(DB_OPEN, &rinfo);
    if(0 != result) 
    {
		fprintf(stdout, "open cursor for  tblMergerelareq error[%d]\n", result);
		return FAIL;
	}
    
	/* 查询临时表   */
	lrec = 0;
    while(1) 
    {
		++lrec;
        memset(&rinfo,0x00,sizeof(rinfo));
		memset(&ainfo,0x00,sizeof(ainfo));
		memset(&rela,0x00,sizeof(rela));
        result = tblMergerelareq(DB_FETCH, &rinfo);
        if(result!=0 && result!=SQLNOTFOUND && result!=SQLPARTFOUND )
        {
            fprintf(stdout, "tblMergerelareq(DB_FETCH, &rinfo):fetch %dth record error[%d]\n", lrec, result);
			break;
        }
        else if(result ==SQLNOTFOUND )
        {
            fprintf(stdout, "tblMergerelareq(DB_FETCH, &rinfo):fetch record end. Toal:%d\n", --lrec);
			break;
        }else if(result==SQLPARTFOUND)
		{
			fprintf(stdout, "tblMergerelareq(DB_FETCH, &rinfo):fetched column value is NULL:%d\n", lrec);
			break;
		}
		/*fprintf(stdout, "%.8s%08d%.2s%c%.8s%.2s%.20s%.30s\n",
				rinfo.Bank, rinfo.LineNum, rinfo.ErrCode, rinfo.Flag, 
				rinfo.IssBank, rinfo.IdType, rinfo.IdNo, rinfo.HisCard);*/
		/**>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>**/
		if('E' == rinfo.ErrCode[0])
		{/*打印到关联关系处理失败应答文ACCT-$$$$$$$$-MERGERELAREQ-YYYYMMDD-ERR(中行)*/
			fprintf(fpr_bc_err, "%.8s%08d%.2s%c%.8s%.2s%.20s%.30s\n",
				rinfo.Bank, rinfo.LineNum, rinfo.ErrCode, rinfo.Flag, 
				rinfo.IssBank, rinfo.IdType, rinfo.IdNo, rinfo.HisCard);
		}
		
		else if(' ' == rinfo.ErrCode[0])
		{	
			memcpy(ainfo.IdNo, rinfo.IdNo, sizeof(ainfo.IdNo));
			result=tblMergereq(DB_SELECT, &ainfo); 
			if(result!=0 && result!=SQLNOTFOUND && result!=SQLPARTFOUND )
			{
				fprintf(stdout, "===tblMergereq(DB_SELECT, &ainfo):select %dth record error[%d]\n", lrec, result);
				break;
			}
			else if(result ==SQLNOTFOUND )
			{
				fprintf(stdout, "===tblMergereq(DB_SELECT, &ainfo):select record end.Toal:%d\n", --lrec);
				break;
			}else if(result==SQLPARTFOUND)
			{
				fprintf(stdout, "===tblMergereq(DB_SELECT, &ainfo):select column value is NULL:%d\n", lrec);
				break;
			}
			
			if(0 == memcmp(ainfo.ErrCode, "A2", sizeof(ainfo.ErrCode)))
			{/*该中账户对应的该关联关系不处理*/
				continue;
			}
			
			if(0 == memcmp(ainfo.ErrCode, "A3", sizeof(ainfo.ErrCode)))
			{/*打印到关联关系处理结果文件ACCT-$$$$$$$$-MERGERELARSP-YYYYMMDD(工行)*/
				fprintf(fpr_icbc, "%.8s%.2s%.20s%.30s\n",
					BK_ICBC, rinfo.IdType, rinfo.IdNo, rinfo.HisCard);
			}
			#ifndef XXXXXX
			memcpy(rela.HisCard, rinfo.HisCard, sizeof(rela.HisCard));
			result=tblRelation(DB_SELECT, &rela);
			if(result!=0 && result!=SQLNOTFOUND && result!=SQLPARTFOUND )
			{
				fprintf(stdout, "===tblRelation(DB_SELECT, &rela):select %dth record error[%d]\n", lrec, result);
				break;
			}
			else if(SQLPARTFOUND == result)
			{
				fprintf(stdout, "===tblRelation(DB_SELECT, &rela):select column value is NULL:%d\n", lrec);
				break;
			}
			else if(SQLNOTFOUND == result)
			{/*关联关系在交换中心不存在,交换中心新增关联关系*/
				transRelation(&rela, &rinfo);/*rinfo --> rela*/
				memcpy(rela.BankId, BK_ICBC, sizeof(rela.BankId));/*************************************************************/
				result = tblRelation(DB_INSERT, &rela);
				if(0 != result) 
				{
					fprintf(stdout, "===tblRelation(DB_INSERT, &rinfo):insert %dth record error[%d]\n", lrec, result);
					break;
				}
			}else if('0' == rela.rel_state[0])
			{/*关联关系在交换中心存在且是“解绑”状态,交换中心更新关联关系中相关的信息*/
				memcpy(rela.BankId, BK_ICBC, sizeof(rela.BankId));/***********************************************************/
				rela.rel_state[0] = '1';
				result = tblRelation(DB_UPDATE, &rela);
				if(0 != result)  
				{
					fprintf(stdout, "===tblMergereq(DB_UPDATE, &ainfo);===ERROR[%d]=========\n",result);
					break;
				}
			}
			#endif
		}
		
		if(0 == lrec%300)
		{
			if(0 != DbCommit())
				fprintf(stdout, "DbCommit Failed[%d] record[%d]!\n", result,lrec);
			else
				fprintf(stdout, "DbCommit record[%d]!\n",lrec);
        }
		/**<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<**/
	}
	
	if(0 != lrec%300)
	{
		if(0 != DbCommit())
			fprintf(stdout, "DbCommit Failed[%d] record[%d]!\n", result,lrec);
		else
			fprintf(stdout, "DbCommit record[%d]!\n", lrec);
    }
	/* 关闭游标 */
	result = tblMergerelareq(DB_CLOSE, &rinfo);
    if(result!=0 && result!=SQLPARTFOUND  )  
    {
        fprintf(fp_log, "close tblMergerelareq erro[%d]\n",result);
    }
    
    /* 关闭数据库、文件 */
    result = DbClose();
	if(result) 
    {
		fprintf(fp_log, "DbClose Failed[%d]!\n", result);
	}  

	/* 关闭文件指针 */
	fclose(fpr_bc_err);
	fclose(fpr_icbc);
	fclose(fp_log);
	fprintf(stderr, "close file\n"); 
	
	return 0;
}

int transRelation(struct card_join_acct *rel, struct relinfo* rinfo)
{
	memcpy(rel->IdType,  rinfo->IdType, sizeof(rel->IdType));
	memcpy(rel->IdNo,  rinfo->IdNo, sizeof(rel->IdNo));
	memcpy(rel->HisCard,  rinfo->HisCard, sizeof(rel->HisCard));
	memcpy(rel->BankId,  rinfo->IssBank, sizeof(rel->BankId));
	/*memcpy(rel->create_date,  " ", sizeof(rel->create_date));
	memcpy(rel->cancel_date,  " ", sizeof(rel->cancel_date));*/
	rel->rel_state[0] = '1';
	
	return 0;
}