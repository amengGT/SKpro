#!/bin/ksh
bank=$1

#3.4�˻���¼������ͬ�������˻���Ӧ�����й�����ϵ�ܾ�
sqlplus $DB_USER_NAME/$DB_USER_PWD@prepaid <<EOF
update tbl_tmp_mergerelareq_${bank} set ErrCode='EA' where IdNo In 
(select distinct a.IdNo from tbl_tmp_mergereq_${bank} a, tbl_tmp_mergerelareq_${bank} r
	Where a.ErrCode='EA' and a.IdNo = r.IdNo and r.errcode is null
);

commit;

EOF
