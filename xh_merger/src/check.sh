#!/bin/ksh

chmod +x $MERGE_HOME/sql/gen_tmp_script.sh
chmod +x $MERGE_HOME/sql/lload_tmp.sh
export DB_USER_NAME="sktest"
echo $DB_USER_NAME
export DB_USER_PWD="sktestrun"
echo $DB_USER_PWD
export ORACLE_SID="prepaid"
echo $ORACLE_SID
export MERGE_HOME="$HOME/xh_merger"
echo $MERGE_HOME
cd $MERGE_HOME/bin
rm -rf ../prefile/* ../outfile/* ../log/*
clear
./check_format ACCT-01042900-MERGEREQ-20150121 ACCT-01042900-MERGERELAREQ-20150121
cd -
#.$MERGE_HOME/bin/check_format ACCT-01042900-MERGEREQ-20150121 ACCT-01042900-MERGERELAREQ-20150121
