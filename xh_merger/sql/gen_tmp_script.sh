#!/bin/ksh
printacct()
{
	bankno=$1	#����shell������˵'$1'��ʾ�����ĵ�һ������
	printf "--�˻���Ϣ��--\n"
	printf "create table tbl_tmp_mergereq_%-8s\n(\n" $bankno # -8s <---> $bankno
	printf "\t--������--\n"
	printf "\tBank char(8) not null,\n"
	printf "\tLineNum number(8),\n"
	printf "\t--�������--\n"
	printf "\tErrCode char(2),\n"
	printf "\t--���䶯���--\n"
	printf "\tFlag    char(1),\n"
	printf "\tIdType  char(2) not null,\n"
	printf "\tIdNo    char(20) not null,\n"
	printf "\tName    char(30) not null,\n"
	printf "\tGender  char(1) not null,\n"
	printf "\tHospId  char(11) not null,\n"
	printf "\tBankId  char(8) not null,\n"
	printf "\tBalance number(12) not null,\n"
	printf "\tStime   char(14) not null,\n"
	printf "\tTelNo   char(11),\n"
	printf "\tAddr    char(100),\n"
	printf "\tTxnMsg  char(200)\n"
	printf ");\n\n"

	printf "create index tmp_acct_idx_%-.4s\n" $bankno
	printf "on tbl_tmp_mergereq_%-8s\n" $bankno
	printf "(\n"
	printf "\tBank,\n"
	printf "\tIdNo,\n"
	printf "\tBalance,\n"
	printf "\tStime DESC\n"
	printf ")\n"
    printf "NOLOGGING;\n\n" 

    printf "create index tmp_acct_idx2_%-.4s\n" $bankno
    printf "on tbl_tmp_mergereq_%-8s\n" $bankno
    printf "(\n"
    printf "\tIdTYPE,\n"
    printf "\tIDNO\n"
    printf ")\n"
    printf "NOLOGGING;\n\n" 
  

    printf "create index tmp_acct_idx3_%-.4s\n" $bankno
	printf "on tbl_tmp_mergereq_%-8s\n" $bankno
	printf "(\n"
	printf "\tBank\n"
	printf ")\n"
    
	printf "NOLOGGING;\n\n"
}

printrel()
{
	bankid=$1
	printf "--������ϵ��--\n"
	printf "create table tbl_tmp_mergerelareq_%-8s\n(\n" $bankid
	printf "\tBank    char(8) not null,\n"
	printf "\tLineNum number(8),\n"
	printf "\tErrCode char(2),\n"
	printf "\tFlag    char(1),\n"
	printf "\tIssBank char(8) not null,\n"
	printf "\tIdType  char(2) not null,\n"
	printf "\tIdNo    char(20) not null,\n"
	printf "\tHisCard char(30) not null\n"
	printf ");\n\n"

	printf "create index tmp_rel_idx_%-.4s\n" $bankid
	printf "on tbl_tmp_mergerelareq_%-8s\n" $bankid
	printf "(\n"
	printf "\tBank,\n"
	printf "\tIdNo,\n"
	printf "\tHisCard\n"
	printf ")\n"
	printf "NOLOGGING;\n\n"


    printf "create index tmp_rel_idx2_%-.4s\n" $bankid
    printf "on tbl_tmp_mergerelareq_%-8s\n" $bankid
    printf "(\n"
    printf "\tHisCard\n"
    printf ")\n"
    printf "NOLOGGING;\n\n"


    printf "create index tmp_rel_idx3_%-.4s\n" $bankid
    printf "on tbl_tmp_mergerelareq_%-8s\n" $bankid
    printf "(\n"
    printf "\tBank\n"
    printf ")\n"
    printf "NOLOGGING;\n\n"


}

printhead()
{
	printf "sqlplus \$DB_USER_NAME/\$DB_USER_PWD@\$ORACLE_SID <<EOF\n\n"
}

printtail()
{
	printf "\nEOF\n"
}

###create_table_tmp.sh
cd $MERGE_HOME/sql
echo "Going to generate create table script! \n"
printhead > create_table_tmp.sh
for i in $@
do
	printacct ${i} >> create_table_tmp.sh #${i}��Ϊprintacct�Ĳ���
	printrel ${i} >> create_table_tmp.sh
done
printtail >> create_table_tmp.sh

###drop_table_tmp.sh/truncate_tmp.sh
echo "Going to generate droptable/truncate script! \n"
printhead > drop_table_tmp.sh
printhead > truncate_tmp.sh
for i in $@
do
	printf "drop table tbl_tmp_mergereq_%-8s;\n" ${i} >> drop_table_tmp.sh
	printf "drop table tbl_tmp_mergerelareq_%-8s;\n" ${i} >> drop_table_tmp.sh
	printf "truncate table tbl_tmp_mergereq_%-8s;\n" ${i} >> truncate_tmp.sh
	printf "truncate table tbl_tmp_mergerelareq_%-8s;\n" ${i} >> truncate_tmp.sh
	#truncate ɾ���������ݣ�ͨ����־�ɻָ�
	#delete ɾ���������ݣ����ɻָ������ٶȿ�
	#drop ɾ�����ݼ���ṹ
done
printf "commit;\n\nEOF\n" >> drop_table_tmp.sh
printf "commit;\n\nEOF\n" >> truncate_tmp.sh

###alter_tmp_index.sh
echo "Going to generate alterindex script! \n"
printhead > alter_tmp_index.sh
for i in $@
do
	printf "alter index tmp_acct_idx_%-.4s rebuild;\n" ${i} >> alter_tmp_index.sh
	printf "alter index tmp_rel_idx_%-.4s rebuild;\n" ${i} >> alter_tmp_index.sh
done
printtail >> alter_tmp_index.sh

###����shell�ű���ִ��Ȩ��
echo "chmod for scripts.."
chmod +x create_table_tmp.sh
chmod +x alter_tmp_index.sh
chmod +x truncate_tmp.sh
chmod +x drop_table_tmp.sh
