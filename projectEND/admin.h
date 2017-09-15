#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"general.h"

#ifndef __ADMIN_CPP__
#define __ADMIN_CPP__


char  enterChoice(void);
void deleteRecord(FILE *);
void outputInfo(FILE*);
char * enternum(void);
void newRecord(FILE *);
void admchoice(FILE*cfPtr, admin * adminacc);


#endif