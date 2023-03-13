#ifndef COMMON_H_
#define COMMON_H_

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

int checkFile(const char *path, FILE **fl, int decision);

int noFileErrorPrint(const char *fileName, int isDir);

#endif  //  COMMON_H_