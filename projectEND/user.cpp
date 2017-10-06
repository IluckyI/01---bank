#include<cstdio>
#include"user.h"
#define _CRT_SECURE_NO_WARNINGS
void outputRecord(UserPtr user)  //用户显示自己账户信息（密码除外）
{
	printf("\t\t\t\t%-6s  %-16s%10s  %-3s\n", "acctnum", "name", "balance","password");
	printf("\t\t\t\t%-6d  %-16s%10.2lf   %-3d\n", user->acctNum, user->name, user->balance,user->password);

}
//void userUpdateRecord(FILE*file, UserPtr user)//   用户修改密码
//{
//	getchar();
//	char a = 0;
//	printf("\t\t\t\t change your password?(Y/N):");
//	scanf("%c", &a);
//	if (a == 'N' || a == 'n')
//		return;
//	int Newinfo;
//	printf("\t\t\t\t Please input New password:");
//	scanf("%d", &Newinfo);
//	//data.balance += transaction;
//	user->password = Newinfo;
//	printf("\t\t\t\t your New password is %d\n", user->password);
//	fseek(file, -sizeof(UserData), SEEK_CUR);
//	fwrite(user, sizeof(UserData), 1, file);
//	ss_clients[index].balance = user->password);//更新数据缓冲
//	fseek(cfPtr,
//		index * sizeof(UserData),
//		SEEK_SET);
//	fwrite(&ss_clients[index], sizeof(UserData), 1,
//		cfPtr);//更新文件
//
////	updateData(UPDATE, user);//
//}

int enterUserAcc(void)   //输入你的账户
{
	int accnum;

	printf("\t\t\tPlease input your accNum:");
	scanf("%d", &accnum);
	return accnum;
}
char enterUser(void)     //用户选项
{
	char menuChoice[100];

	printf( "\t\t                  ***************************\n"
			"\t\t                  *     Enter your choice   *\n"
			"\t\t                  * 1 - save information    *\n"
			"\t\t                  * 2 - update password     *\n"
			"\t\t                  * 3 - output information  *\n"
			"\t\t                  * 4 - find acctnum info   *\n"
			"\t\t                  * 5 - end program?        *\n"
			"\t\t                  ***************************\n");
	printf("\t\t\t---------------------------------------------------\n");
	printf("\t\t\t       your choice:");
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



void Userchoice(UserData client[100])      //用户命令
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
		printf("\t\t\t Don't have this account!\n");
		return;
	}
	printf("\n");

	int index = recordIndex(acc);



	int password; //= enterPassword();
	int j;
	for (j = 0; j < 3; j++)
	{
		password = enterPassword();
		if (password == client[index].password)
			break;
		printf("\t\t\t this password is error!\n");
		printf("\t\t\t---------------------------------------------------\n");

	}
	if (j == 3)
	{
		printf("\t\t\t input three error password!\n");
		printf("\t\t\t---------------------------------------------------\n");
		return;
	}
	printf("\t\t\t---------------------------------------------------\n");
	FILE *file = ufPtr;
	char choice;
	while ((choice = enterUser()) != USEREXIT)
	{
		for (; choice == '\n';)
		{
			choice = getchar();
			if (choice == USEREXIT)
				return;
		}
		switch (choice)
		{
		case USERWRITE:
			//textFile(ufPtr);
			printf("\t\t\t---------------------------------------------------\n");
			break;
		case USERUPDATE:
			userUpdateRecord(ufPtr, user);
			printf("\t\t\t---------------------------------------------------\n");
			break;
		case USEROUTPUT:
			outputRecord(user);
			printf("\t\t\t---------------------------------------------------\n");
			break;
		case USERFIND:
			int acctnum = 0;
			printf("\t\t\t\tPlease input you find acctnum:");
			scanf("%d", &acctnum);
			findRecord(file, acctnum);
			printf("\t\t\t---------------------------------------------------\n");
			break;

		}
		//getchar();
	}
	free(user);
}
