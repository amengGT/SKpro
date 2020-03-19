sqlplus $DB_USER_NAME/$DB_USER_PWD@$ORACLE_SID <<EOF

--账户信息表--
create table tbl_tmp_mergereq_01042900
(
	--机构号--
	Bank char(8) not null,
	LineNum number(8),
	--错误代码--
	ErrCode char(2),
	--余额变动标记--
	Flag    char(1),
	IdType  char(2) not null,
	IdNo    char(20) not null,
	Name    char(30) not null,
	Gender  char(1) not null,
	HospId  char(11) not null,
	BankId  char(8) not null,
	Balance number(12) not null,
	Stime   char(14) not null,
	TelNo   char(11),
	Addr    char(100),
	TxnMsg  char(200)
);

create index tmp_acct_idx_0104
on tbl_tmp_mergereq_01042900
(
	Bank,
	IdNo,
	Balance,
	Stime DESC
)
NOLOGGING;

create index tmp_acct_idx2_0104
on tbl_tmp_mergereq_01042900
(
	IdTYPE,
	IDNO
)
NOLOGGING;

create index tmp_acct_idx3_0104
on tbl_tmp_mergereq_01042900
(
	Bank
)
NOLOGGING;

--关联关系表--
create table tbl_tmp_mergerelareq_01042900
(
	Bank    char(8) not null,
	LineNum number(8),
	ErrCode char(2),
	Flag    char(1),
	IssBank char(8) not null,
	IdType  char(2) not null,
	IdNo    char(20) not null,
	HisCard char(30) not null
);

create index tmp_rel_idx_0104
on tbl_tmp_mergerelareq_01042900
(
	Bank,
	IdNo,
	HisCard
)
NOLOGGING;

create index tmp_rel_idx2_0104
on tbl_tmp_mergerelareq_01042900
(
	HisCard
)
NOLOGGING;

create index tmp_rel_idx3_0104
on tbl_tmp_mergerelareq_01042900
(
	Bank
)
NOLOGGING;


EOF
