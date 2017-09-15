#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"general.h"
#ifndef __USER_CPP__
#define __USER_CPP__



void outputRecord(UserPtr);
void userUpdateRecord(FILE*file, UserPtr user);
int enterUserAcc(void);
char enterUser(void);

void Userchoice(UserData client[100]);

#endif