#!/bin/ksh
bank=$1
#3.3���ÿ�ž�ҽ���Ƿ������һ���˻���ҽ�ƿ��Ƿ��ظ���
#ҽ�ƿ��ظ�(EB)
sqlplus $DB_USER_NAME/$DB_USER_PWD@prepaid <<EOF
update tbl_tmp_mergerelareq_${bank} set ErrCode='EB' where HisCard In 
(select HisCard from tbl_tmp_mergerelareq_${bank} 
		Where ErrCode is null Group By HisCard Having COUNT(*) > 1
);

commit;

EOF
