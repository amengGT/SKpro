#!/bin/ksh
#3.2检查所有账户是否都属于同一银行（账户是否重复）
bank=$1
sqlplus $DB_USER_NAME/$DB_USER_PWD@prepaid <<EOF

update tbl_tmp_mergereq_${bank} set ErrCode='EA' where IdNo In 
(select IdNo from tbl_tmp_mergereq_${bank} 
	Where ErrCode is null Group By IdNo Having COUNT(*) > 1
);

commit;

EOF
