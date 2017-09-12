#include<cstdio>
#include"user.h"
#define _CRT_SECURE_NO_WARNINGS
void outputRecord(UserPtr user)  //用户显示自己账户信息（密码除外）
{
	printf("%-6s  %-16s%10s\n", "acctnum", "name", "balance");
	printf("%-6d  %-16s%10.2lf\n", user->acctNum, user->name, user->balance);

}
void userUpdateRecord(FILE*file, UserPtr user)//   用户修改密码
{
	getchar();
	char a = 0;
	printf("change your password?(Y/N)\n");
	scanf("%c", &a);
	if (a == 'N' || a == 'n')
		return;
	int Newinfo;
	printf("Please input New password:\n");
	scanf("%d", &Newinfo);
	//data.balance += transaction;
	user->password = Newinfo;
	printf("your New password is %d\n", user->password);
	fseek(file, -sizeof(UserData), SEEK_CUR);
	fwrite(user, sizeof(UserData), 1, file);
	//	updateData(UPDATE, user);
}

int enterUserAcc(void)   //输入你的账户
{
	int accnum;
	printf("Please input your accNum\n");
	scanf("%d", &accnum);
	return accnum;
}
int enterUser(void)     //用户选项
{
	int menuChoice;

	printf("                  ***************************\n"
		"                  *     Enter your choice   *\n"
		"                  * 1 - save information    *\n"
		"                  * 2 - update information  *\n"
		"                  * 3 - output information  *\n"
		"                  * 4 - find acctnum info   *\n"
		"                  * 5 - end program?        *\n"
		"                  ***************************\n");
	scanf("%d", &menuChoice);
	return menuChoice;
}



void Userchoice(void)      //用户命令
{
	ufPtr = fopen(FILE_PATH, "r+");//应改为r+
	UserPtr user = (UserPtr)malloc(sizeof(UserData));
	int acc = 0;
	acc = enterUserAcc();
	int i = 0;
	fseek(ufPtr, 0, SEEK_SET);
	while (!feof(ufPtr))
	{
		size_t ret = fread(user, sizeof(UserData), 1,
			ufPtr);
		if (ret != 1)
		{
			break;
		}
		//fread(user, sizeof(UserData), 1, ufPtr);
		if (acc == user->acctNum)
		{
			break;
		}
		fseek(ufPtr, i * sizeof(UserData), SEEK_SET);
		i++;
	}
	if (feof(ufPtr))
	{
		printf("Don't have this account!\n");
		return;
	}
	int password; //= enterPassword();
	int j;
	for (j = 0; j < 3; j++)
	{
		password = enterPassword();
		if (password == user->password)
			break;
		printf("this password is error!\n");


	}
	if (j == 3)
	{
		printf("input three error password!\n");
		return;
	}
	FILE *file = ufPtr;
	int choice;
	while ((choice = enterUser()) != USEREXIT)
	{

		switch (choice)
		{
		case USERWRITE:
			textFile(ufPtr);
			break;
		case USERUPDATE:
			userUpdateRecord(ufPtr, user);
			break;
		case USEROUTPUT:
			outputRecord(user);
			break;
		case USERFIND:
			int acctnum = 0;
			printf("Please input you find information\n");
			scanf("%d", &acctnum);
			findRecord(file, acctnum);
			break;
		}
	}
	free(user);
}
