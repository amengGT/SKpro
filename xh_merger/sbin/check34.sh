#!/bin/ksh
bank=$1
#3.3检查每张就医卡是否仅属于一个账户（医疗卡是否重复）
#医疗卡重复(EB)
sqlplus $DB_USER_NAME/$DB_USER_PWD@prepaid <<EOF
update tbl_tmp_mergerelareq_${bank} set ErrCode='EB' where HisCard In 
(select HisCard from tbl_tmp_mergerelareq_${bank} 
		Where ErrCode is null Group By HisCard Having COUNT(*) > 1
);

commit;

EOF
