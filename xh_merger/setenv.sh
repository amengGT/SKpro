#!/bin/ksh

export DB_USER_NAME="sktest"
echo $DB_USER_NAME
export DB_USER_PWD="sktestrun"
echo $DB_USER_PWD
export ORACLE_SID="prepaid"
echo $ORACLE_SID
export MERGE_HOME="$HOME/xh_merger"
echo $MERGE_HOME

chmod +x ./sql/gen_tmp_script.sh
chmod +x ./sql/lload_tmp.sh

chmod +x ./sbin/check31.sh
chmod +x ./sbin/check32.sh
chmod +x ./sbin/check33.sh
chmod +x ./sbin/check34.sh
chmod +x ./sbin/check35.sh
chmod +x ./sbin/check36.sh
