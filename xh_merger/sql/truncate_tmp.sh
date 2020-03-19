sqlplus $DB_USER_NAME/$DB_USER_PWD@$ORACLE_SID <<EOF

truncate table tbl_tmp_mergereq_01042900;
truncate table tbl_tmp_mergerelareq_01042900;
commit;

EOF
