sqlplus $DB_USER_NAME/$DB_USER_PWD@$ORACLE_SID <<EOF

alter index tmp_acct_idx_0104 rebuild;
alter index tmp_rel_idx_0104 rebuild;

EOF
