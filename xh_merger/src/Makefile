OBJ=getrec.o pub.o fetchdata.o
CFILE=getrec.c pub.c fetchdata.c
POBJ=database.o tbl_pub.o
PCFT=database.c tbl_pub.c
PCF=database.pc tbl_pub.pc
CC=cc
CFLAGS=-g
#SPCFLAGS=-L$(ORACLE_HOME)/lib -lclntsh -lecpg #no used
PCFLAGS=-L$(ORACLE_HOME)/lib32 -lclntsh
INC=-I ../include
PINC=include=../include
PCC=proc

CHKFILE=../bin/check_format
RESACCT=../bin/result_acct
RESREL=../bin/result_rel

all : $(CHKFILE) $(RESACCT) $(RESREL) clean

#$@  表示目标文件
#$^  表示所有的依赖文件
#$<  表示第一个依赖文件
#$?  表示比目标还要新的依赖文件列表

###DataBase
database.c : database.pc
	$(PCC) iname=database.pc $(PINC)
tbl_pub.c : tbl_pub.pc
	$(PCC) iname=tbl_pub.pc $(PINC)
$(OBJ) $(POBJ) : $(CFILE) $(PCFT) 
	$(CC) $(CFLAGS) $(PCFLAGS) $(INC) -c $< -o $@    
	
###Check_File
$(CHKFILE) : check_format.o $(OBJ)
	$(CC) $(CFLAGS) -o $(CHKFILE) check_format.o $(OBJ)
check_format.o : check_format.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
    
###Result_acct
$(RESACCT): result_acct.o $(OBJ) $(POBJ)
	$(CC) $(CFLAGS) $(PCFLAGS) -o $(RESACCT) result_acct.o $(OBJ) $(POBJ)
result_acct.o : result_acct.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
	
###	Result_rel
$(RESREL): result_rel.o $(OBJ) $(POBJ)
	$(CC) $(CFLAGS) $(PCFLAGS) -o $(RESREL) result_rel.o $(OBJ) $(POBJ)
result_rel.o : result_rel.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
    
clean:
	-rm  -f *.o tp* *.lis tp* tbl_*.c database.c tool.c 
