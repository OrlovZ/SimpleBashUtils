#ifndef S21_GREP_H_
#define S21_GREP_H_

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/common.h"

#define LENGTH 4096

typedef struct enabledOptions {
  int eOpt;
  int iOpt;
  int vOpt;
  int cOpt;
  int lOpt;
  int nOpt;
  int hOpt;
  int sOpt;
  int fOpt;
  int oOpt;
  int noOpt;
  int result;
} enabledOptions;

void optInit(enabledOptions *opt);

void lineHandle(char *filename, char *line, enabledOptions *opt,
                int *lineNumber, int *countMatches, regex_t *preg);

void fileCountChecker(int argc, char ***argv, enabledOptions *opt,
                      char *patterns, int argFileInd, int *fileCount);

void grepPrintL(char *filename);

void grepPrintO(regex_t *preg, char *line, regmatch_t *pmatch, char *filename,
                enabledOptions *opt, int lineNumber);

void grepPrintC(enabledOptions *opt, char *filename, int countMatches);

int parseFlagsPatternsAndFiles(int argc, char ***argv, enabledOptions *opt,
                               char *patterns, int *argFileInd);

void getOptFunc(int argc, char ***argv, enabledOptions *opt, char *patterns,
                int *argFileInd);

void errorOption(enabledOptions *opt);

void switchingOptions(int res, enabledOptions *opt, char *patterns, FILE **fl);

void addPattern(char *patterns, char *pattern);

void addPatternsFromFile(char *patterns, FILE **fl, enabledOptions *opt);

void illegalOptionErrorPrint(char ch);

void noArgErrorPrint(char ch);

void grepUsagePrint();

void gotOption(enabledOptions *opt);

void grepPatternsInFiles(int argc, char ***argv, enabledOptions *opt,
                         char *patterns, int argFileInd, int *fileNumber,
                         int *fileCount);

int fileCounter(int argc, char ***argv, int argFileInd);

#endif  //  S21_GREP_H_
