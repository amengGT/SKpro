#!/bin/ksh

#3.2 检查医疗卡没有关联账户
check_rel()
{
bank=$1
sqlplus $DB_USER_NAME/$DB_USER_PWD@prepaid <<EOF
update tbl_tmp_mergerelareq_${bank} set ErrCode='E9' where IdNo In 
(select distinct rId from 
	(select a.IdNo aId, r.IdNo rId from 
		tbl_tmp_mergereq_${bank} a right join tbl_tmp_mergerelareq_${bank} r 
		on a.IdNo = r.IdNo
	)
where aId is null
);
commit;
EOF
}

#############################
check_rel $1