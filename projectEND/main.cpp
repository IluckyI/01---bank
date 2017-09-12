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
		//�����ļ�
		fseek(cfPtr, index * sizeof(UserData), SEEK_SET);
		data->acctNum = INVALID_ACCOUNT_NO;
		fwrite(data,
			sizeof(UserData), 1, cfPtr);
		//�����ڴ�����
		s_clients[index].acctNum = INVALID_ACCOUNT_NO;
	}
	break;
	case UPDATE:
	{
		//��������
		s_clients[index].balance = data->balance;//�������ݻ���
		fseek(cfPtr,
			index * sizeof(UserData),
			SEEK_SET);
		fwrite(&s_clients[index], sizeof(UserData), 1,
			cfPtr);//�����ļ�

	}
	break;
	case INSERT:
	{
		//�������ݼ�¼�ļ�
		fseek(cfPtr, index * sizeof(UserData), SEEK_SET);
		fwrite(data, sizeof(UserData), 1, cfPtr);
		//���»���������	
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
	//���ļ��Ƿ���ڷ��������
	if (!fileExist(FILE_PATH))
	{
		cfPtr = fopen(FILE_PATH, "w+");
	}
	else
	{
		cfPtr = fopen(FILE_PATH, "r+");//ʵ�飺�ȸ��ǣ�����ã�r+��
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
			cfPtr = fopen(FILE_PATH, "r+");//Ӧ��Ϊr+
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



