#pragma once
#include<cstdio>
#include<cstdlib>
#include <cstdbool>
#include <cstring>
#define _CRT_SECURE_NO_WARNINGS
#ifndef __GENERAL_CPP__
#define __GENERAL_CPP__
enum OperatorType
{
	INSERT,
	DELETE,
	UPDATE,
};

enum loginname
{
	ADMIN = '1',
	USER,
	LOGEXIT,
};

enum adminexe
{
	ADMWRITE = '1',
	ADMUPDATE,
	ADMNEW,
	ADMDELETE,
	ADMFIND,
	ADMOUTPUT,
	ADMEXIT,
};
enum Userexe
{
	USERWRITE = '1',
	USERUPDATE,
	USEROUTPUT,
	USERFIND,
	USEREXIT,
};
enum
{
	INVALID_ACCOUNT_NO = 0,
};

typedef struct _admin  //管理员结构体
{
	char adminName[20];
	int password;
}admin, *adminPtr;


typedef struct _User   //用户结构体
{
	int acctNum;
	int password;
	char name[20];
	double balance;
}UserData, *UserPtr;
void findRecord(FILE*, int);
int	 updateData(OperatorType type, UserData* data);
int enterPassword(void);
char enterlogin(void);
void textFile(FILE *);
void updateRecord(FILE *);
void initialize(FILE *);
int  recordIndex(int);


static const int MAX_ACCOUNT_NUM = 100;
static const char* FILE_PATH = "credit.dat";
static const char* Admin_PATH = "admin.dat";
static UserData s_clients[MAX_ACCOUNT_NUM] = { 0 };

//static FILE *admfPtr = 0;
static FILE *ufPtr = 0;
#endif