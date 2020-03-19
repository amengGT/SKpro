#ifndef _FORMAT_H_
#define _FORMAT_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#define BK_BC	"01042900"
#define BK_ICBC	"01022900"
#define HOSP_ID	"42502659900"

#define MAXSIZE 1024
#define SUCCESS 0
#define FAIL	1
#define SQLNOTFOUND 1403
#define SQLPARTFOUND -1405

#define DB_INIT 0	/*没用到*/
#define DB_CUR 1	/*DB_CUR_DECLARE*/
#define DB_OPEN 2	/*DB_CUR_OPEN*/
#define DB_FETCH 3	/*DB_CUR_FETCH*/
#define DB_CLOSE 4	/*DB_CUR_CLOSE*/
#define DB_SELECT 5		/*DB_DML_SELECT*/
#define DB_SELECT1 6
#define DB_UPDATE 7		/*DB_DML_UPDATE*/
#define DB_UPDATE2 8
#define DB_UPDATE3 9
#define DB_INSERT	15	/*DB_DML_INSERT*/
#define DB_CUR1 11
#define DB_OPEN1 12
#define DB_FETCH1 13
#define DB_CLOSE1 14

/*******************************/
/*  file process use struct    */
struct facctinf {
	char IdType[2+1];
	char IdNo[20+1];
	char Name[30+1];
	char Gender[1+1];
	char HospId[11+1];
	char BankId[8+1];
	char Balance[12+1]; /** need to check Balance correct,should keep origin **/
	char Stime[14+1];
	char TelNo[11+1];
	char Addr[100+1];
	char TxnMsg[200+1];
};/**LEN: 409 + 11**/

struct frelinf {
	char IssBank[8+1];
	char IdType[2+1];
	char IdNo[20+1];
	char HisCard[30+1];
};/**LEN: 60 + 4**/
/*******************************/
struct acctinfo {
	char Bank[8+1];
	int  LineNum;
	char ErrCode[2+1];
	char Flag;
	char IdType[2+1];
	char IdNo[20+1];
	char Name[30+1];
	char Gender;
	char HospId[11+1];
	char BankId[8+1];
	long Balance;/*LEN:12*/
	char Stime[14+1];
	char TelNo[11+1];
	char Addr[100+1];
	char TxnMsg[200+1];
};/**LEN: 428 + 15**/

struct relinfo {
	char Bank[8+1];
	int  LineNum;
	char ErrCode[2+1];
	char Flag;
	char IssBank[8+1];
	char IdType[2+1];
	char IdNo[20+1];
	char HisCard[30+1];
};/**LEN: 79 + 8**/
/*******************************/

/*交换中心关联关系表*/
struct card_join_acct {
	char IdType   [   2+1];
	char IdNo     [  20+1];
	char HisCard  [  30+1];
	char BankId   [  8 +1];
	char create_date[14+1];
	char cancel_date[14+1];
	char rel_state[  1 +1];
};

/*交换中心账户表*/
struct acct_inf {
	char IdType   [   2+1];
	char IdNo     [  20+1];
	char BankId   [   8+1];
	char unitno   [  11+1];
	char name     [  30+1];
	char gender   [  1 +1];
    char telno    [  11+1];
    char addr     [ 100+1];
    char open_date[  14+1];
    char last_mod_date[  14+1];
    char state    [  1 +1];
};
/*身份类型、身份证 结构体*/
/*
struct ID {
	char IdType [     2+1];
	char IdNo   [    20+1];
    char HisCard[    30+1]; 
};*/

#endif
