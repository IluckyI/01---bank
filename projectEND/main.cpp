
#define _CRT_SECURE_NO_WARNINGS
//#include <my_global.h>
//#include <mysql.h>
#include<cstdio>
#include"admin.h"
#include"general.h"
#include"user.h"
MYSQL *con = 0;
static FILE *admfPtr = 0;
static FILE *cfPtr = 0;
static UserData ss_clients[100] = { 0 };
void txt_write();
void Read_Saved_information();
void finish_with_error(MYSQL *con)
{
	fprintf(stderr, "%s\n", mysql_error(con));
	mysql_close(con);
	exit(1);
}


void mysql_con(char **argv)
{
	
	if(con)
		finish_with_error(con);
	con = mysql_init(NULL);
	if (con == NULL)
	{
		fprintf(stderr, "mysql_init() failed\n");
		exit(1);
	}
	if (mysql_real_connect(con, "127.0.0.1", "root", "root",
		"bank", 0, NULL, 0) == NULL)
	{
		finish_with_error(con);
	}
	if (mysql_query(con, "create table if not exists bank(acctNum INT,Name TEXT,Password INT,Balance DOUBLE)"))
	{
		finish_with_error(con);
	}
}
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

void userUpdateRecord(FILE*file, UserPtr user)//   用户修改密码
{
	getchar();
	char a = 0;
	char c[100] = { 0 };
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
	ss_clients[index].password = user->password;//更新数据缓冲
	fseek(cfPtr,
		index * sizeof(UserData),
		SEEK_SET);
	fwrite(&ss_clients[index], sizeof(UserData), 1,
		cfPtr);//更新文件
	sprintf(c, "update bank set Password='%d'where acctNum='%d'", 
		ss_clients[index].password, ss_clients[index].acctNum);
	if (mysql_query(con, c))
	{
		finish_with_error(con);
	}
			   //	updateData(UPDATE, user);//
}
void updateRecord(FILE *fPtr)           //管理员数据更新函数
{
	int account;
	char c[100] = { 0 };
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
	if (transaction < -data.balance)
	{
		printf("\t\t\t\tnot enough money!\n");
		return;

	}
	data.balance += transaction;
	printf("\t\t\t\t%-6d%-25s% 10.2f\n",
		data.acctNum, data.name,
		data.balance);

	updateData(UPDATE, &data);

	sprintf(c,"update bank set balance='%lf'where acctNum='%d'",data.balance,data.acctNum );
	if (mysql_query(con, c))
	{
		finish_with_error(con);
	}
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
		//更新文件
		fseek(cfPtr, index * sizeof(UserData), SEEK_SET);
		data->acctNum = INVALID_ACCOUNT_NO;
		fwrite(data,
			sizeof(UserData), 1, cfPtr);
		//更新内存数据
		ss_clients[index].acctNum = INVALID_ACCOUNT_NO;
	}
	break;
	case UPDATE:
	{
		//更新数据
		ss_clients[index].balance = data->balance;//更新数据缓冲
		fseek(cfPtr,
			index * sizeof(UserData),
			SEEK_SET);
		fwrite(&ss_clients[index], sizeof(UserData), 1,
			cfPtr);//更新文件

	}
	break;
	case INSERT:
	{
		//更新数据记录文件
		fseek(cfPtr, index * sizeof(UserData), SEEK_SET);
		fwrite(data, sizeof(UserData), 1, cfPtr);
		//更新缓冲区数据	
		ss_clients[index] = *data;
	}
	break;
	default:
		break;
	}

	return 0;
}

