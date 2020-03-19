#include "format.h"

 extern char bankno[];
int check_acct_format(struct facctinf* acct_inf, char* errcode)
{
	int i;

	if(memcmp(acct_inf->IdType,"01", sizeof(acct_inf->IdType))) {
		memcpy(errcode,"E5", 2);
		return FAIL;
	}

	for(i=0;acct_inf->IdNo[i] != ' ' && acct_inf->IdNo[i] != '\0';i++);
	if(15 == i) {
		if(isDigit(acct_inf->IdNo, 15)) {
			memcpy(errcode, "E2", 2);
			return FAIL;
		}
	}
	else if(i == 18) {
		if(isDigit(acct_inf->IdNo, 17)) {
			memcpy(errcode, "E2", 2);
			return FAIL;
		}
		if(acct_inf->IdNo[17]=='x')
			acct_inf->IdNo[17] = 'X';
		if((acct_inf->IdNo[i-1] < '0') || 
			(acct_inf->IdNo[i-1] > '9' && acct_inf->IdNo[i-1] != 'X') ) 
		{
			memcpy(errcode, "E2", 2);
			return FAIL;
		}
	}else{
		memcpy(errcode, "E2", 2);
		return FAIL;
	}

	if(acct_inf->Gender[0] != 'F' && acct_inf->Gender[0] != 'M') {
		memcpy(errcode, "E6", 2);
		return FAIL;
	}

	if(memcmp(bankno, acct_inf->BankId, sizeof(acct_inf->BankId))) {
		memcpy(errcode, "E1", 2);
		return FAIL;
	}

	if(isDigit(acct_inf->Balance, sizeof(acct_inf->Balance)-1)) {
		memcpy(errcode, "E4", 2);
		return FAIL;
	}

	if(memcmp(acct_inf->Balance,"999999999999",4) > 0) {/**10000000**/
		memcpy(errcode, "E4", 2);
		return FAIL;
	}

	if(isDigit(acct_inf->Stime, sizeof(acct_inf->Stime)-1)) {
		memcpy(errcode, "E7", 2);
		return FAIL;
	}

	if(memcmp(acct_inf->Stime,"2000",4) < 0) {
		memcpy(errcode, "E7", 2);
		return FAIL;
	}
	
	if(memcmp(acct_inf->HospId, HOSP_ID, sizeof(acct_inf->HospId) - 1)) {
		memcpy(errcode, "E3", 2);
		return FAIL;
	}
	
	return SUCCESS;
}

int check_rel_format(struct frelinf* rel_inf, char* errcode)
{
	int i;

	if(memcmp(rel_inf->IssBank, bankno, sizeof(rel_inf->IssBank))) {
		memcpy(errcode, "E1", 2);
		return FAIL;
	}
	
	if(memcmp(rel_inf->IdType,"01", sizeof(rel_inf->IdType))) {
		memcpy(errcode, "E5", 2);
		return FAIL;
	}

	for(i=0;rel_inf->IdNo[i] != ' '&&rel_inf->IdNo[i] != '\0';i++);
	if(i != 15 && i != 18) {
		memcpy(errcode, "E2", 2);
		return FAIL;
	}
	if(i == 15) {
		if(isDigit(rel_inf->IdNo, 15)) {
			memcpy(errcode, "E2", 2);
			return FAIL;
		}
	}
	else {
		if(isDigit(rel_inf->IdNo, 17)) {
			memcpy(errcode, "E2", 2);
			return FAIL;                                                        
		}
	}

	if( i==18 && rel_inf->IdNo[17]=='x')
		rel_inf->IdNo[17] = 'X';

	if( i == 18 && (rel_inf->IdNo[i-1] < '0' || 
				(rel_inf->IdNo[i-1] > '9' && 
				rel_inf->IdNo[i-1] != 'X')) ) {
		memcpy(errcode, "E2", 2);
		return FAIL;
	}

	return SUCCESS;
}


