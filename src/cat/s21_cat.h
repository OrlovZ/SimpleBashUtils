#ifndef S21_CAT_H_
#define S21_CAT_H_

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/common.h"

typedef struct enabledOptions {
  int bOpt;
  int eOpt;
  int nOpt;
  int sOpt;
  int tOpt;
  int vOpt;
  int eGnuOpt;
  int tGnuOpt;
  char err;
  int result;
} enabledOptions;

enabledOptions parseOpt(const char *str);

int illegalOptionErrorPrint(char ch);

int printFile(enabledOptions *opt, FILE *fl);

void printLine(char **line, int *lineNumber, int isLineEmpty,
               int isLineEmptyAgain, enabledOptions *opt, int lineLength);

void handleB(char **line, int *lineNumber, int lineLength, int lineEmptyStatus);

void specialCharPrint(char ch);

void handleE(char **line, enabledOptions *opt, int lineLength);

void handleV(char **line, int lineLength);

void handleT(char **line, enabledOptions *opt, int lineLength);

void optInit(enabledOptions *opt);

void printer(char **line, int lineLength);

#endif  //  S21_CAT_H_
