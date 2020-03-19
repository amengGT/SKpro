#########
#!/bin/ksh
sqlplus $DB_USER_NAME/$DB_USER_PWD@$ORACLE_SID <<EOF

truncate table tbl_acct_inf;
truncate table tbl_card_join_acct;
commit;
EOF

cd $MERGE_HOME
imp sktest/sktestrun@prepaid IGNORE=Y TABLES=tbl_acct_inf FULL=N file=./acct.dmp
imp sktest/sktestrun@prepaid IGNORE=Y TABLES=tbl_card_join_acct FULL=N file=./rela.dmp
cd -
