#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include"admin.h"
#include"general.h"
#include"user.h"

static FILE *admfPtr = 0;
static FILE *cfPtr = 0;
bool fileExist(const char* file)
{
	FILE *pf = fopen(file, "r");
	if (pf == 0)
		return false;
	fclose(pf);
	return true;
}
int updateData(OperatorType type, UserData * data)
{
	int index = recordIndex(type == INSERT ? INVALID_ACCOUNT_NO : data->acctNum);
	if (-1 == index)
	{
		printf("Account %d does not valid.\n", data->acctNum);
		return -1;
	}

	switch (type)
	{
	case DELETE:
	{
		//更新文件
		fseek(cfPtr, index * sizeof(UserData), SEEK_SET);
		data->acctNum = INVALID_ACCOUNT_NO;
		fwrite(data,
			sizeof(UserData), 1, cfPtr);
		//更新内存数据
		s_clients[index].acctNum = INVALID_ACCOUNT_NO;
	}
	break;
	case UPDATE:
	{
		//更新数据
		s_clients[index].balance = data->balance;//更新数据缓冲
		fseek(cfPtr,
			index * sizeof(UserData),
			SEEK_SET);
		fwrite(&s_clients[index], sizeof(UserData), 1,
			cfPtr);//更新文件

	}
	break;
	case INSERT:
	{
		//更新数据记录文件
		fseek(cfPtr, index * sizeof(UserData), SEEK_SET);
		fwrite(data, sizeof(UserData), 1, cfPtr);
		//更新缓冲区数据	
		s_clients[index] = *data;
	}
	break;
	default:
		break;
	}

	return 0;
}

int main()
{
	admin *adminacc = 0;
	adminacc = (adminPtr)malloc(sizeof(admin));
	if (!fileExist(Admin_PATH))
	{
		adminPtr adm = (adminPtr)malloc(sizeof(admin));
		admfPtr = fopen(Admin_PATH, "w+");
		strcpy(adm->adminName, "admin");
		adm->password = 666666;

		strcpy(adminacc->adminName, "admin");
		adminacc->password = adm->password;

		fwrite(adm, sizeof(admin), 1, admfPtr);
		fclose(admfPtr);
		free(adm);
	}
	else
	{
		admfPtr = fopen(Admin_PATH, "r+");
		fread(adminacc, sizeof(admin), 1, admfPtr);

	}
	//对文件是否存在分情况处理
	if (!fileExist(FILE_PATH))
	{
		cfPtr = fopen(FILE_PATH, "w+");
	}
	else
	{
		cfPtr = fopen(FILE_PATH, "r+");//实验：先覆盖，后改用（r+）
	}

	if (cfPtr == 0)
	{
		perror("create file:");
		return -1;
	}

	initialize(cfPtr);
	fclose(cfPtr);
	int login;
	while ((login = enterlogin()) != LOGEXIT)
	{
		switch (login)
		{
		case ADMIN:
		{
			cfPtr = fopen(FILE_PATH, "r+");//应改为r+
			initialize(cfPtr);
			admchoice(cfPtr, adminacc);
			break;
		}

		case USER:
		{
			Userchoice();
			break;
		}
		}
	}
	fclose(cfPtr);

	fclose(admfPtr);
	free(adminacc);
	return 0;
}



