#!/bin/ksh
bank=$1
#3.5������ϵ������ͬ�����ù�����ϵ��Ӧ�������˻�����
sqlplus $DB_USER_NAME/$DB_USER_PWD@prepaid <<EOF
update tbl_tmp_mergereq_${bank} set ErrCode='EB' where IdNo In 
(select distinct r.IdNo from tbl_tmp_mergerelareq_${bank} r, tbl_tmp_mergereq_${bank} a
	Where r.ErrCode='EB' and r.IdNo = a.IdNo and a.Errcode is null
);

commit;

EOF
