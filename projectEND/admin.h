#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"general.h"

#ifndef __ADMIN_CPP__
#define __ADMIN_CPP__


char  enterChoice(void);
void deleteRecord(FILE *);
void outputInfo();
char * enterNum(void);
void newRecord(FILE *);
void admChoice(FILE*cfPtr, admin * adminacc);


#endif