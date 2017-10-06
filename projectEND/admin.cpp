#include"admin.h"

#define _CRT_SECURE_NO_WARNINGS
//void deleteRecord(FILE *fPtr)      //管理员的删除
//{
//	int accountNum;
//
//	printf("\t\t\tEnter account number to "
//		"delete: ");
//	scanf("%d", &accountNum);
//
//	UserData 	blankClient = { INVALID_ACCOUNT_NO, 0, "", 0 };
//	blankClient.acctNum = accountNum;
//
//	updateData(Delect, &blankClient);
//	sprintf(c, "INSERT INTO bank VALUES(%d, '%s', '%d','%lf')",);
//
//	if (mysql_query(con, c)) {
//		finish_with_error(con);
//	}
//
//	printf("\t\t\tdelete record successful!\n");
//
//}
//void outputInfo(FILE*file,UserData *ptr)    //管理员输出所有账户信息（密码除外）
//{
//	printf("\t\t\t\t%-6s  %-16s%10s\n", "acctnum", "name", "balance");
//	//UserPtr data = (UserPtr)malloc(sizeof(UserData));
//	//fseek(file, 0, SEEK_SET);
//	int i = 0;
//	for(;i<100;i++)
//	{
//
//		//size_t ret = fread(data, sizeof(UserData), 1, file);
//		//if (ret != 1)
//		//{
//		//	break;
//		//}
//
//
//		if (ptr[i].acctNum == 0)
//			continue;
//		printf("\t\t\t\t%-6d  %-16s%10.2lf\n", ptr[i].acctNum, ptr[i].name, ptr[i].balance);
//	}
//}

char * enterNum()  //输入管理员账户
{
	char num[10];
	printf("\t\t\tPlease input admin name:");
	scanf("%s", num);
	return num;
}

//void newRecord(FILE *fPtr)
//{
//	UserData client = { 0, 0, "", 0.0 };
//	int accountNum;
//	char c[100] = {0};
//	printf("\t\t\tEnter new account number : ");
//	scanf("%s", c);
//
//	int size = 0;
//	size=strlen(c);
//	for (int i = 0; i < size&&c[i]!='\n'; i++)
//	{
//		if (c[i] < '0' || c[i] > '9')
//		{
//			printf("\t\t\t\tinput error!!!!!\n");
//			printf("\t\t\t\tPlease input number !!!\n");
//			return;
//		}
//	}
//	accountNum=atoi(c);
//	
//	//找到该记录在列表中的索引
//	int index = recordIndex(accountNum);
//	if (-1 != index)//如果该记录已经存在
//	{
//		printf("\t\t\tAccount  already contains information.\n");
//		printf("\t\t\t---------------------------------------------------\n");
//		return;
//	}
//	printf("\t\t\tEnter password,name, balance\n ");
//	printf("\t\t\t      ?");
//	scanf("%d%s%lf", &client.password, client.name,
//		&client.balance);
//	client.acctNum = accountNum;
//
//	updateData(INSERT, &client);
//
//	if (mysql_query(con, "CREATE TABLE Cars(Id INT, Name TEXT, Price INT)")) {
//		finish_with_error(con);
//	}
//
//	if (mysql_query(con, "INSERT INTO Cars VALUES(1,'Audi',52642)")) {
//		finish_with_error(con);
//	}
//
//	printf("\t\t\t\t   newrecord successful!\n");
//	printf("\t\t\t---------------------------------------------------\n");
//}
void admChoice(FILE*cfPtr, admin * adminacc)    //  管理员命令
{
	char *accnump = 0;
	accnump = enterNum();
	if (strcmp(accnump, adminacc->adminName))
	{
		printf("| \t\t\t Don't have this account!\n");
		return;
	}
	printf("\n");
	int password = enterPassword();
	if (password != adminacc->password)
	{
		printf("| \t\t\t The password is error!\n");
		return;
	}
	printf("\t\t\t---------------------------------------------------\n");
	char choice;
	while ((choice = enterChoice()) != ADMEXIT)
	{
		int acctnum = 0;
		for (; choice == '\n';)
		{
			choice = getchar();
			if (choice == ADMEXIT)
				return;
		}
		switch (choice)
		{
	//	case ADMWRITE:
	//		textFile(cfPtr);
	//		break;
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
			printf("\t\t\t---------------------------------------------------\n");
			printf("\t\t\tPlease input you find information:");
			scanf("%d", &acctnum);
			findRecord(cfPtr, acctnum);
			break;
		case ADMOUTPUT:
			outputInfo();
			break;
		}
		if (choice == ADMEXIT)
		{
			fclose(cfPtr);
			return;
		}
	//	getchar();
	}


}
char enterChoice(void)
{
	char menuChoice[100];

	printf( "\t\t                  ***************************\n"
		    "\t\t                  *     Enter your choice   *\n"
			"\t\t                  * 1 - save information    *\n"
			"\t\t                  * 2 - update an account   *\n"
			"\t\t                  * 3 - add a new account   *\n"
			"\t\t                  * 4 - delete an account   *\n"
			"\t\t                  * 5 - find account        *\n"
			"\t\t                  * 6 - output all account  *\n"
			"\t\t                  * 7 - end program?        *\n"
			"\t\t                  ***************************\n");
	printf("\t\t\t---------------------------------------------------\n");
	printf("\t\t\t\t     select your choice:");
	scanf("%s", menuChoice);
	if (strlen(menuChoice) == 1)
		return menuChoice[0];
	else
	{
		printf("\t\t\t\t error!!!\n");
		return '0';
	}
	//return menuChoice;
}