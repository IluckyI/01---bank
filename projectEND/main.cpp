#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include"admin.h"
#include"general.h"
#include"user.h"

static FILE *admfPtr = 0;
static FILE *cfPtr = 0;
static UserData ss_clients[100] = { 0 };
void txt_write();

void buff_1(FILE *file)
{
	fseek(file, 0, SEEK_SET);
	int i = 0;
	while (!feof(file))
	{
		int ret = fread(&ss_clients[i],sizeof(UserData),1,file);
		if (ret != 1)
			break;
		i++;
	}
}

void userUpdateRecord(FILE*file, UserPtr user)//   �û��޸�����
{
	getchar();
	char a = 0;
	printf("\t\t\t\t change your password?(Y/N):");
	scanf("%c", &a);
	if (a == 'N' || a == 'n')
		return;
	int Newinfo;
	printf("\t\t\t\t Please input New password:");
	scanf("%d", &Newinfo);
	//data.balance += transaction;
	user->password = Newinfo;
	printf("\t\t\t\t your New password is %d\n", user->password);
	fseek(file, -sizeof(UserData), SEEK_CUR);
	fwrite(user, sizeof(UserData), 1, file);
	int index = recordIndex(user->acctNum);
	ss_clients[index].password = user->password;//�������ݻ���
	fseek(cfPtr,
		index * sizeof(UserData),
		SEEK_SET);
	fwrite(&ss_clients[index], sizeof(UserData), 1,
		cfPtr);//�����ļ�

			   //	updateData(UPDATE, user);//
}
void updateRecord(FILE *fPtr)           //����Ա���ݸ��º���
{
	int account;

	printf("\t\t\t---------------------------------------------------\n");
	printf("\t\t\t\tEnter account to update : ");
	scanf("%d", &account);

	int index = recordIndex(account);
	if (-1 == index)
	{
		printf("\t\t\t\tAcount #%d has no information.\n", account);
		printf("\t\t\t---------------------------------------------------\n");
		return;
	}

	UserData data = ss_clients[index];

	printf("\t\t\t\t%-6d%-25s%10.2f\n\n",
		data.acctNum, data.name,
		data.balance);
	printf("\t\t\t\tEnter charge ( + ) or payment ( - ): ");
	double transaction;
	scanf("%lf", &transaction);
	if (transaction > data.balance)
	{
		printf("not enough money!");
		return;

	}
	data.balance += transaction;
	printf("\t\t\t\t%-6d%-25s% 10.2f\n",
		data.acctNum, data.name,
		data.balance);

	updateData(UPDATE, &data);
	printf("\t\t\t---------------------------------------------------\n");
}

int recordIndex(int account)
{
	for (int i = 0; i < MAX_ACCOUNT_NUM; i++)
	{
		if (ss_clients[i].acctNum == account)
		{
			return i;
		}
	}
	return  -1;
}

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
		printf("\t\t\tAccount %d does not valid.\n", data->acctNum);
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
		ss_clients[index].acctNum = INVALID_ACCOUNT_NO;
	}
	break;
	case UPDATE:
	{
		//��������
		ss_clients[index].balance = data->balance;//�������ݻ���
		fseek(cfPtr,
			index * sizeof(UserData),
			SEEK_SET);
		fwrite(&ss_clients[index], sizeof(UserData), 1,
			cfPtr);//�����ļ�

	}
	break;
	case INSERT:
	{
		//�������ݼ�¼�ļ�
		fseek(cfPtr, index * sizeof(UserData), SEEK_SET);
		fwrite(data, sizeof(UserData), 1, cfPtr);
		//���»���������	
		ss_clients[index] = *data;
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

	buff_1(cfPtr);

	initialize(cfPtr);
	fclose(cfPtr);
	char login;
	while ((login = enterlogin()) != LOGEXIT)
	{
		//getchar();
		for (; login == '\n';)
		{
			login = getchar();
        }
			if (login == LOGEXIT)
				break ;
		
		switch (login)
		{
		case ADMIN:
		{
			cfPtr = fopen(FILE_PATH, "r+");//Ӧ��Ϊr+
			initialize(cfPtr);
			admchoice(cfPtr, adminacc);
			txt_write();
			break;
		}

		case USER:
		{
			printf("\t\t\t---------------------------------------------------\n");
			Userchoice(ss_clients);
			txt_write();
			break;
		}
		default:
			printf("\t\t\t\t\t\terror��\n");
			break;
		}
		//getchar();
	}
	fclose(cfPtr);

	fclose(admfPtr);
	free(adminacc);
	return 0;
}



void txt_write()
{
	FILE *writePtr = fopen("accounts.txt", "w+");
	fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
		"Acct", "password", "Name", "Balance");
	for(int i=0;i<100;i++)
	{
		if (ss_clients[i].acctNum == 0)
			continue;
		fprintf(writePtr, "%-6d%-16d%-11s%10.2lf\n",
		ss_clients[i].acctNum, ss_clients[i].password,
			ss_clients[i].name, ss_clients[i].balance);
	}


}