#include"general.h"
#define _CRT_SECURE_NO_WARNINGS



char enterlogin(void)  //输入登录方式  admin or User
{
	char login[100];
	printf("\t\t\t---------------------------------------------------\n");
	printf("\t\t\tEnter your log in(admin :1 or User :2 or exit :3\n");
	printf("\t\t\t\t\t\t");
	scanf("%s", login);
	if(strlen(login)==1)
	return login[0];
	else
	{
		return '0';
	}
}
int enterPassword(void)
{
	int password;
	printf("\t\t\tPlease input password:");
	scanf("%d", &password);
	return password;
}

void initialize(FILE *file)
{
	memset(s_clients, 0, MAX_ACCOUNT_NUM * sizeof(UserData));
	int count = 0;
	while (!feof(file) && count < MAX_ACCOUNT_NUM)
	{
		size_t ret = fread(&s_clients[count], sizeof(UserData), 1, file);
		if (ret != 1)
		{
			break;
		}
		count++;
	}
}


void textFile(FILE *readPtr)
{
	FILE *writePtr = 0;
	UserData client = { 0, 0, "", 0.0 };

	if ((writePtr = fopen("accounts.txt", "w+")) == NULL)//同cfptr一样，后改用w
	{
		printf("File could not be opened.\n");
	}
	else
	{
		rewind(readPtr);
		fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
			"Acct", "password", "Name", "Balance");

		while (!feof(readPtr))
		{
			size_t ret = fread(&client, sizeof(UserData), 1,
				readPtr);
			if (ret != 1)
			{
				break;
			}

			if (client.acctNum != INVALID_ACCOUNT_NO)
				fprintf(writePtr, "%-6d%-16d%-11s%10.2lf\n",
					client.acctNum, client.password,
					client.name, client.balance);
		}

		fclose(writePtr);
	}

}

//void updateRecord(FILE *fPtr)           //管理员数据更新函数
//{
//	int account;
//
//	printf("\t\t\t---------------------------------------------------\n");
//	printf("\t\t\t\tEnter account to update : ");
//	scanf("%d", &account);
//
//	int index = recordIndex(account);
//	if (-1 == index)
//	{
//		printf("\t\t\t\tAcount #%d has no information.\n", account);
//		printf("\t\t\t---------------------------------------------------\n");
//		return;
//	}
//
//	UserData data = ss_client[index];
//
//	printf("\t\t\t\t%-6d%-25s%10.2f\n\n",
//		data.acctNum, data.name,
//		data.balance);
//	printf("\t\t\t\tEnter charge ( + ) or payment ( - ): ");
//	double transaction;
//	scanf("%lf", &transaction);
//	data.balance += transaction;
//	printf("\t\t\t\t%-6d%-25s% 10.2f\n",
//		data.acctNum, data.name,
//		data.balance);
//
//	updateData(UPDATE, &data);
//	printf("\t\t\t---------------------------------------------------\n");
//}

//void findRecord(FILE*file, int acctNum)  //用户查询其他用户的标准信息（即姓名，账号，余额）
//{
//	if (mysql_query(con, "SELECT img FROM cars WHERE acctNum=''"))
//	{
//		finish_with_error(con);
//	}
//
//
//
//
//
//
//
//
//
//
//
//	//UserPtr data = (UserPtr)malloc(sizeof(UserData));
//	//fseek(file, 0, SEEK_SET);
//
//	//while (!feof(file))
//	//{
//	//	fread(data, sizeof(UserData), 1, file);
//	//	if (data->acctNum == acctNum)
//	//	{
//	//		printf("\t\t\t\t%-6s  %-16s%10s\n", "acctnum", "name", "balance");
//	//		printf("\t\t\t\t%-6d  %-16s%10.2lf\n", data->acctNum, data->name, data->balance);
//	//		free(data);
//	//		data = 0;
//	//		printf("\t\t\t---------------------------------------------------\n");
//	//		return;
//	//	}
//	//	//		fseek(file, sizeof(UserData), SEEK_CUR);
//
//	//}
//	//printf("\t\t\t\tDon't have this acctNum!\n");
//	//free(data);
//	//data = 0;
//	printf("\t\t\t---------------------------------------------------\n");
//}




