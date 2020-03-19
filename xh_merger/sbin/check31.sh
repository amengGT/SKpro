#!/bin/ksh

#3.1 检查账户信息没有关联医疗卡
check_acct()
{
bank=$1
sqlplus $DB_USER_NAME/$DB_USER_PWD@prepaid <<EOF
update tbl_tmp_mergereq_${bank} set ErrCode='E8' where IdNo In 
(select distinct aId from 
	(select a.IdNo aId, r.IdNo rId from 
		tbl_tmp_mergereq_${bank} a left join tbl_tmp_mergerelareq_${bank} r
		on a.IdNo = r.IdNo
	) 
where rId is null
);
commit;
EOF
}

###################
check_acct $1

