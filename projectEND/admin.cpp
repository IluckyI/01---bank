#include"admin.h"

#define _CRT_SECURE_NO_WARNINGS
void deleteRecord(FILE *fPtr)      //����Ա��ɾ��
{
	int accountNum;

	printf("Enter account number to "
		"delete ( 1 - 100 ): ");
	scanf("%d", &accountNum);

	UserData 	blankClient = { INVALID_ACCOUNT_NO, 0, "", 0 };
	blankClient.acctNum = accountNum;

	updateData(DELETE, &blankClient);

}
void outputInfo(FILE*file)    //����Ա��������˻���Ϣ��������⣩
{
	printf("%-6s  %-16s%10s\n", "acctnum", "name", "balance");
	UserPtr data = (UserPtr)malloc(sizeof(UserData));
	fseek(file, 0, SEEK_SET);
	while (!feof(file))
	{

		size_t ret = fread(data, sizeof(UserData), 1, file);
		if (ret != 1)
		{
			break;
		}
		if (data->acctNum == 0)
			continue;
		printf("%-6d  %-16s%10.2lf\n", data->acctNum, data->name, data->balance);
	}
}

char * enternum()  //�������Ա�˻�
{
	char num[10];
	printf("Please input admin name\n");
	scanf("%s", num);
	return num;
}

void newRecord(FILE *fPtr)
{
	UserData client = { 0, 0, "", 0.0 };
	int accountNum;
	printf("Enter new account number : ");
	scanf("%d", &accountNum);
	//�ҵ��ü�¼���б��е�����
	int index = recordIndex(accountNum);
	if (-1 != index)//����ü�¼�Ѿ�����
	{
		printf("Account  already contains information.\n",
			client.acctNum);
		return;
	}
	printf("Enter password,name, balance\n? ");
	scanf("%d%s%lf", &client.password, client.name,
		&client.balance);
	client.acctNum = accountNum;

	updateData(INSERT, &client);
}
void admchoice(FILE*cfPtr, admin * adminacc)    //  ����Ա����
{
	char *accnump = 0;
	accnump = enternum();
	if (strcmp(accnump, adminacc->adminName))
	{
		printf("|  Don't have this account!\n");
		return;
	}
	int password = enterPassword();
	if (password != adminacc->password)
	{
		printf("|  The password is error!\n");
		return;
	}
	printf("---------------------------------------------------\n");
	int choice;
	while ((choice = enterChoice()) != ADMEXIT)
	{
		int acctnum = 0;
		switch (choice)
		{
		case ADMWRITE:
			textFile(cfPtr);
			break;
		case ADMUPDATE:
			updateRecord(cfPtr);
			break;
		case ADMNEW:
			newRecord(cfPtr);
			break;
		case ADMDELETE:
			deleteRecord(cfPtr);
			break;
		case ADMFIND:
			printf("Please input you find information\n");
			scanf("%d", &acctnum);
			findRecord(cfPtr, acctnum);
			break;
		case ADMOUTPUT:
			outputInfo(cfPtr);
			break;
		}
	}
	fclose(cfPtr);

}
int enterChoice(void)
{
	int menuChoice;

	printf("                  ***************************\n"
		"                  *     Enter your choice   *\n"
		"                  * 1 - save information    *\n"
		"                  * 2 - update an account   *\n"
		"                  * 3 - add a new account   *\n"
		"                  * 4 - delete an account   *\n"
		"                  * 5 - find account        *\n"
		"                  * 6 - output all info     *\n"
		"                  * 7 - end program?        *\n"
		"                  ***************************\n");
	scanf("%d", &menuChoice);
	return menuChoice;
}