int main(char **argv)
{
	mysql_con(argv);

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

	//buff_1(cfPtr);
	Read_Saved_information();

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
			cfPtr = fopen(FILE_PATH, "r+");//应改为r+
			initialize(cfPtr);
			admChoice(cfPtr, adminacc);
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
			printf("\t\t\t\t\t\terror！\n");
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

void Read_Saved_information()
{
	if (mysql_query(con, "SELECT * FROM bank "))
	{
		finish_with_error(con);
	}

	MYSQL_RES *result = mysql_store_result(con);

	if (result == NULL)
	{
		finish_with_error(con);
	}

	int num_fields = mysql_num_fields(result);//获取列个数

	MYSQL_ROW row;
	MYSQL_FIELD *field;
	int i = 0;
	while ((row = mysql_fetch_row(result)))//读一行
	{

		ss_clients[i].acctNum = atoi(row[0]);
		strcpy(ss_clients[i].name, row[1]);
		ss_clients[i].password = atoi(row[2]);
		ss_clients[i].balance = atof(row[3]);
		i++;
	}
	mysql_free_result(result);



}

void outputInfo()    //管理员输出所有账户信息（密码除外）
{
	if (mysql_query(con, "SELECT * FROM bank "))
	{
		finish_with_error(con);
	}

	MYSQL_RES *result = mysql_store_result(con);

	if (result == NULL)
	{
		finish_with_error(con);
	}

	int num_fields = mysql_num_fields(result);//获取列个数

	MYSQL_ROW row;
	MYSQL_FIELD *field;
	int i = 0;
	printf("\t\t\t\t%-10s%-10s%-10s\n", "acctnum", "name", "balance");
	while ((row = mysql_fetch_row(result)))
	{
		printf("\t\t\t\t");
		for (int i = 0; i < num_fields; i++)
		{
			if (i == 2)
				continue;
			printf("%-10s ", row[i] ? row[i] : "NULL");
		}
		printf("\n");
	}
	mysql_free_result(result);
	mysql_close(con);


	//printf("\t\t\t\t%-6s  %-16s%10s\n", "acctnum", "name", "balance");
	//int i = 0;
	//for (; i<100; i++)
	//{
	//	if (ss_clients[i].acctNum == 0)
	//		continue;
	//	printf("\t\t\t\t%-6d  %-16s%10.2lf\n", ss_clients[i].acctNum, ss_clients[i].name, ss_clients[i].balance);
	//}
}

void newRecord(FILE *fPtr)
{
	UserData client = { 0, 0, "", 0.0 };
	int accountNum;
	char c[100] = { 0 };
	printf("\t\t\tEnter new account number : ");
	scanf("%s", c);

	int size = 0;
	size = strlen(c);
	for (int i = 0; i < size&&c[i] != '\n'; i++)
	{
		if (c[i] < '0' || c[i] > '9')
		{
			printf("\t\t\t\tinput error!!!!!\n");
			printf("\t\t\t\tPlease input number !!!\n");
			return;
		}
	}
	accountNum = atoi(c);

	//找到该记录在列表中的索引
	int index = recordIndex(accountNum);
	if (-1 != index)//如果该记录已经存在
	{
		printf("\t\t\tAccount  already contains information.\n");
		printf("\t\t\t---------------------------------------------------\n");
		return;
	}
	printf("\t\t\tEnter password,name, balance\n ");
	printf("\t\t\t      ?");
	scanf("%d%s%lf", &client.password, client.name,
		&client.balance);
	client.acctNum = accountNum;

	updateData(INSERT, &client);
	sprintf(c,"INSERT INTO bank VALUES(%d, '%s', '%d','%lf')",
		client.acctNum,client.name,client.password,client.balance);

	if (mysql_query(con, c)) {
		finish_with_error(con);
	}

	printf("\t\t\t\t   newrecord successful!\n");
	printf("\t\t\t---------------------------------------------------\n");
}

void deleteRecord(FILE *fPtr)      //管理员的删除
{
	int accountNum;
	char c[50] = { 0 };
	printf("\t\t\tEnter account number to "
		"delete: ");
	scanf("%d", &accountNum);

	UserData 	blankClient = { INVALID_ACCOUNT_NO, 0, "", 0 };
	blankClient.acctNum = accountNum;
	if (updateData(Delect, &blankClient) == -1)
		return;

	sprintf(c, "delete from bank where acctNum='%d'",accountNum);
	if (mysql_query(con, c)) 
	{
		finish_with_error(con);
	}

	printf("\t\t\tdelete record successful!\n");

}

void findRecord(FILE*file, int acctNum)  //用户查询其他用户的标准信息（即姓名，账号，余额）
{
	char c[50] = { 0 };
	UserData Data;
	sprintf(c, "SELECT *FROM bank WHERE acctNum = '%d'", acctNum);
	if (mysql_query(con, c))
	{
		finish_with_error(con);
	}
	MYSQL_RES *result = mysql_store_result(con);
	if (result->row_count == 0)
	{
		printf("\t\t\t#%d no exist!\n",acctNum);
		return;
	}
	if (result == NULL)
	{
		finish_with_error(con);
	}
	MYSQL_ROW row = mysql_fetch_row(result);
	Data.acctNum = atoi(row[0]);
	strcpy(Data.name, row[1]);
	Data.password = atoi(row[2]);
	Data.balance = atof(row[3]);
	printf("\t\t\t\t%-6s  %-16s%10s\n", "acctnum", "name", "balance");
	printf("\t\t\t\t%-6d  %-16s%10.2lf\n", Data.acctNum, Data.name, Data.balance);

	printf("\t\t\t---------------------------------------------------\n");
}