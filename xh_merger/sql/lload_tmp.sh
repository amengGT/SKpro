#!/bin/ksh
echo "clean last time history..."
cd ${MERGE_HOME}/sql/ctl
rm -f load_tmp_acct_*
rm -f load_tmp_rel_*
echo "done"
cd ${MERGE_HOME}/prefile #格式检查产生的文件所在路径
flist=`ls *PRELOAD*|grep MERGEREQ`
cd ${MERGE_HOME}/sql/ctl
for fname in $flist
do
	bankid=`expr $fname : 'ACCT-\([0-9]*\)-M.*'`
	echo $bankid
	cd ${MERGE_HOME}/prefile
	relname=`ls *PRELOAD*|grep ${bankid}-MERGERELAREQ`
	cd ${MERGE_HOME}/sql/ctl

	echo "generate load_tmp_acct_$bankid.ctl file ..."
	echo ">>>>>FILE[$fname]<<<<<"
	#sed  's/dog/cat/g' file      ##dog被替换的内容，cat替换的内容
	sed "s/TMP_ACCT/$fname/;s/XXXXXXXX/${bankid}/" load_tmp_acctinf.ctl >load_tmp_acct_$bankid.ctl
	echo "process load tmp acctinfo[$bankid]..."
	sqlldr $DB_USER_NAME/$DB_USER_PWD@$ORACLE_SID control=load_tmp_acct_$bankid.ctl log=load_tmp_acct_$bankid.log

	echo "generate load_tmp_rel_$bankid.ctl file ..."
	echo ">>>>>FILE[$relname]<<<<<"
	sed "s/TMP_REL/$relname/;s/XXXXXXXX/${bankid}/" load_tmp_relinf.ctl >load_tmp_rel_$bankid.ctl
	echo "process load tmp relinfo[$bankid]..."
	sqlldr $DB_USER_NAME/$DB_USER_PWD@$ORACLE_SID control=load_tmp_rel_$bankid.ctl log=load_tmp_rel_$bankid.log
	echo "load $bankid tmp table complete .."
done

echo "All done"

#echo "---Waiting for recreating Index for acct / rel "
#cd ${MERGE_HOME}/sql
#./alter_tmp_index.sh
cd ${MERGE_HOME}/sql/ctl
rm -f load_tmp_acct_*
rm -f load_tmp_rel_*
sleep 1
